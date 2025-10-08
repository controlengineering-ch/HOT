import express from 'express';
import multer from 'multer';
import { z } from 'zod';
import { ReceiptService } from '../services/receiptService.js';
import { logger } from '../utils/logger.js';

const upload = multer();
const router = express.Router();
const receiptService = new ReceiptService();

const createReceiptSchema = z.object({
  submitterId: z.string().min(1),
  submitterName: z.string().optional(),
  vendorName: z.string().min(1),
  description: z.string().optional(),
  currency: z.string().length(3),
  totalAmount: z.coerce.number().nonnegative(),
  taxAmount: z.coerce.number().min(0),
  transactionDate: z.string().refine((value) => !Number.isNaN(Date.parse(value)), {
    message: 'transactionDate must be a valid date',
  }),
  costCenter: z.string().optional(),
  expenseReportId: z.string().optional(),
  bookedToExpenseReport: z.coerce.boolean(),
  paid: z.coerce.boolean(),
  inPaymentProcess: z.coerce.boolean(),
  freeField1: z.string().max(2).optional(),
  freeField2: z.string().max(2).optional(),
  freeField3: z.string().max(2).optional(),
  freeField4: z.string().max(2).optional(),
  captureDevice: z.enum(['mobile', 'web', 'import']).optional(),
});

const listSchema = z.object({
  status: z.enum(['draft', 'review', 'posted']).optional(),
  submitterId: z.string().optional(),
  expenseReportId: z.string().optional(),
  page: z.coerce.number().int().min(1).optional(),
  pageSize: z.coerce.number().int().min(1).max(100).optional(),
});

const updateSchema = z.object({
  vendorName: z.string().optional(),
  description: z.string().optional(),
  totalAmount: z.number().nonnegative().optional(),
  taxAmount: z.number().min(0).optional(),
  transactionDate: z
    .string()
    .refine((value) => !Number.isNaN(Date.parse(value)), {
      message: 'transactionDate must be a valid date',
    })
    .optional(),
  costCenter: z.string().optional(),
  expenseReportId: z.string().optional(),
  accountingStatus: z.enum(['draft', 'review', 'posted']).optional(),
  ocrStatus: z.enum(['pending', 'processing', 'completed', 'failed']).optional(),
  flags: z
    .object({
      bookedToExpenseReport: z.boolean().optional(),
      paid: z.boolean().optional(),
      inPaymentProcess: z.boolean().optional(),
    })
    .optional(),
  freeFields: z
    .object({
      freeField1: z.string().max(2).optional(),
      freeField2: z.string().max(2).optional(),
      freeField3: z.string().max(2).optional(),
      freeField4: z.string().max(2).optional(),
    })
    .optional(),
});

router.post('/', upload.single('image'), async (req, res, next) => {
  try {
    const parsed = createReceiptSchema.parse(req.body);
    if (!req.file) {
      return res.status(400).json({ error: 'Receipt image is required' });
    }
    const receipt = await receiptService.create(parsed, {
      buffer: req.file.buffer,
      mimeType: req.file.mimetype,
      originalName: req.file.originalname,
    });
    res.status(201).json(receipt);
  } catch (error) {
    if (error instanceof z.ZodError) {
      return res.status(400).json({ error: 'Validation failed', details: error.flatten() });
    }
    return next(error);
  }
});

router.get('/', async (req, res, next) => {
  try {
    const filters = listSchema.parse(req.query);
    const receipts = await receiptService.list(filters);
    res.json(receipts);
  } catch (error) {
    if (error instanceof z.ZodError) {
      return res.status(400).json({ error: 'Validation failed', details: error.flatten() });
    }
    return next(error);
  }
});

router.get('/:id', async (req, res, next) => {
  try {
    const receipt = await receiptService.findById(req.params.id);
    if (!receipt) {
      return res.status(404).json({ error: 'Receipt not found' });
    }
    res.json(receipt);
  } catch (error) {
    return next(error);
  }
});

router.put('/:id', async (req, res, next) => {
  try {
    const parsed = updateSchema.parse(req.body);
    const receipt = await receiptService.update(req.params.id, parsed);
    res.json(receipt);
  } catch (error) {
    if (error instanceof z.ZodError) {
      return res.status(400).json({ error: 'Validation failed', details: error.flatten() });
    }
    if (error instanceof Error && (error as { status?: number }).status === 404) {
      logger.warn('receipt.update_missing', { id: req.params.id });
    }
    return next(error);
  }
});

export const receiptRoutes = router;
