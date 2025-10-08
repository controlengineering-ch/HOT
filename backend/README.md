# Riku Hoitaa Backend

This service exposes the first set of APIs required by the Riku Hoitaa mobile receipt scanner. It accepts receipt images, stores
metadata, and orchestrates downstream processing hooks so that receipts can be archived and forwarded to expense reports and
bookkeeping pipelines.

## Getting started

```bash
cd backend
npm install
npm run dev
```

By default the API listens on `http://localhost:4000`.

## Key endpoints

- `POST /api/receipts` – Upload a new receipt with a multipart form containing an image and JSON metadata. The server validates
  the payload, stores the image in `storage/images`, and persists metadata in `storage/receipts.json`.
- `GET /api/receipts` – List stored receipts with pagination and filtering options.
- `GET /api/receipts/:id` – Fetch a single receipt, including URLs for the archived image and processing status.
- `PUT /api/receipts/:id` – Update bookkeeping flags, free-form fields, or attach the receipt to an expense report once it has
  been reviewed.

## Configuration

Environment variables are optional:

- `PORT` – API port (default `4000`).
- `STORAGE_DIR` – Base directory for metadata and images (default `storage`).
- `PUBLIC_BASE_URL` – Public host for constructing shareable image URLs (default resolves to the local server path).

## Development notes

- Storage uses the file system to ease local prototyping. The repository service can later be swapped for PostgreSQL or object
  storage without modifying the routes.
- Validation is handled with Zod to keep the same schema definitions across mobile and backend implementations.
- Logging uses a minimal wrapper around `console` so the interface can be swapped for a structured logger in production.

## Roadmap

1. Replace local file storage with Cloud Storage / S3 implementation.
2. Wire OCR and LLM agent queue producers after successful uploads.
3. Secure endpoints with OAuth2 and signed upload URLs.
4. Expand the reporting endpoints to emit XML/CSV exports on demand.
