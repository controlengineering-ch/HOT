export type ReceiptStatus = 'pending' | 'processed' | 'failed';

export interface ReceiptDraft {
  id: string;
  imagePath: string;
  processedImagePath?: string;
  capturedAt: string;
  amount?: number;
  taxAmount?: number;
  currency?: string;
  merchantName?: string;
  purchaseDate?: string;
  paymentMethod?: string;
  costCenter?: string;
  expenseReportId?: string;
  status: ReceiptStatus;
  confirmationRequiredFields?: Array<keyof ReceiptDraft>;
  notes?: string;
}

export interface ReceiptExtractionResult {
  draft: ReceiptDraft;
  rawText: string;
}
