# Riku Hoitaa – Mobile App Concept for IO:S

## Vision
Riku Hoitaa on agenttipohjainen mobiilisovellus, joka tarjoaa mahdollisimman vaivattoman ja reaaliaikaisen tavan hoitaa yrityksen ja yrittäjän kirjanpitoa sekä talouden seurantaa. Sovellus yhdistää automaattisen tiedonkeruun, älykkään päätöksenteon ja selkeän käyttöliittymän, jotta käyttäjä pysyy ajan tasalla taloudestaan ilman manuaalista työtä.

## Kohdeyleisö
- Pienyrittäjät ja freelancerit, jotka tarvitsevat reaaliaikaista näkyvyyttä kassavirtaan.
- Taloushallinnon ammattilaiset, jotka haluavat yhdistää asiakkaiden tiedot yhteen näkymään.
- Sijoittajat ja hallituksen jäsenet, jotka tarvitsevat nopean tavan tarkistaa taloudelliset tunnusluvut.

## Väripaletti ja käyttöliittymän periaatteet
| Väri | Tarkoitus |
| --- | --- |
| **Vihreä (#3CB371)** | Positiiviset tunnusluvut, hyväksytyt tapahtumat, kasvutrendit |
| **Keltainen (#FFD166)** | Huomiota vaativat tilanteet, muistutukset |
| **Valkoinen (#FFFFFF)** | Taustat ja kortit, joissa esitetään tietoa selkeästi |
| **Tumma navy blue (#0B1D3D)** | Navigaatio, yläpalkit, tumma tila |

Käyttöliittymä on modulaarinen ja korttipohjainen. Tärkeimmät luvut on tuotu yläosaan vihreillä ja keltaisilla aktsenteilla. Tumma sininen luo luotettavuuden tuntua ja erottuu valkoisesta datataustasta.

## Käyttökokemuksen kulku
1. **Agenttialoitus** – Käyttäjä antaa luvan yhdistää pankkitilit, laskutusjärjestelmät ja kuittiskannerin. Agentti oppii käyttäjän tavoitteet (esim. kassavirran minimitaso, veron ennakointi).
2. **Älykäs keruu** – Agentti hakee tiedot rajapinnoista, luo automaattisesti kirjaukset ja ehdottaa kategorioita kuiteille.
3. **Reaaliaikainen dashboard** – Päänäkymä näyttää kassavirran, kulut ja verovarauman. Vihreät trendiviivat korostavat positiivista kehitystä, keltaiset ilmoittavat riskeistä.
4. **Toimenpidekortit** – Agentti ehdottaa konkreettisia toimia (esim. “Maksa arvonlisävero 12.5.”) ja merkitsee ne suorituksen jälkeen.
5. **Raportointi ja vienti** – Käyttäjä voi jakaa raportit kirjanpitäjälle tai ladata sähköiseen arkistoon.

## Keskeiset ominaisuudet
- **Agenttipohjainen automaatio**: LLM-agentit tunnistavat tapahtumien luokat, täydentävät puuttuvia tietoja ja kommunikoivat käyttäjän puolesta esim. laskutuksen kanssa.
- **Reaaliaikainen kassavirtaseuranta**: Pankkitilien ja maksupalveluiden API-yhteydet päivittyvät sekuntitasolla.
- **Verosuunnittelu**: Agentti laskee ALV:n, ennakkoverot ja muistuttaa tulevista eräpäivistä.
- **Budjetointi ja ennusteet**: Ennustemallit käyttävät historiallista dataa ja ulkoisia indikaattoreita tuottaakseen 30/60/90 päivän näkymiä.
- **Moniagentti-yhteistyö**: Oma agentti, kirjanpitäjän agentti ja compliance-agentti jakavat tietoa, mutta noudattavat tietoturvapolitiikkaa.

## Kehitysvaihe 1 – Älykäs kuittiskanneri
Ensimmäinen toimitettava ominaisuus on kuittien tallentaminen ja litterointi käyttäjän mobiililaitteen kameralla. Tavoitteena on, että kuittien digitalisointi on nopeaa, virheetöntä ja että lopputulos voidaan kirjata kirjanpidon tapahtumaksi ilman manuaalista työtä.

### Käyttötapaus
1. Käyttäjä avaa sovelluksen ja valitsee **"Lisää kuitti"** -toiminnon dashboardilta tai pikatoimintopainikkeesta.
2. Kamera avautuu automaattisesti ohjeineen. Näyttöön ilmestyy vihreä rajaus, joka reagoi kuittipinnan reunoihin.
3. Kun kuitti on tunnistettu, sovellus ottaa kuvan automaattisesti (auto-shutter) ja suorittaa paikallisen esikäsittelyn (perspektiivin oikaisu, kontrastin parannus, kohinan poisto).
4. Esikäsitelty kuva lähetetään OCR- ja LLM-agenttiketjuun, joka tuottaa rakenteisen tiedon (päivämäärä, toimittaja, summa, ALV, kululuokka, maksutapa).
5. Käyttäjä saa tiiviin yhteenvedon keltaisella korostetuista epävarmuuksista (esim. puuttuva ALV), joita voi korjata chatin tai valintalistojen avulla.
6. Hyväksytty kuitti kirjautuu automaattisesti oikeaan kirjanpitokategoriaan ja tallentuu turvalliseen dokumenttiarkistoon.

### Käyttöliittymä ja vuorovaikutus
- **Kameranäkymä**: Tumma navy blue -tausta ja valkoiset ohjetekstit, vihreä reunus onnistuneelle tunnistukselle, keltainen jos valaistus/ tarkennus vaatii säätöä.
- **Esikatselukortti**: Valkoisella taustalla esitetty kuitti ja vihreä check-ikoni, kun agentit ovat varmoja tiedoista. Keltaiset pillit osoittavat kenttiä, jotka vaativat vahvistusta.
- **Chatbot-paneeli**: Käyttäjä voi kysyä agentilta "Miksi tämä kuitti meni matkakuluihin?" ja saa selityksen. Paneelin väritys seuraa tumma navy blue + vihreä/valkoinen typografiaa.

### Tekninen toteutus
- **Kameraintegraatio**: React Native Camera / VisionCamera (tai Flutterin `camera` + `google_ml_kit`), joka tukee automaattista rajauksen tunnistusta ja tasapainottaa valotuksen.
- **Kuvankäsittely**: Paikallisesti ajettava OpenCV-pipeline (JSI/NDK-moduulina) suoristaa ja parantaa kuvan ennen OCR:ää.
- **OCR**: Aluksi käytetään Google ML Kitin tekstitunnistusta offline-tilassa; vaihtoehtoisesti voidaan hyödyntää Tesseractiin perustuvaa omaa mallia, jota agentti täydentää LLM:llä.
- **Rakenteistusagentti**: Palvelinpuolella ajettava ketju (esim. LangChain/LangGraph), joka yhdistää OCR-tulokset toimittajarekisteriin, ALV-taulukkoon ja oppii yrityksen kustannuspaikat. LLM tarkistaa summien johdonmukaisuuden ja ehdottaa kategorioita.
- **Validointisäännöt**: PostgreSQL:ssä sijaitsevat yrityskohtaiset tilikartat ja ALV-prosentit. Agentti vertailee niitä OCR-tietoon ja nostaa epävarmuustasot UI:hin.
- **Synkronointi**: Offline-tuki tallentamalla kuvat ja metatiedot laitteelle (encrypted SQLite). Kun yhteys palaa, agentit jatkavat prosessointia ja synkronoivat kirjanpidon.
- **Tilapäinen OCR-stub**: Ensimmäisessä mobiiliversiossa backend tarjoaa `/api/receipts/process` -pisteen, joka tuottaa placeholder-datan base64-kuvasta. Se mahdollistaa sovelluksen virtauslogiikan testaamisen ennen varsinaisen OCR/LLM-ketjun käyttöönottoa.

### Datavirta, arkistointi ja tietomallit
- **Kuvien vastaanotto ja varastointi**: Esiprosessoitu kuva pumpataan ensin `receipt_images`-tauluun, joka tallentaa kuvan URL:n (tai objektivaraston polun), hashin, resoluution, käytetyn esikäsittelypipelinenversion sekä aikaleiman. Samalla kuittien binäärit talletetaan krypattuun objektivarastoon, josta arkistointi- ja auditointipalvelu voi hakea ne myöhemmin.
- **Rakenteinen data**: OCR/LLM-ketjun tuottama JSON kirjataan `receipt_entries`-tietueeksi. Agentti rikastaa sen toimittajan tunnisteella, verokannan koodilla ja luo linkityksen käyttäjän kululaskutukseen (`expense_report_id`).
- **Kirjanpidon integraatio**: `gl_postings` muodostaa automaattisesti debet/kredit-viennit kustannuspaikkoineen ja verokoodeineen sekä liittää oikean kuittiarkiston avaimen. Tietueet siirtyvät agentin orkestroimaan "Kululasku-jonoon", josta ERP/GL-integraatio noutaa ne.
- **XML/CSV-vienti kirjanpitoon**: Jokainen hyväksytty kuitti julkaistaan rivinä valittuun vientiformaattiin (XML tai CSV). Rakenne on vakioitu seuraavilla kentillä järjestyksessä: `summa`, `veron_osuus`, `paivamaara`, `ilmoittaja`, `kuvaus`, `kirjattu_kululaskuun` (Y/N), `maksettu` (Y/N), `maksatuksessa` (Y/N), `kustannuspaikka`, sekä neljä 2 tavun mittaista vapaata kenttää (`vapaa_1`–`vapaa_4`) mahdollisia laajennuksia varten. Agentti varmistaa, että XML:ssä kentät ovat omissa tageissaan ja CSV:ssä erotettuna puolipisteellä.
- **Audit trail**: Jokaisesta viennistä syntyy kirjaus `receipt_audit_log` -tauluun (tapahtumatyyppi, aikaleima, agentti, käyttäjän mahdolliset korjaukset), jotta prosessi on jäljitettävissä.

### Laadunvarmistus ja mittarit
- OCR- ja LLM-putken onnistumisprosentti ilman manuaalisia korjauksia (tavoite ≥ 85 % ensimmäisessä versiossa).
- Käsittelyaika kuvaushetkestä hyväksyttyyn kirjaukseen (tavoite < 15 sekuntia hyvällä yhteydellä).
- Kuittien automaattinen ALV-kenttien täyttöaste.
- Käyttäjätyytyväisyys kamerakokemukseen (in-app rating ≥ 4/5).

### Roadmap seuraaville iteraatioille
1. **Iteraatio 1**: MVP ilman agentin selittäviä chat-vastauksia; painottuu luotettavaan OCR:ään ja manuaaliseen vahvistukseen.
2. **Iteraatio 2**: Agentin selitys- ja keskusteluominaisuudet, toimittajarekisterin automaattinen rikastus.
3. **Iteraatio 3**: Monikielinen tuki, valuuttamuunnokset ja automaattiset kululaskut (matkaliput, kilometrikorvaukset).
4. **Iteraatio 4**: Reaaliaikainen kuittien ja korttitapahtumien yhdistäminen pankkisyötteistä.

## Tekninen arkkitehtuuri
- **Frontend**: React Native tai Flutter, jotta sovellus voidaan julkaista iOS:lle ja Androidille yhdellä koodipohjalla.
- **Agenttipalvelu**: Pilvessä ajettava orkestroija (esim. LangGraph tai custom orchestrator), joka hallitsee tehtävien delegointia eri agenteille.
- **Integraatiokerros**: Node.js / Python -mikropalvelut, jotka muodostavat yhteydet pankki-, laskutus- ja verohallinnon API:hin.
- **Tietokanta**: PostgreSQL taloustietojen rakenteelliseen tallennukseen, lisäksi BigQuery tai vastaava analytiikkaa varten.
- **Turvallisuus**: OAuth2.0/BankID kirjautuminen, end-to-end -salaus arkaluontoisille dokumenteille, audit trail kaikista automaattisista päätöksistä.

### Hallintapaneelin WebUI-pohja

Ensimmäinen web-hallintapaneelin versio rakennetaan Vite + React + TypeScript -pinolla. TailwindCSS huolehtii väripaletin (vihreä, keltainen, valkoinen, tumma navy blue) toistosta komponenteissa. Pohja sisältää seuraavat näkymät ja komponentit:

- **Päänäkymä**: Talouden ohjauspöydän yläosa esittelee agentin ajantasaiset toimenpide-ehdotukset ja tarjoaa navigaation dashboardiin, kuittilistaan ja raportteihin.
- **Kuittien sisäänkirjaus** (`ReceiptIntakePanel`): Lomake, johon voi pudottaa mobiililla kuvatun kuitin, tarkistaa LLM-agentin tunnistamat kentät ja lähettää ne backendin `/api/receipts`-rajapintaan.
- **Viimeisimmät kuitit** (`RecentReceipts`): Mock-datalla täytetty lista, jonka tilalle kytketään backend-haut, kun API on valmis palauttamaan kuitit agentin tilatietoineen.

Pohja toimii itsenäisenä `web/`-hakemistossa ja on suunniteltu samaan autentikointiin kuin mobiili- ja backend-palvelut. Proxy-asetuksella (`vite.config.ts`) voidaan ohjata API-kutsut paikalliseen backend-instanssiin kehityksessä.

### Backend-hosting-suositus
Ensimmäisen vaiheen tarpeisiin suosittelen rakentamaan backendin Google Cloud Platformiin (GCP) seuraavista syistä:

- **Serverless-lähestyminen**: Cloud Run soveltuu hyvin OCR/LLM-agenttiputken mikropalveluille. Se skaalaa automaattisesti kuittikuormien mukaan ja mahdollistaa konttipohjaisen kehitysprosessin ilman jatkuvaa palvelininfraa.
- **Data- ja analytiikkaintegraatio**: Cloud SQL (PostgreSQL) ja BigQuery ovat luonnollisia jatkeita jo määritellylle tietomallille. Niiden välinen integraatio on valmiiksi saumaton, mikä helpottaa raportointia ja koneoppimista.
- **Tiedostojen hallinta**: Cloud Storage tarjoaa versionoidut bucketit kuittiarkistolle ja tukee automaattisia elinkaarisääntöjä sekä KMS-salausta.
- **Identiteetti ja tietoturva**: Identity-Aware Proxy, Secret Manager ja Cloud Audit Logs tekevät helpoksi täyttää talousdatan vaatimukset ilman raskasta ylläpitoa.
- **Laajennettavuus**: Mikäli tarvitaan erikoisempia agenttityönkulkuja, Vertex AI Workbench ja LangChain-ympäristöt ovat saatavilla samasta pilvestä.

Vaihtoehtona AWS tarjoaa vastaavat rakennuspalikat (EKS/Fargate, RDS, S3, Bedrock). Mikäli tiimillä on enemmän AWS-osaamista tai integraatiovaatimuksia paikallisiin pankki-API-ympäristöihin, siirtyminen AWS:ään onnistuu ilman merkittäviä arkkitehtuurimuutoksia. Keskeistä on varmistaa, että valittu pilvi tukee vahvaa tietoturvaa, EU-alueen dataresidenssiä ja agenttipohjaisten palvelujen skaalautuvuutta.

### WordPress-integraatio vai standalone-backend?
- **Nopea aloitus WordPressillä**: Mikäli organisaatiolla on valmiiksi hallittu WordPress-ympäristö (esim. WP VIP, oman palvelimen multisite), voidaan kuittiskannerin hallintapaneeli julkaista pluginina. Plugin integroi käyttäjähallinnan (JWT/OAuth-silta), tarjoaa embedatun React Native Web -hallintanäkymän ja delegoi kuittien prosessoinnin erilliselle API-palvelulle. WordPress soveltuu etenkin sisäiseen käyttöliittymään ja sisällönhallintaan, kun halutaan minimoida aloituskonfigurointi.
- **Stand-alone ilman WordPressiä**: Mikäli WordPressiä ei tarvita, sama backend toteutetaan kevyenä serverless- tai konttialustana (Cloud Run/AWS Fargate). React Native -mobiilisovellus kommunikoi suoraan API gatewayn kautta, ja hallintapaneeli voidaan rakentaa esim. Next.js:llä. Tämä malli vähentää WordPressin ylläpitotarvetta, on turvallisempi kriittiselle talousdatalla ja tarjoaa selkeämmän CICD-putken.
- **Hybridimalli**: WordPress voi toimia vain julkisena sivustona ja markkinointikerroksena, kun taas sovelluksen kirjautunut käyttö kulkee täysin erillisessä backendissä. Tällä tavalla markkinointitiimi hyödyntää tuttua CMS:ää, mutta tekninen ydin pysyy modulaarisena.

Kriteerinä kannattaa käyttää sitä, onko tiimillä WP-osaamista ja tarvetta WordPressin sisällönhallintaan. Mikäli päätavoite on talousdatan turvallinen käsittely ja agenttipohjaiset työnkulut, suosittelen standalone-backendiä; WordPressin voi liittää myöhemmin kevyeen hallintaportaaliin, jos sisällönhallintaa kaivataan.

### Päätös: WordPress ei ole välttämätön

- **Backend rakennetaan ilman WordPressiä.** Ensimmäinen versio toteutetaan kevyenä Node.js/TypeScript-palveluna, joka ajetaan pilvessä (esim. Cloud Run). Näin varmistetaan, että talousdata pysyy erillisessä, auditoinnin kannalta läpinäkyvässä ympäristössä ja että API-rakenne pysyy yksinkertaisena mobiilisovellukselle.
- **WordPress varataan markkinointiin.** Mikäli markkinointitiimi tarvitsee nopean julkaisualustan, WordPress voidaan käynnistää erikseen `www.riku.click` -sivustolle tai landing page -käyttöön. Se ei kuitenkaan ole tekninen riippuvuus backendille.
- **Hallintapaneeli toteutetaan web-komponenttina.** Sisäistä hallintaa varten rakennetaan erillinen React/Next.js-sovellus, joka hyödyntää samaa autentikointia kuin mobiili. Näin vältetään WordPress-plugineihin liittyvä ylläpitokuorma.

Tämä linjaus mahdollistaa etenemisen heti backendin kanssa ilman WordPress-asennusta. Tarvittaessa WordPress voidaan lisätä myöhemmin markkinointi- tai dokumentointitarpeisiin ilman, että ydintekniikka muuttuu.

### Domainin valmistelu: riku.click
Jotta **riku.click** voidaan ottaa käyttöön palvelun julkisena osoitteena, tarvitaan seuraavat valmistelut:

1. **Domainin hankinta ja DNS**
   - Rekisteröi `riku.click` luotettavalta rekisterinpitäjältä (esim. Google Domains siirron jälkeen Squarespace, Namecheap, Gandi).
   - Varmista mahdolliset varadomainit (`rikuhoitaa.com`, `rikupro.click`) typo-suojaksi ja ohjaa ne päädomainiin.
   - Määritä ensivaiheessa seuraavat DNS-tietueet: `A`/`AAAA` backendin julkiseen osoitteeseen, `CNAME` markkinointisivulle (esim. `app.riku.click`, `api.riku.click`), `TXT`-tietueet SPF:ää ja domain-varmistuksia (Google Workspace / Microsoft 365 / SendGrid) varten.

2. **Sertifikaatit ja tietoturva**
   - Automatisoi TLS-varmenteet (Let’s Encrypt, Google Managed SSL) kaikille alidomaineille.
   - Lisää HSTS ja varmista, että backend ja markkinointisivusto pakottavat HTTPS-yhteydet.
   - Dokumentoi sertifikaattien uusimisprosessi ja valvo, että se on osa CI/CD-putkea tai GCP/AWS hallintaa.

3. **Sisällön ja palveluiden julkaisu**
   - Pystytä julkinen laskeutumissivu (Next.js/WordPress-static) esim. Cloud Run + Cloud CDN -kokoonpanoon tai vaihtoehtoisesti Vercel/Netlify-projektiin, joka ohjataan `www.riku.click`- tai `riku.click`-juureen.
   - Reititä `app.riku.click` mobiilisovelluksen backendille (Cloud Run -palvelu, API Gateway). Varmista CORS ja autentikaatio-suojaus.
   - Luo `status.riku.click` -aliverkkotunnus status-sivulle (esim. Better Uptime, Statuspage) sekä `docs.riku.click` kehittäjäportaalille (Stoplight, Mintlify).

4. **Sähköposti ja viestintä**
   - Hanki sähköpostipalvelu (Google Workspace, Fastmail). Luo vähintään `info@riku.click`, `support@riku.click`, `billing@riku.click`.
   - Konfiguroi SPF, DKIM ja DMARC -tietueet viestien toimitettavuuden ja tietoturvan varmistamiseksi.

5. **Analytiikka ja seuranta**
   - Lisää markkinointisivulle analytiikkatyökalut (Plausible, Google Analytics 4) ja määrittele evästekäytännöt.
   - Liitä domain Cloud Loggingiin / Stackdriveriin tai vastaavaan, jotta API- ja sivustotapahtumat voidaan auditoida.

6. **Lailliset ja brändilliset valmistelut**
   - Julkaise `privacy.riku.click` ja `terms.riku.click` -sivut, joissa määritellään tietosuojakäytännöt (GDPR) ja käyttöehdot.
   - Varmista tavaramerkin saatavuus Suomessa/EU:ssa ja aloita tarvittaessa rekisteröinti.
   - Luo brändiohjeisto (logo, värit, typografia) domainia varten ja varmista, että se on yhdenmukainen mobiilisovelluksen kanssa.
   - **Toteuta kirjanpitolain noudattamissuunnitelma**: kirjaa vastuuhenkilö, käy läpi [Kirjanpitolaki 1336/1997 -muistio](compliance/fi_accounting_act_1336_1997.md), toteuta audit trail sekä 6/10 vuoden säilytysaikavaatimukset, ja varmista että sähköinen arkistointi täyttää lain 2 luvun 8 §:n vaatimukset.

7. **DevOps ja hallittavuus**
   - Toteuta IaC-konfiguraatio (Terraform, Pulumi) DNS-, CDN- ja backend-resursseille, jotta `riku.click` voidaan toistaa eri ympäristöihin (staging, prod).
   - Lisää valvonta- ja hälytyssäännöt (uptime, cert expiry, latenssit) sekä incident-prosessi.

Lisätietoja Namecheap-konfiguraatiosta ja sen nykyisestä tilanteesta löytyy muistilapusta [docs/domain/riku_click_dns_review.md](domain/riku_click_dns_review.md), jossa on yksityiskohtainen luettelo muutoksista ennen tuotantoon siirtymistä.

Kun yllä oleva lista on hoidettu, domain voidaan julkaista luotettavasti sekä markkinointi- että sovellusliikenteelle.

## Käyttöliittymän osiot
1. **Dashboard** – KPI-kortit (liikevaihto, kulut, tulos), kassavirran kaavio, veropuskuri. Värikoodaus selkeyttää tilannetta.
2. **Toimeksiannot** – Agentin ehdottamat tehtävät, priorisoituina keltaisella, vihreä kertoo valmiit tehtävät.
3. **Tapahtumat** – Lista pankkitapahtumista, suodatettuna statuksen mukaan. Agentin varmuusaste näkyy väripalkkina.
4. **Raportit** – Tuloslaskelma, tase, ALV-yhteenvedot. PDF-/XBRL-vienti yhdellä painalluksella.
5. **Agenttikeskus** – Näyttää aktiiviset agentit, niiden tavoitteen ja viimeisimmät toimenpiteet. Mahdollisuus chatata agentin kanssa luonnollisella kielellä.

## Käyttöönotto ja seuraavat vaiheet
1. Luodaan interaktiivinen prototyyppi (Figma) määritellyllä väripaletilla.
2. Toteutetaan integraatiot tärkeimpiin talouspalveluihin (pankki, laskutus, verottaja).
3. Rakennetaan agenttiorkestraatio, joka hyödyntää talousdatan tietomalleja.
4. Kehitetään beta-versio valitulle käyttäjäryhmälle ja kerätään palaute.
5. Sertifioidaan sovellus tietoturvastandardien (esim. ISO 27001) mukaisesti ennen julkaisua.

## Mittarit menestyksen arviointiin
- Aktiivisten käyttäjien määrä ja pidätysaste 3/6/12 kuukauden kohdalla.
- Kuinka suuri osuus kirjauksista syntyy täysin automaattisesti.
- Käyttäjien raportoima ajan säästö per kuukausi.
- Agentin antamien toimenpide-ehdotusten toteutusprosentti.
- Asiakastyytyväisyys (NPS) ja tukipyyntöjen määrä.

## Pitkän aikavälin visio
Riku Hoitaa laajentuu henkilökohtaiseen taloudenhallintaan, tarjoaa AI-pohjaisia skenaarioanalyysejä ja integroituu suoraan kirjanpidon ohjelmistoihin. Agentit toimivat yhteistyössä kumppaneiden kanssa (esim. vakuutusyhtiöt, pankit), jotta käyttäjä saa kokonaisvaltaisen taloudellisen näkymän yhdestä paikasta.


## Toteutus: kuittiskannerin ensimmäinen versio
Ensimmäinen tuotantokelpoinen toteutus perustuu React Native -sovellukseen:

- `mobile/app/screens/ReceiptScannerScreen.tsx` vastaa kameran käytöstä, ohjeistaa käyttäjää vihreällä kehysvärillä ja lähettää kuvan prosessointiputkeen, kun auto-shutter laukeaa.
- `mobile/app/services/receiptService.ts` arkistoi otoksen laitteelle, lataa sen objektivarastoon ja kutsuu OCR/LLM-prosessointia, joka palauttaa rakenteisen datan kululaskua varten.
- `mobile/app/storage/receiptStorage.ts` säilyttää sekä kuvien tiedostopolut että välitulokset salatussa sovellustallennuksessa ja mahdollistaa offline-tilassa jatkamisen.
- `mobile/app/types/receipt.ts` määrittää kuittiluonnosten tyypit ja varmistaa, että pakolliset kentät voidaan validoida käyttöliittymässä.

Näiden avulla saadaan toteutettua vaatimus: kuitti kuvataan, kuva arkistoidaan ja data tallennetaan automaattisesti jatkokäsittelyyn.

## Toteutettu backend-runko
Ensimmäisen vaiheen päätöksenä rakennettiin oma Node.js/TypeScript-pohjainen backend, joka tarjoaa REST-rajapinnan kuittien talletukseen
ja hallintaan. Palvelu tallentaa kuittikuvat tiedostojärjestelmään, ylläpitää metadatan JSON-muodossa ja validoi pyyntöjen kentät
(Zod). Ratkaisu vastaa päätökseen toteuttaa koko backend ilman WordPress-riippuvuutta ja toimii pohjana jatkokehitykselle, jossa
metadatan voi siirtää PostgreSQL:ään ja kuvat objektivarastoon.
