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

## Tekninen arkkitehtuuri
- **Frontend**: React Native tai Flutter, jotta sovellus voidaan julkaista iOS:lle ja Androidille yhdellä koodipohjalla.
- **Agenttipalvelu**: Pilvessä ajettava orkestroija (esim. LangGraph tai custom orchestrator), joka hallitsee tehtävien delegointia eri agenteille.
- **Integraatiokerros**: Node.js / Python -mikropalvelut, jotka muodostavat yhteydet pankki-, laskutus- ja verohallinnon API:hin.
- **Tietokanta**: PostgreSQL taloustietojen rakenteelliseen tallennukseen, lisäksi BigQuery tai vastaava analytiikkaa varten.
- **Turvallisuus**: OAuth2.0/BankID kirjautuminen, end-to-end -salaus arkaluontoisille dokumenteille, audit trail kaikista automaattisista päätöksistä.

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

