import type { ReceiptListFilters, ReceiptListResponse, ReceiptMetadata } from '../models/receipt.js';
import { FileReceiptRepository, type CreateReceiptInput, type ReceiptRepository, type UpdateReceiptInput } from './storage/ReceiptRepository.js';
import { LocalImageStorage } from './storage/ImageStorage.js';
import type { SaveImageInput } from './storage/ImageStorage.js';

export interface CreateReceiptPayload {
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
  bookedToExpenseReport: boolean;
  paid: boolean;
  inPaymentProcess: boolean;
  freeField1?: string;
  freeField2?: string;
  freeField3?: string;
  freeField4?: string;
  captureDevice?: ReceiptMetadata['captureDevice'];
}

export interface ReceiptServiceDependencies {
  repository?: ReceiptRepository;
  imageStorage?: LocalImageStorage;
}

export class ReceiptService {
  private readonly repository: ReceiptRepository;
  private readonly imageStorage: LocalImageStorage;

  constructor({ repository, imageStorage }: ReceiptServiceDependencies = {}) {
    this.repository = repository ?? new FileReceiptRepository();
    this.imageStorage = imageStorage ?? new LocalImageStorage();
  }

  async create(payload: CreateReceiptPayload, image: SaveImageInput): Promise<ReceiptMetadata> {
    const savedImage = await this.imageStorage.save(image);

    const receiptInput: CreateReceiptInput = {
      submitterId: payload.submitterId,
      submitterName: payload.submitterName,
      vendorName: payload.vendorName,
      description: payload.description,
      currency: payload.currency,
      totalAmount: payload.totalAmount,
      taxAmount: payload.taxAmount,
      transactionDate: payload.transactionDate,
      costCenter: payload.costCenter,
      expenseReportId: payload.expenseReportId,
      captureDevice: payload.captureDevice ?? 'mobile',
      imagePath: savedImage.path,
      imageUrl: savedImage.url,
      ocrStatus: 'pending',
      accountingStatus: payload.bookedToExpenseReport ? 'review' : 'draft',
      flags: {
        bookedToExpenseReport: payload.bookedToExpenseReport,
        paid: payload.paid,
        inPaymentProcess: payload.inPaymentProcess,
      },
      freeFields: {
        freeField1: payload.freeField1,
        freeField2: payload.freeField2,
        freeField3: payload.freeField3,
        freeField4: payload.freeField4,
      },
    };

    return this.repository.create(receiptInput);
  }

  async update(id: string, input: UpdateReceiptInput): Promise<ReceiptMetadata> {
    return this.repository.update(id, input);
  }

  async findById(id: string): Promise<ReceiptMetadata | undefined> {
    return this.repository.findById(id);
  }

  async list(filters: ReceiptListFilters): Promise<ReceiptListResponse> {
    return this.repository.list(filters);
  }
}
