import { useState } from 'react';
import { useMutation } from '@tanstack/react-query';
import axios from 'axios';

interface ReceiptDraft {
  vendorName: string;
  grossTotal: string;
  taxAmount: string;
  costCenter: string;
  description: string;
  needsExpenseReport: boolean;
}

const defaultDraft: ReceiptDraft = {
  vendorName: '',
  grossTotal: '',
  taxAmount: '',
  costCenter: '',
  description: '',
  needsExpenseReport: true
};

interface ProcessedReceiptResponse {
  id: string;
  status: 'draft' | 'processing' | 'completed';
  nextAction?: string;
}

export function ReceiptIntakePanel() {
  const [draft, setDraft] = useState<ReceiptDraft>(defaultDraft);
  const [imageFile, setImageFile] = useState<File | null>(null);

  const mutation = useMutation({
    mutationFn: async () => {
      if (!imageFile) {
        throw new Error('Valitse ensin kuitti.');
      }

      const formData = new FormData();
      formData.append('receipt', imageFile);
      formData.append('metadata', JSON.stringify(draft));

      const response = await axios.post<ProcessedReceiptResponse>('/api/receipts', formData, {
        headers: { 'Content-Type': 'multipart/form-data' }
      });

      return response.data;
    }
  });

  const handleReset = () => {
    setDraft(defaultDraft);
    setImageFile(null);
    mutation.reset();
  };

  return (
    <div className="space-y-6">
      <div className="rounded-2xl border border-primary/20 bg-white/60 p-6 shadow-sm">
        <h3 className="text-lg font-semibold text-primary">1. Lataa kuitti</h3>
        <p className="mt-2 text-sm text-primary/70">
          Tuo mobiilisovelluksen ottama kuva tai vedä tiedosto tähän. Sovellus tunnistaa kuittikentät automaattisesti.
        </p>
        <label
          htmlFor="receipt"
          className="mt-4 flex cursor-pointer flex-col items-center justify-center rounded-xl border-2 border-dashed border-primary/40 bg-primary/5 p-8 text-center text-primary/70 transition hover:border-accent hover:text-accent"
        >
          <span className="text-sm font-medium">
            {imageFile ? imageFile.name : 'Valitse tai pudota kuittikuva (.jpg, .png, .pdf)'}
          </span>
          <input
            id="receipt"
            name="receipt"
            type="file"
            accept="image/*,.pdf"
            className="hidden"
            onChange={(event) => {
              const file = event.target.files?.[0];
              if (file) {
                setImageFile(file);
              }
            }}
          />
        </label>
      </div>

      <div className="grid grid-cols-1 gap-4 md:grid-cols-2">
        <div className="rounded-2xl border border-primary/20 bg-white/60 p-6 shadow-sm">
          <h3 className="text-lg font-semibold text-primary">2. Tarkista tiedot</h3>
          <div className="mt-4 space-y-4">
            <Field
              label="Toimittaja"
              placeholder="Esim. Oy Palvelu Ab"
              value={draft.vendorName}
              onChange={(vendorName) => setDraft((prev) => ({ ...prev, vendorName }))}
            />
            <div className="grid grid-cols-1 gap-4 sm:grid-cols-2">
              <Field
                label="Bruttosumma (€)"
                value={draft.grossTotal}
                onChange={(grossTotal) => setDraft((prev) => ({ ...prev, grossTotal }))}
              />
              <Field
                label="ALV (€)"
                value={draft.taxAmount}
                onChange={(taxAmount) => setDraft((prev) => ({ ...prev, taxAmount }))}
              />
            </div>
            <Field
              label="Kustannuspaikka"
              placeholder="Esim. 401 Matkakulut"
              value={draft.costCenter}
              onChange={(costCenter) => setDraft((prev) => ({ ...prev, costCenter }))}
            />
            <Field
              label="Kuvaus"
              placeholder="Agentti täydentää kuittirivit automaattisesti"
              value={draft.description}
              onChange={(description) => setDraft((prev) => ({ ...prev, description }))}
              multiline
            />
            <label className="flex items-center gap-3 text-sm font-medium text-primary">
              <input
                type="checkbox"
                checked={draft.needsExpenseReport}
                onChange={(event) =>
                  setDraft((prev) => ({ ...prev, needsExpenseReport: event.target.checked }))
                }
                className="h-4 w-4 rounded border-primary/40 text-accent focus:ring-warning"
              />
              Kirjataan kululaskuun
            </label>
          </div>
        </div>

        <div className="flex h-full flex-col justify-between rounded-2xl border border-primary/20 bg-white/60 p-6 shadow-sm">
          <div>
            <h3 className="text-lg font-semibold text-primary">3. Lähetä agentille</h3>
            <p className="mt-2 text-sm text-primary/70">
              Agentti validoi summat, varmistaa ALV:n ja ehdottaa kirjauksia. Tulokset näkyvät sekunneissa.
            </p>
          </div>
          <div className="space-y-3">
            <button
              type="button"
              onClick={() => mutation.mutate()}
              disabled={mutation.isPending || !imageFile}
              className="flex w-full items-center justify-center rounded-full bg-accent px-6 py-3 text-sm font-semibold text-primary transition disabled:cursor-not-allowed disabled:bg-accent/50"
            >
              {mutation.isPending ? 'Prosessoidaan…' : 'Lähetä kuitti'}
            </button>
            <button
              type="button"
              onClick={handleReset}
              className="w-full rounded-full border border-primary/20 px-6 py-3 text-sm font-semibold text-primary transition hover:border-warning hover:text-warning"
            >
              Tyhjennä kentät
            </button>
            {mutation.isError && (
              <p className="rounded-xl bg-warning/20 px-4 py-3 text-sm font-medium text-warning">
                Lähetys epäonnistui: {(mutation.error as Error).message}
              </p>
            )}
            {mutation.isSuccess && (
              <p className="rounded-xl bg-accent/20 px-4 py-3 text-sm font-medium text-primary">
                Kuitti vastaanotettu (#{mutation.data.id}). {mutation.data.nextAction ?? 'Agentti viimeistelee kirjauksen automaattisesti.'}
              </p>
            )}
          </div>
        </div>
      </div>
    </div>
  );
}

interface FieldProps {
  label: string;
  value: string;
  onChange: (value: string) => void;
  placeholder?: string;
  multiline?: boolean;
}

function Field({ label, value, onChange, placeholder, multiline = false }: FieldProps) {
  const baseClasses =
    'mt-1 w-full rounded-xl border border-primary/20 bg-surface px-4 py-3 text-sm text-primary shadow-sm placeholder:text-primary/30 focus:border-accent focus:outline-none focus:ring-2 focus:ring-warning/70';

  if (multiline) {
    return (
      <label className="block text-sm font-medium text-primary">
        {label}
        <textarea
          rows={3}
          value={value}
          placeholder={placeholder}
          onChange={(event) => onChange(event.target.value)}
          className={baseClasses}
        />
      </label>
    );
  }

  return (
    <label className="block text-sm font-medium text-primary">
      {label}
      <input
        value={value}
        placeholder={placeholder}
        onChange={(event) => onChange(event.target.value)}
        className={baseClasses}
      />
    </label>
  );
}
