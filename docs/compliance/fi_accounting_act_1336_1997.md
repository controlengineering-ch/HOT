# Kirjanpitolaki 1336/1997 – keskeiset velvoitteet Riku Hoitaa -palvelulle

Tämä muistio kokoaa suomalaisen kirjanpitolain (1336/1997, muutoksineen) keskeiset vaatimukset, jotka Riku Hoitaa -mobiilisovelluksen ja sen backend-palvelun on huomioitava. Laki sisältää useita muutoksia (mm. 1620/2015, 1376/2022); muista tarkistaa ajantasainen Finlex-versio ennen tuotantoon menoa. **Viimeisin tarkistettu versio (19.12.2024) löytyy osoitteesta:** <https://www.finlex.fi/eli?uri=http://data.finlex.fi/eli/sd/1997/1336/ajantasa/2024-12-19/fin>.

## 1. Kirjanpitovelvollisuus ja yleiset periaatteet (Luku 1 & 2)
- **Kirjanpitovelvollisuus** koskee kaikkea liiketoimintaa harjoittavaa yhteisöä ja luonnollista henkilöä. Palvelun on mahdollistettava kirjanpitovelvollisen oman tilikartan ja tilikauden asetukset.
- **Oikea ja riittävä kuva** (2:3 §): Kirjanpidon tulee antaa todenmukainen kuva toiminnan tuloksesta ja taloudellisesta asemasta. Agenttien automaattiset kirjaukset on auditoitava ja dokumentoitava.
- **Jatkuvuus ja johdonmukaisuus** (2:4 §): Käytettävät kirjausmenetelmät sekä tilikartta on dokumentoitava. Muutokset (esim. AI-luokkien päivitykset) tulee arkistoida ja versioida.
- **Maksuperusteinen kirjanpito pienille toimijoille** (2:4a §): Tarjoa tuki sekä suorite- että maksuperusteiseen kirjanpitoon.

## 2. Tositteet ja niiden sisältö (Luku 2, 2:5–2:7)
- Jokaisesta liiketapahtumasta on oltava **tosite**, joka sisältää vähintään:
  - päivämäärän, vastapuolen, tapahtuman sisällön ja rahamäärän,
  - verokannan ja veron osuuden, kun sovellettavissa,
  - merkinnän siitä, miten tapahtuma on kirjattu pääkirjaan ja reskontraan.
- **Kuittiskannerin** on liitettävä jokainen kuva yksilölliseen tosite-ID:hen ja varmistettava, että OCR/LLM-tuotettu data sisältää nämä kentät. Käyttäjän tekemät korjaukset on lokitettava.
- **Muuntaminen sähköiseen muotoon** on sallittua, kunhan tiedon luettavuus säilyy. Sähköisen tositteen tulee olla muuttumaton ja siitä on pystyttävä tuottamaan selväkielinen kopio.

## 3. Kirjanpitoaineiston säilytys (Luku 2, 2:8 §)
- **Pääkirjat, päiväkirjat, tililuettelo, tilinpäätös ja liitetiedot** on säilytettävä vähintään **10 vuotta** tilikauden päättymisestä.
- **Tositteet, kirjanpitoa täydentävä kirjeenvaihto ja muu aineisto** on säilytettävä vähintään **6 vuotta** sen vuoden lopusta, jonka aikana tilikausi on päättynyt.
- Sähköinen säilytys on sallittu, kunhan aineisto voidaan tuottaa **selväkielisenä Suomessa ilman aiheetonta viivettä**. Palvelun on tarjottava vienti vähintään PDF/XML/CSV-muodoissa ja varmistettava, että arkistot ovat EU/ETA-alueella.
- Toteuta **varmuuskopiointi ja toipumissuunnitelma** (DRP), jotta aineiston eheys turvataan koko säilytysajan.

## 4. Kirjausketju ja jäljitettävyys (Audit trail, 2:6 § & 2:7 §)
- Kirjausten on muodostettava **katkeamaton kirjausketju** päiväkirjasta pääkirjaan ja edelleen tilinpäätökseen.
- Kaikista automaattisista päätöksistä on säilytettävä **tapahtumaloki**, josta käy ilmi: tapahtuma, aikaleima, vastuullinen käyttäjä/agentti sekä muutokset.
- Varmista, että järjestelmä tukee **tilikausien lukitusta** ja muutoslokia jälkikäteen tehtäville korjauksille (korjausmerkintä, ei alkuperäisen poistamista).

## 5. Tilinpäätös ja liitetiedot (Luku 3)
- Palvelun on mahdollistettava tilinpäätösaineiston tuottaminen lain edellyttämässä muodossa (tuloslaskelma, tase, liitetiedot). Pienyrityksille tarjoa valmiit mallipohjat lain liitteiden mukaisesti.
- Huomioi **tilinpäätöksen laatimisaika**: viimeistään neljän kuukauden kuluessa tilikauden päättymisestä (3:6 §).

## 6. Sähköinen arkistointi ja tietoturva
- Toteuta vahva **tietojen eheys- ja käytettävyyskontrolli** (esim. hashit, allekirjoitukset). Kaikki muutokset on voitava jäljittää alkuperäiseen tositteeseen.
- Käytä **sähköisen arkistoinnin dokumentoitua prosessia**: missä data sijaitsee, miten sitä varmuuskopioidaan, kuinka pääsyä hallitaan (roolit, käyttöoikeudet, lokit).
- GDPR ja muu tietosuojalainsäädäntö täydentävät vaatimuksia: käsittele henkilötietoja lainmukaisesti (oikeusperuste, minimointi, tietoturva).

## 7. Toimintasuunnitelma Riku Hoitaa -tiimille
1. **Lue ajantasainen Finlex-teksti** ja kirjaa tiimille vastuuhenkilö (legal/compliance). Päivitä tämä muistio tarvittaessa.
2. **Laadi kirjanpitoaineiston hallintapolitiikka**, joka kattaa säilytysajat, formaatit, varmuuskopioinnin ja käyttöoikeudet.
3. **Implementoi audit trail** backendissä: versioitu kuitti, OCR-data, käyttäjän muokkaukset ja kirjanpitovienti.
4. **Toteuta export-rajapinnat**: XML/CSV, jotka sisältävät vaaditut kentät sekä viittauksen alkuperäiseen tositekuvaan.
5. **Suunnittele tilinpäätösraportointi** ja varmista, että sovellus tuottaa lakisääteiset raportit vähintään suomeksi.
6. **Varmista tietoturva** (TLS, salaus levossa, pääsynhallinta) sekä mahdollisten kolmansien osapuolten sopimukset (DPA:t).

Kun yllä olevat vaatimukset on täytetty ja dokumentoitu, palvelu voidaan arvioida kirjanpitolain vaatimusten mukaiseksi ennen tuotantokäyttöönottoa.
