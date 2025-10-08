import express from 'express';
import { config } from './config.js';
import { errorHandler } from './middleware/errorHandler.js';
import { receiptRoutes } from './routes/receiptRoutes.js';
import { logger } from './utils/logger.js';

const app = express();

app.use(express.json());
app.get('/healthz', (_req, res) => {
  res.json({ status: 'ok' });
});

app.use('/api/receipts', receiptRoutes);
app.use(errorHandler);

app.listen(config.port, () => {
  logger.info(`Riku Hoitaa backend listening on port ${config.port}`);
});
