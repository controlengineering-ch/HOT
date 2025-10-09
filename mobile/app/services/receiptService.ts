import { Platform } from 'react-native';
import { v4 as uuidv4 } from 'uuid';
import RNFS from 'react-native-fs';

import { ReceiptDraft, ReceiptExtractionResult } from '../types/receipt';
import { receiptStorage } from '../storage/receiptStorage';

type ProcessReceiptResponse = {
  amount?: number;
  taxAmount?: number;
  currency?: string;
  merchantName?: string;
  purchaseDate?: string;
  paymentMethod?: string;
  costCenter?: string;
  expenseReportId?: string;
  confirmationRequiredFields?: Array<keyof ReceiptDraft>;
  notes?: string;
  rawText: string;
};

const envBaseUrl =
  typeof process !== 'undefined' ? process.env?.EXPO_PUBLIC_API_BASE_URL ?? process.env?.API_BASE_URL : undefined;

const API_BASE_URL = envBaseUrl ?? (Platform.OS === 'android' ? 'http://10.0.2.2:4000' : 'http://localhost:4000');
const RECEIPT_PIPELINE_ENDPOINT = `${API_BASE_URL}/api/receipts/process`;
const SUBMITTER_ID =
  (typeof process !== 'undefined' && process.env?.EXPO_PUBLIC_SUBMITTER_ID) ?? 'mobile-user';
const SUBMITTER_NAME =
  (typeof process !== 'undefined' && process.env?.EXPO_PUBLIC_SUBMITTER_NAME) ?? undefined;

async function copyImageToArchive(originalPath: string, receiptId: string): Promise<string> {
  const extension = originalPath.split('.').pop() ?? 'jpg';
  const destinationDir = `${RNFS.DocumentDirectoryPath}/receipts/${receiptId}`;
  await RNFS.mkdir(destinationDir);
  const destinationPath = `${destinationDir}/original.${extension}`;
  await RNFS.copyFile(originalPath, destinationPath);
  return destinationPath;
}

const ensureFileUri = (filePath: string) => {
  if (filePath.startsWith('file://')) {
    return filePath;
  }
  return `file://${filePath}`;
};

async function callProcessingPipeline(
  imageBase64: string,
  fileName: string,
  receiptId: string,
): Promise<ProcessReceiptResponse> {
  const response = await fetch(RECEIPT_PIPELINE_ENDPOINT, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({
      imageBase64,
      fileName,
      receiptId,
      platform: Platform.OS,
    }),
  });

  if (!response.ok) {
    throw new Error('Receipt processing failed');
  }

  return (await response.json()) as ProcessReceiptResponse;
}

export async function processReceipt(imagePath: string): Promise<ReceiptExtractionResult> {
  const receiptId = uuidv4();
  const archivedImagePath = await copyImageToArchive(imagePath, receiptId);
  const imageBase64 = await RNFS.readFile(archivedImagePath, 'base64');
  const fileName = archivedImagePath.split('/').pop() ?? `receipt-${receiptId}.jpg`;
  let processed: ProcessReceiptResponse;

  try {
    processed = await callProcessingPipeline(imageBase64, fileName, receiptId);
  } catch (error) {
    console.warn('Falling back to placeholder receipt extraction', error);
    processed = {
      currency: 'EUR',
      merchantName: fileName.replace(/\.[^.]+$/, ''),
      purchaseDate: new Date().toISOString(),
      rawText: 'OCR ei ole käytettävissä – tarkista ja täydennä tiedot.',
    };
  }

  let backendId: string | undefined;
  let backendImageUrl: string | undefined;

  try {
    const formData = new FormData();
    formData.append('submitterId', SUBMITTER_ID);
    if (SUBMITTER_NAME) {
      formData.append('submitterName', SUBMITTER_NAME);
    }
    formData.append('vendorName', processed.merchantName ?? 'Tuntematon toimittaja');
    formData.append('description', processed.notes ?? '');
    formData.append('currency', processed.currency ?? 'EUR');
    formData.append('totalAmount', String(processed.amount ?? 0));
    formData.append('taxAmount', String(processed.taxAmount ?? 0));
    formData.append('transactionDate', processed.purchaseDate ?? new Date().toISOString());
    if (processed.costCenter) {
      formData.append('costCenter', processed.costCenter);
    }
    if (processed.expenseReportId) {
      formData.append('expenseReportId', processed.expenseReportId);
    }
    formData.append('bookedToExpenseReport', String(false));
    formData.append('paid', String(false));
    formData.append('inPaymentProcess', String(false));
    formData.append('captureDevice', 'mobile');

    const imageType = fileName.toLowerCase().endsWith('.png') ? 'image/png' : 'image/jpeg';
    formData.append('image', {
      uri: ensureFileUri(archivedImagePath),
      type: imageType,
      name: fileName,
    } as any);

    const response = await fetch(`${API_BASE_URL}/api/receipts`, {
      method: 'POST',
      body: formData,
    });

    if (!response.ok) {
      throw new Error(`Receipt sync failed with status ${response.status}`);
    }

    const backendReceipt = (await response.json()) as {
      id: string;
      imageUrl?: string;
    };

    backendId = backendReceipt.id;
    backendImageUrl = backendReceipt.imageUrl;
  } catch (error) {
    console.warn('Failed to sync receipt with backend', error);
  }

  const draft: ReceiptDraft = {
    id: receiptId,
    imagePath: archivedImagePath,
    backendId,
    backendImageUrl,
    capturedAt: new Date().toISOString(),
    amount: processed.amount,
    taxAmount: processed.taxAmount,
    currency: processed.currency,
    merchantName: processed.merchantName,
    purchaseDate: processed.purchaseDate,
    paymentMethod: processed.paymentMethod,
    costCenter: processed.costCenter,
    expenseReportId: processed.expenseReportId,
    status: backendId ? 'processed' : 'pending',
    confirmationRequiredFields: processed.confirmationRequiredFields,
    notes: processed.notes,
  };

  await receiptStorage.saveDraft(draft);

  return {
    draft,
    rawText: processed.rawText,
  };
}

export async function saveManualReceiptOverride(draft: ReceiptDraft): Promise<void> {
  await receiptStorage.saveDraft({
    ...draft,
    status: 'processed',
  });
}
