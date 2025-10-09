import { useState } from 'react';
import { ReceiptIntakePanel } from './components/ReceiptIntakePanel';
import { RecentReceipts } from './components/RecentReceipts';

const navItems = [
  { id: 'dashboard', label: 'Yhteenveto' },
  { id: 'receipts', label: 'Kuitit' },
  { id: 'reports', label: 'Raportit' }
];

export default function App() {
  const [activeNav, setActiveNav] = useState(navItems[0].id);

  return (
    <div className="flex h-full flex-col bg-surface text-primary">
      <header className="border-b border-primary/20 bg-primary px-8 py-6 text-surface shadow-lg">
        <div className="flex items-center justify-between">
          <div>
            <p className="text-sm font-semibold uppercase tracking-[0.35em] text-warning">
              Riku Hoitaa
            </p>
            <h1 className="mt-1 text-3xl font-bold text-surface">Talouden ohjauspöytä</h1>
            <p className="mt-2 max-w-xl text-sm text-surface/80">
              Hallitse kuitteja, kuluraportteja ja kassavirtaa yhdestä näkymästä. Agentti ehdottaa seuraavia toimenpiteitä reaaliaikaisen datan perusteella.
            </p>
          </div>
        </div>
        <nav className="mt-6 flex gap-6">
          {navItems.map((item) => (
            <button
              key={item.id}
              className={`rounded-full px-4 py-2 text-sm font-semibold transition focus:outline-none focus-visible:ring-2 focus-visible:ring-warning focus-visible:ring-offset-2 focus-visible:ring-offset-primary ${
                activeNav === item.id
                  ? 'bg-accent text-primary shadow'
                  : 'bg-primary/60 text-surface hover:bg-primary/80'
              }`}
              type="button"
              onClick={() => setActiveNav(item.id)}
            >
              {item.label}
            </button>
          ))}
        </nav>
      </header>

      <main className="flex flex-1 flex-col gap-6 overflow-y-auto bg-surface px-8 py-8 lg:flex-row">
        <section className="w-full flex-1 rounded-3xl border border-primary/10 bg-white/80 p-8 shadow-sm backdrop-blur">
          <h2 className="text-2xl font-semibold text-primary">Kuittien nopea sisäänkirjaus</h2>
          <p className="mt-2 text-sm text-primary/80">
            Aloita uuden kuitin käsittely, tarkista OCR-tulokset ja ohjaa kirjaukset oikeaan kustannuspaikkaan. Tila päivittyy taustalla agentin suorittamien tarkistusten perusteella.
          </p>
          <div className="mt-6">
            <ReceiptIntakePanel />
          </div>
        </section>

        <aside className="w-full max-w-xl rounded-3xl border border-primary/10 bg-primary/95 p-8 text-surface shadow-lg">
          <h2 className="text-2xl font-semibold">Viimeisimmät kuitit</h2>
          <p className="mt-2 text-sm text-surface/80">
            Seuraa käsiteltyjen kuittien etenemistä ja varmista, että kaikki on kirjattu kululaskuun ja maksettu ajallaan.
          </p>
          <div className="mt-6">
            <RecentReceipts />
          </div>
        </aside>
      </main>
    </div>
  );
}
