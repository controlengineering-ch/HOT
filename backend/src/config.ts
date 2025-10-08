import path from 'node:path';

const root = process.cwd();
const storageDir = process.env.STORAGE_DIR ?? path.join(root, 'backend', 'storage');

export const config = {
  port: Number(process.env.PORT ?? 4000),
  storageDir,
  metadataFile: path.join(storageDir, 'receipts.json'),
  imageDir: path.join(storageDir, 'images'),
  publicBaseUrl: process.env.PUBLIC_BASE_URL,
};
