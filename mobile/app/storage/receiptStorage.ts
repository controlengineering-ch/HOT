import AsyncStorage from '@react-native-async-storage/async-storage';
import RNFS from 'react-native-fs';

import { ReceiptDraft } from '../types/receipt';

const STORAGE_KEY = 'riku-hoitaa:receipts';

type ReceiptStore = Record<string, ReceiptDraft>;

async function loadStore(): Promise<ReceiptStore> {
  const serialized = await AsyncStorage.getItem(STORAGE_KEY);
  if (!serialized) {
    return {};
  }

  try {
    return JSON.parse(serialized) as ReceiptStore;
  } catch (error) {
    console.warn('Failed to parse receipt storage, resetting', error);
    await AsyncStorage.removeItem(STORAGE_KEY);
    return {};
  }
}

async function persistStore(store: ReceiptStore): Promise<void> {
  await AsyncStorage.setItem(STORAGE_KEY, JSON.stringify(store));
}

async function ensureDirectoryExists(directory: string): Promise<void> {
  const exists = await RNFS.exists(directory);
  if (!exists) {
    await RNFS.mkdir(directory);
  }
}

async function moveProcessedImage(receiptId: string, processedPath: string): Promise<string> {
  const destinationDir = `${RNFS.DocumentDirectoryPath}/receipts/${receiptId}`;
  await ensureDirectoryExists(destinationDir);
  const extension = processedPath.split('.').pop() ?? 'jpg';
  const destinationPath = `${destinationDir}/processed.${extension}`;
  await RNFS.moveFile(processedPath, destinationPath);
  return destinationPath;
}

export const receiptStorage = {
  async saveDraft(draft: ReceiptDraft): Promise<void> {
    const store = await loadStore();
    store[draft.id] = draft;
    await persistStore(store);
  },

  async markForReview(receiptId: string, fields: Array<keyof ReceiptDraft>): Promise<void> {
    const store = await loadStore();
    const draft = store[receiptId];
    if (!draft) {
      throw new Error('Receipt not found');
    }

    store[receiptId] = {
      ...draft,
      confirmationRequiredFields: fields,
      status: 'pending',
    };

    await persistStore(store);
  },

  async attachProcessedImage(receiptId: string, processedPath: string): Promise<void> {
    const store = await loadStore();
    const draft = store[receiptId];
    if (!draft) {
      throw new Error('Receipt not found');
    }

    const archivedPath = await moveProcessedImage(receiptId, processedPath);

    store[receiptId] = {
      ...draft,
      processedImagePath: archivedPath,
    };

    await persistStore(store);
  },

  async listDrafts(): Promise<ReceiptDraft[]> {
    const store = await loadStore();
    return Object.values(store).sort((a, b) => (a.capturedAt < b.capturedAt ? 1 : -1));
  },

  async getDraft(receiptId: string): Promise<ReceiptDraft | undefined> {
    const store = await loadStore();
    return store[receiptId];
  },

  async remove(receiptId: string): Promise<void> {
    const store = await loadStore();
    const draft = store[receiptId];

    if (draft) {
      const directory = `${RNFS.DocumentDirectoryPath}/receipts/${receiptId}`;
      const exists = await RNFS.exists(directory);
      if (exists) {
        await RNFS.unlink(directory);
      }

      delete store[receiptId];
      await persistStore(store);
    }
  },
};
