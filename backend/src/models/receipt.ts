export interface ReceiptFlags {
  bookedToExpenseReport: boolean;
  paid: boolean;
  inPaymentProcess: boolean;
}

export interface ReceiptFreeFields {
  freeField1?: string;
  freeField2?: string;
  freeField3?: string;
  freeField4?: string;
}

export interface ReceiptMetadata {
  id: string;
  createdAt: string;
  updatedAt: string;
  captureDevice: 'mobile' | 'web' | 'import';
  submitterId: string;
  submitterName?: string;
  vendorName: string;
  description?: string;
  currency: string;
  totalAmount: number;
  taxAmount: number;
  transactionDate: string;
  costCenter?: string;
  expenseReportId?: string;
  imagePath: string;
  imageUrl: string;
  ocrStatus: 'pending' | 'processing' | 'completed' | 'failed';
  accountingStatus: 'draft' | 'review' | 'posted';
  flags: ReceiptFlags;
  freeFields: ReceiptFreeFields;
}

export interface ReceiptListFilters {
  status?: ReceiptMetadata['accountingStatus'];
  submitterId?: string;
  expenseReportId?: string;
  page?: number;
  pageSize?: number;
}

export interface ReceiptListResponse {
  items: ReceiptMetadata[];
  total: number;
  page: number;
  pageSize: number;
}
