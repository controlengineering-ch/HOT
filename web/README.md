# Riku Hoitaa WebUI

Agenttipohjaisen taloushallinnan hallintapaneelin Vite + React + Tailwind -pohja. Tämä versio keskittyy kuitinhallintaan ja toimii rinnakkaisena käyttöliittymänä mobiilisovellukselle.

## Käynnistys

> Huom. `npm install` ei ehkä toimi CI-ympäristössä ilman verkko-oikeuksia. Alla oleva ohje olettaa, että npm-yhteys toimii.

```bash
cd web
npm install
npm run dev
```

Palvelu käynnistyy oletuksena osoitteessa <http://localhost:5173>. Rakennetta voi muokata Tailwind-teeman värilaajennuksilla (`tailwind.config.ts`) ja komponenttikohtaisilla osilla `src/components/`-hakemistossa.

## Rakenne

- `src/App.tsx` – Päänäkymä, joka yhdistää kuittien sisäänkirjauspaneelin ja tuoreet kuitit.
- `src/components/ReceiptIntakePanel.tsx` – Lomakekuvaus kuittien lähetykseen agentille.
- `src/components/RecentReceipts.tsx` – Mock-dataan perustuva viimeisimpien kuittien lista, jonka voi korvata API-haulla.
- `src/styles.css` – Tailwindin lisäksi globaaleja typografia- ja väriasetuksia.

Backend-kutsut oletetaan löytyvän polusta `/api/receipts`. Konfiguroi `vite.config.ts`-tiedoston `server.proxy`-asetus, jos käytät erillistä API-hostia paikallisesti.
