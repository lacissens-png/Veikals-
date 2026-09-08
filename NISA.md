# Nišas meklējums — pielāgots ierobežojumiem

Sagatavots: 2026-09-08.
Ierobežojumi: **kapitāls ≤ €1000 · prasmes: programmēšana · laiks: dažas stundas
nedēļā blakus darbam.**

Šie trīs punkti izslēdz gandrīz visu, kas atrodams "top nišu" sarakstos. Nav
kapitāla inventāram, nav laika klientu zvaniem darba laikā. Paliek viena forma:
**produkts, ko pārdod pats, kamēr tu guli.**

---

## 1. LĒMUMS

> **Shopify aplikācija, kas kārto ES muitas Product Identifier (PID) datus.**

No **2026. gada 1. novembra** ES muita pieprasa PID katrā B2C sūtījumā uz ES —
**neatkarīgi no vērtības**. Kopš 1. jūlija to pieņem brīvprātīgi; no novembra tas
ir obligāti, un muita drīkst deklarācijas ar trūkstošu vai kļūdainu PID **noraidīt
vai izmeklēt**.

Prasīti ir divi:
1. **Tirgotāja produkta identifikators** — pārdevēja SKU. To tirgotājs zina.
2. **Ražotāja nestandartizētais produkta identifikators** — *ražotāja* preces kods.
   **To vairums tirgotāju sistēmā vispār neglabā.**

Otrais punkts ir viss bizness. Tā nav klasifikācija — tā ir datu problēma:
savākt, uzglabāt pie katra varianta un padot tālāk uz muitas deklarāciju.

### Kāpēc tas der tieši taviem ierobežojumiem

| Ierobežojums | Kā risinās |
|---|---|
| €1000 kapitāls | Shopify izstrādātāja konts bez maksas, hostings dažus eiro mēnesī |
| Dažas stundas nedēļā | Neliela aplikācija ap vienu datu lauku, ne platforma |
| Nav laika pārdošanai | **Shopify App Store meklēšana pārdod tavā vietā** — nav zvanu, nav demo |
| Programmēšana | Tieši tā prasme, kas vajadzīga; nekas cits nav vajadzīgs |
| Ieņēmumi | Abonements. Uzbūvē vienreiz, pelna atkārtoti |

**Distribūcija ir īstais iemesls, kāpēc šī uzvar.** Cilvēkam ar dažām stundām
nedēļā grūtākais nav uzbūvēt — grūtākais ir dabūt klientus. Aplikāciju veikals
šo problēmu jau ir atrisinājis: pircēji tur meklē paši, un karte jau ir pievienota.

---

## 2. Konkurence — pārbaudīju, un tā maina plānu

Shopify App Store **jau ir** HS kodu aplikācijas: *Tariff HS Code Compliance* un
*DutyCode* (bulk klasifikācija, CN8/UK/HTS, ticamības vērtējums).

**Tāpēc HS kodu klasifikāciju netaisi. Tā vieta ir aizņemta.**

Bet PID ir cita problēma. HS kods pasaka, *kāda veida* prece šķērso robežu; PID
pasaka, *tieši kura* — līdz SKU, ražotājam un svītrkodam. Klasifikācija pret datu
savākšanu. Šobrīd tur ir sprauga.

### Divi riski, kas var to nogalināt

1. **Esošie spēlētāji paplašinās uz PID.** Tas ir acīmredzamākais viņu nākamais solis.
2. **Shopify to iebūvē pats.** Tie jau pievienoja €3 ES muitas nodevas atbalstu —
   pierādījums, ka viņi šādas lietas absorbē.

Tas nav teorētiski. Tas ir ticamākais iznākums 12–24 mēnešu laikā.

**Ko ar to darīt:** neplāno mūžīgu biznesu. Plāno **2–3 gadu logu** un pieņem, ka
prasme ir vērtīgāka par produktu. Nākamais tāds pats vilnis jau redzams —
Digitālā produkta pase tekstilprecēm no 2027. Tas pats modelis, jauna regula.

---

## 3. Svarīgākā atziņa par termiņu

