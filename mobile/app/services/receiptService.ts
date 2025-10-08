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

const RECEIPT_PIPELINE_ENDPOINT = 'https://api.example.com/receipts/process';

async function copyImageToArchive(originalPath: string, receiptId: string): Promise<string> {
  const extension = originalPath.split('.').pop() ?? 'jpg';
  const destinationDir = `${RNFS.DocumentDirectoryPath}/receipts/${receiptId}`;
  await RNFS.mkdir(destinationDir);
  const destinationPath = `${destinationDir}/original.${extension}`;
  await RNFS.copyFile(originalPath, destinationPath);
  return destinationPath;
}

async function uploadImage(originalPath: string): Promise<string> {
  // Placeholder implementation – replace with multipart upload to object storage.
  const fileData = await RNFS.readFile(originalPath, 'base64');
  const payload = JSON.stringify({
    filename: originalPath.split('/').pop(),
    contentType: 'image/jpeg',
    data: fileData,
  });

  const response = await fetch('https://api.example.com/receipts/upload', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: payload,
  });

  if (!response.ok) {
    throw new Error('Receipt upload failed');
  }

  const { assetUrl } = (await response.json()) as { assetUrl: string };
  return assetUrl;
}

async function callProcessingPipeline(assetUrl: string, receiptId: string): Promise<ProcessReceiptResponse> {
  const response = await fetch(RECEIPT_PIPELINE_ENDPOINT, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ assetUrl, receiptId, platform: Platform.OS }),
  });

  if (!response.ok) {
    throw new Error('Receipt processing failed');
  }

  return (await response.json()) as ProcessReceiptResponse;
}

export async function processReceipt(imagePath: string): Promise<ReceiptExtractionResult> {
  const receiptId = uuidv4();
  const archivedImagePath = await copyImageToArchive(imagePath, receiptId);
  const assetUrl = await uploadImage(archivedImagePath);
  const processed = await callProcessingPipeline(assetUrl, receiptId);

  const draft: ReceiptDraft = {
    id: receiptId,
    imagePath: archivedImagePath,
    capturedAt: new Date().toISOString(),
    amount: processed.amount,
    taxAmount: processed.taxAmount,
    currency: processed.currency,
    merchantName: processed.merchantName,
    purchaseDate: processed.purchaseDate,
    paymentMethod: processed.paymentMethod,
    costCenter: processed.costCenter,
    expenseReportId: processed.expenseReportId,
    status: 'processed',
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
