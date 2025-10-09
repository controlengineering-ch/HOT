import type { NextFunction, Request, Response } from 'express';
import { logger } from '../utils/logger.js';

interface HttpError extends Error {
  status?: number;
  details?: unknown;
}

export const errorHandler = (err: HttpError, _req: Request, res: Response, _next: NextFunction) => {
  const status = err.status ?? 500;
  logger.error(err.message, { status, details: err.details });
  res.status(status).json({
    error: err.message,
    details: err.details,
  });
};
