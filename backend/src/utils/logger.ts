/* eslint-disable no-console */
export type LogLevel = 'info' | 'warn' | 'error';

const format = (level: LogLevel, message: string, meta?: Record<string, unknown>) => {
  const time = new Date().toISOString();
  const metaString = meta ? ` ${JSON.stringify(meta)}` : '';
  return `[${time}] [${level.toUpperCase()}] ${message}${metaString}`;
};

export const logger = {
  info(message: string, meta?: Record<string, unknown>) {
    console.log(format('info', message, meta));
  },
  warn(message: string, meta?: Record<string, unknown>) {
    console.warn(format('warn', message, meta));
  },
  error(message: string, meta?: Record<string, unknown>) {
    console.error(format('error', message, meta));
  },
};
