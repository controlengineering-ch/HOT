const mockReceipts = [
  {
    id: 'RH-2451',
    vendor: 'Oy Kahvihetki Ab',
    total: '18,40 €',
    status: 'Odottaa tarkistusta',
    statusTone: 'warning',
    submittedAt: '6 min sitten'
  },
  {
    id: 'RH-2450',
    vendor: 'Matkahuolto',
    total: '42,90 €',
    status: 'Kululaskussa',
    statusTone: 'accent',
    submittedAt: '1 h sitten'
  },
  {
    id: 'RH-2449',
    vendor: 'Pilvipalvelut Oy',
    total: '312,00 €',
    status: 'Kirjattu kirjanpitoon',
    statusTone: 'accent',
    submittedAt: 'Eilen'
  }
] as const;

export function RecentReceipts() {
  return (
    <div className="space-y-4">
      {mockReceipts.map((receipt) => (
        <article
          key={receipt.id}
          className="rounded-2xl border border-surface/10 bg-surface/10 p-4 shadow-sm backdrop-blur"
        >
          <div className="flex items-center justify-between">
            <div>
              <p className="text-xs font-semibold uppercase tracking-widest text-surface/70">
                {receipt.id}
              </p>
              <h3 className="mt-1 text-lg font-semibold text-surface">{receipt.vendor}</h3>
            </div>
            <p className="text-lg font-semibold text-warning">{receipt.total}</p>
          </div>
          <div className="mt-4 flex items-center justify-between text-sm text-surface/80">
            <span
              className={`rounded-full px-3 py-1 text-xs font-semibold ${
                receipt.statusTone === 'accent'
                  ? 'bg-accent/20 text-surface'
                  : 'bg-warning/20 text-warning'
              }`}
            >
              {receipt.status}
            </span>
            <span>{receipt.submittedAt}</span>
          </div>
        </article>
      ))}
    </div>
  );
}
