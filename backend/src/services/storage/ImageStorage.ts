import { promises as fs } from 'node:fs';
import path from 'node:path';
import { config } from '../../config.js';

export interface SaveImageInput {
  buffer: Buffer;
  mimeType: string;
  originalName: string;
}

export interface SaveImageResult {
  path: string;
  url: string;
}

const extensionFromMime = (mimeType: string) => {
  switch (mimeType) {
    case 'image/jpeg':
      return '.jpg';
    case 'image/png':
      return '.png';
    case 'application/pdf':
      return '.pdf';
    default:
      return '';
  }
};

export class LocalImageStorage {
  constructor(private readonly baseDir: string = config.imageDir) {}

  private async ensureDir() {
    await fs.mkdir(this.baseDir, { recursive: true });
  }

  async save({ buffer, mimeType, originalName }: SaveImageInput): Promise<SaveImageResult> {
    await this.ensureDir();
    const timestamp = Date.now();
    const safeName = originalName.replace(/[^a-zA-Z0-9-_\.]/g, '_');
    const extension = extensionFromMime(mimeType);
    const fileName = `${timestamp}-${safeName}${extension}`;
    const filePath = path.join(this.baseDir, fileName);
    await fs.writeFile(filePath, buffer);
    const url = config.publicBaseUrl ? `${config.publicBaseUrl.replace(/\/$/, '')}/images/${fileName}` : filePath;
    return { path: filePath, url };
  }
}