**Tev nav jāpaspēj līdz 1. novembrim.** Šķiet pretintuitīvi, bet:

Līdz novembrim tirgotāji par PID nedomā. **Pēc** novembra viņiem sāk atgriezties
noraidītas deklarācijas — un tad viņi sāk meklēt risinājumu. Meklējumu pīķis ir
*pēc* termiņa, ne pirms.

Ar dažām stundām nedēļā tu 8 nedēļās neuztaisīsi neko. Bet līdz janvārim —
uztaisīsi, un tieši tad pieprasījums būs augstākajā punktā.

---

## 4. Ko pārbaudīju un noraidīju

| Ideja | Kāpēc krita |
|---|---|
| **Latvijas e-rēķinu rīks** | Termiņš **pārcelts no 2026. uz 2028. gada 1. janvāri.** Steidzamība pazuda, un līdz tam Horizon/Jumis to iebūvēs. ES ViDA prasība nāk tikai 2030 |
| **HS kodu klasifikators** | Vieta aizņemta — DutyCode un Tariff Code Compliance |
| **EUDR konsultācijas** | Laba niša, bet tas ir *pakalpojums* — prasa pilnu slodzi un zvanus darba laikā |
| **Palīglīdzekļu noma** | Vajag €9000 inventārā un mikroautobusu |
| **Pirts preces eksportam** | Vajag inventāru, noliktavu un sezonalitāti |
| **NIS2 kiberdrošība** | Nauda liela (€50–200k projekti), bet vajag reālu ekspertīzi |
| **AI datu centru enerģētika** | Lielākais vilnis pasaulē. Vajag simtus miljonu |

---

## 5. Nākamie soļi

1. **Izlasi ES muitas PID specifikāciju** — tieši to, ne blogus. Kādi lauki, kāds
   formāts, kā tie nonāk deklarācijā. Viens vakars.
2. **Uzinstalē DutyCode un Tariff Code Compliance.** Saproti, ko tie dara un ko ne.
   Otrs vakars.
3. **Izlasi to sliktās atsauksmes App Store.** Tur būs uzrakstīts, kā trūkst.
4. **Uztaisi mazāko iespējamo versiju:** ražotāja koda lauks pie varianta, bulk
   imports no CSV, eksports muitas formātā. Nekā vairāk.
5. **Publicē janvārī**, kad tirgotājiem sāk atgriezties noraidītās deklarācijas.

---

## 6. Piezīme par šo repozitoriju

Šī niša nav e-veikals. `Index.html` un `Style.css` šim mērķim nav vajadzīgi —
Shopify aplikācijai vajag pavisam citu projektu.

Ja veikalu tomēr turpina, divas esošās kļūdas:
- `Index.html:22` ielādē `script.js`, kura repozitorijā nav
- `Index.html:6` norāda `style.css`, bet fails ir `Style.css` — uz GitHub Pages
  stils neielādēsies

---

## Avoti

- PID prasība no 01.11.2026: https://www.royaleinternational.com/2026/09/eu-customs-mandatory-product-identifiers-nov-2026/
- PID datu sagatavošana: https://www.ukpworldwide.com/2026/08/21/eu-customs-changes-again-why-product-data-needs-to-be-ready-for-1-november-2026/
- ES €3 muitas nodeva: https://trade.ec.europa.eu/access-to-markets/en/news/eu-applies-eu3-customs-duty-item-low-value-e-commerce-consignments
- Esošā konkurence: https://apps.shopify.com/dutycode · https://apps.shopify.com/tariff-code-compliance
- Shopify €3 nodevas atbalsts: https://powercommerce.com/blogs/shopify-updates/shopify-and-the-eu-s-3-per-tariff-line-duty-what-merchants-need-to-know-before-july-1-2026
- Latvijas e-rēķinu atlikšana uz 2028: https://lvportals.lv/norises/376664-e-rekinu-sistemas-ieviesanu-uznemumiem-atliks-lidz-2028-gadam-2025
- ES ViDA grafiks: https://edicomgroup.com/blog/vida-the-european-union-promotes-b2b-electronic-invoicing
