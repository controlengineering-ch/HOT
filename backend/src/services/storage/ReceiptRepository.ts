import { promises as fs } from 'node:fs';
import path from 'node:path';
import { nanoid } from 'nanoid';
import { config } from '../../config.js';
import type { ReceiptListFilters, ReceiptListResponse, ReceiptMetadata } from '../../models/receipt.js';
import { logger } from '../../utils/logger.js';

export interface CreateReceiptInput extends Omit<ReceiptMetadata, 'id' | 'createdAt' | 'updatedAt'> {}

export interface UpdateReceiptInput {
  vendorName?: string;
  description?: string;
  totalAmount?: number;
  taxAmount?: number;
  transactionDate?: string;
  costCenter?: string;
  expenseReportId?: string;
  accountingStatus?: ReceiptMetadata['accountingStatus'];
  flags?: Partial<ReceiptMetadata['flags']>;
  freeFields?: Partial<ReceiptMetadata['freeFields']>;
  ocrStatus?: ReceiptMetadata['ocrStatus'];
}

export interface ReceiptRepository {
  create(input: CreateReceiptInput): Promise<ReceiptMetadata>;
  update(id: string, input: UpdateReceiptInput): Promise<ReceiptMetadata>;
  findById(id: string): Promise<ReceiptMetadata | undefined>;
  list(filters: ReceiptListFilters): Promise<ReceiptListResponse>;
}

const ensureStorage = async () => {
  await fs.mkdir(path.dirname(config.metadataFile), { recursive: true });
  try {
    await fs.access(config.metadataFile);
  } catch (error) {
    if ((error as NodeJS.ErrnoException).code === 'ENOENT') {
      await fs.writeFile(config.metadataFile, JSON.stringify([]));
    } else {
      throw error;
    }
  }
};

const applyUpdate = (receipt: ReceiptMetadata, input: UpdateReceiptInput): ReceiptMetadata => {
  return {
    ...receipt,
    vendorName: input.vendorName ?? receipt.vendorName,
    description: input.description ?? receipt.description,
    totalAmount: input.totalAmount ?? receipt.totalAmount,
    taxAmount: input.taxAmount ?? receipt.taxAmount,
    transactionDate: input.transactionDate ?? receipt.transactionDate,
    costCenter: input.costCenter ?? receipt.costCenter,
    expenseReportId: input.expenseReportId ?? receipt.expenseReportId,
    accountingStatus: input.accountingStatus ?? receipt.accountingStatus,
    ocrStatus: input.ocrStatus ?? receipt.ocrStatus,
    flags: {
      ...receipt.flags,
      ...input.flags,
    },
    freeFields: {
      ...receipt.freeFields,
      ...input.freeFields,
    },
    updatedAt: new Date().toISOString(),
  };
};

export class FileReceiptRepository implements ReceiptRepository {
  private async readAll(): Promise<ReceiptMetadata[]> {
    await ensureStorage();
    const content = await fs.readFile(config.metadataFile, 'utf-8');
    return JSON.parse(content) as ReceiptMetadata[];
  }

  private async writeAll(receipts: ReceiptMetadata[]) {
    await fs.writeFile(config.metadataFile, JSON.stringify(receipts, null, 2));
  }

  async create(input: CreateReceiptInput): Promise<ReceiptMetadata> {
    const now = new Date().toISOString();
    const receipt: ReceiptMetadata = {
      ...input,
      id: nanoid(),
      createdAt: now,
      updatedAt: now,
    };
    const receipts = await this.readAll();
    receipts.push(receipt);
    await this.writeAll(receipts);
    logger.info('receipt.created', { id: receipt.id, submitterId: receipt.submitterId });
    return receipt;
  }

  async update(id: string, input: UpdateReceiptInput): Promise<ReceiptMetadata> {
    const receipts = await this.readAll();
    const index = receipts.findIndex((item) => item.id === id);
    if (index === -1) {
      return Promise.reject(Object.assign(new Error('Receipt not found'), { status: 404 }));
    }
    receipts[index] = applyUpdate(receipts[index], input);
    await this.writeAll(receipts);
    logger.info('receipt.updated', { id });
    return receipts[index];
  }

  async findById(id: string): Promise<ReceiptMetadata | undefined> {
    const receipts = await this.readAll();
    return receipts.find((item) => item.id === id);
  }

  async list(filters: ReceiptListFilters): Promise<ReceiptListResponse> {
    const receipts = await this.readAll();
    const { status, submitterId, expenseReportId } = filters;
    let filtered = receipts;
    if (status) {
      filtered = filtered.filter((item) => item.accountingStatus === status);
    }
    if (submitterId) {
      filtered = filtered.filter((item) => item.submitterId === submitterId);
    }
    if (expenseReportId) {
      filtered = filtered.filter((item) => item.expenseReportId === expenseReportId);
    }

    const pageSize = filters.pageSize ?? 25;
    const page = filters.page ?? 1;
    const start = (page - 1) * pageSize;
    const items = filtered.slice(start, start + pageSize);

    return {
      items,
      total: filtered.length,
      page,
      pageSize,
    };
  }
}
