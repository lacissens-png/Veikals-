# Abonementu / rēķinu audita app — MVP

Mobilā aplikācija, kas savieno lietotāja bankas kontu, ar AI atrod atkārtotos
maksājumus (abonementus, regulāros rēķinus), parāda skaidru pārskatu un
sagatavo atcelšanas vai pārrunu vēstules melnrakstu.

**Melnraksti netiek sūtīti automātiski** — lietotājs tos nokopē un nosūta pats.
Bankas piekļuve ir **tikai lasīšanas** režīmā.

Mērķauditorija: Nīderlandes / Eiropas patērētāji.
MVP mērķis: pārbaudīt, vai AI spēj precīzi atpazīt abonementus no bankas
darījumu vēstures un vai lietotāji uzskata šo informāciju par vērtīgu.

---

## Saturs

| Mape | Kas tur ir |
|---|---|
| `backend/` | Node.js + Express 5 + TypeScript API, Prisma 7, PostgreSQL |
| `mobile/` | React Native (Expo SDK 57) aplikācija ar 8 ekrāniem |
| `Index.html`, `Style.css` | Nesaistīts agrāks online veikala demo — MVP to neizmanto |

---

## Priekšnosacījumi

- **Node.js 20 vai jaunāks** — visiem ceļiem
- **Expo Go telefonā** — vajag SDK 57 versiju, sk. "Pirms sākt" zemāk
- **PostgreSQL 14+** — tikai tad, ja backend palaid uz sava datora (A ceļš)
- **Anthropic API atslēga** — [console.anthropic.com](https://console.anthropic.com);
  bez tās abonementu analīze atgriež 503
- **Enable Banking konts** — **nav obligāts**, sk. mock režīmu zemāk

---

## Kā dabūt lietotni telefonā

Trīs ceļi. Galvenā atšķirība ir tā, **kas paliek atkarīgs no datora**.

| Ceļš | Kas vajadzīgs | Ko dod | Dators |
|---|---|---|---|
| **A. Viss uz sava datora** | Docker vai PostgreSQL, viena komanda | Lietotne telefonā, strādā bez interneta | Ieslēgts, tajā pašā Wi-Fi |
| **B. Expo Go + backend internetā** | Render deploy, viena komanda | Tas pats, bet dati nāk no interneta | Tāpat |
| **C. APK (EAS Build)** | Expo konts, backend uz Render | Ikona sākuma ekrānā | **Nevajag** |

Ar `ENABLE_BANKING_MOCK=true` visa plūsma strādā ar reālistiskiem testa
darījumiem, tāpēc projektu var palaist pirmajā dienā, negaidot PSD2 līgumu.

### Pirms sākt: pareizā Expo Go versija

Projekts ir uz **Expo SDK 57**. Expo Go, kas nāk no Play Store, mēdz atpalikt no
jaunākā SDK, un tad, skenējot QR kodu, parādās:

```
Project is incompatible with this version of Expo Go
```

Tas nozīmē tieši to, ko raksta — nesakrīt versijas, nevis kaut kas ir salūzis.
Risinājums ir viena lejupielāde telefonā:

**[expo.dev/go?sdkVersion=57&platform=android&device=true](https://expo.dev/go?sdkVersion=57&platform=android&device=true)**

Tas ir gatavs APK, nevis būvējums, tāpēc nevajag ne Expo kontu, ne gaidīšanu
rindā. Ja telefons pieslēgts ar USB vai lieto emulatoru, `npx expo start` un tad
taustiņš `a` uzstāda pareizo versiju pats.

> **EAS Update šeit nepalīdz.** `expo publish` tika noņemts SDK 50, un ar
> `eas update` publicētu atjauninājumu Expo Go ielādēt nevar — tam vajag
> `expo-dev-client` būvējumu. Tāpēc ceļa "tikai telefons, bez datora" ar Expo Go
> **nav**. Bez datora strādā tikai C ceļš (APK).

### A ceļš: viss uz sava datora

Viena komanda. Skripts sagatavo datubāzi, palaiž backend un Expo, un Ctrl+C
apstādina abus.

```bash
./scripts/start.sh
```

Tad skenē QR kodu ar Expo Go. **Sava IP meklēt nevajag.** Lietotne paņem datora
adresi no Expo izstrādes servera (`Constants.expoConfig.hostUri`) un pati atrod
backend uz porta 4000.

| Karogs | Ko dara |
|---|---|
| `--web` | Atver arī pārlūkā, apskatei bez telefona |
| `--remote <adrese>` | Izlaiž lokālo datubāzi un backend, sk. B ceļu |
| `--skip-setup` | Neizpilda sagatavošanu vēlreiz |

Backend izvade iet uz `backend/dev.log`, lai tā neaizsegtu QR kodu.

> Bez `ANTHROPIC_API_KEY` skripts brīdina uzreiz: darījumi ielādēsies, bet
> analīze atgriezīs 503 un pārskats paliks tukšs.

### B ceļš: Expo Go + backend internetā

Datubāze un backend darbojas uz Render, tāpēc uz datora nevajag ne PostgreSQL,
ne Docker.

1. Izvieto backend — sk. sadaļu **Backend uz Render** zemāk
2. Uz datora:

```bash
./scripts/start.sh --remote https://tavs-serviss.onrender.com
```

3. Skenē QR kodu ar Expo Go

Telefonam un datoram jābūt vienā Wi-Fi tīklā: dators apkalpo JS bundli, bet dati
nāk no interneta.

### Soļi pa vienam

Ja `start.sh` neder vai gribi saprast, kas notiek. Adresi var arī uzspiest:
`EXPO_PUBLIC_API_URL=http://192.168.1.10:4000 npm start`.

#### 1. Datubāze

```bash
# Ar Docker
docker run -d --name abonementi-db \
  -e POSTGRES_PASSWORD=postgres \
  -e POSTGRES_DB=abonementi \
  -p 5432:5432 postgres:16

# Vai izmanto jau esošu PostgreSQL un tikai izveido datubāzi:
createdb abonementi
```

#### 2. Backend

```bash
cd backend
cp .env.example .env
```

Atver `.env` un aizpildi vismaz:

```
DATABASE_URL=postgresql://postgres:postgres@localhost:5432/abonementi?schema=public
JWT_SECRET=<vismaz 32 rakstzīmes — ģenerē ar: openssl rand -hex 32>
ANTHROPIC_API_KEY=sk-ant-...
ENABLE_BANKING_MOCK=true
```

Tikai tagad instalē atkarības un palaid serveri:

```bash
npm install                 # postinstall palaiž `prisma generate`
npx prisma migrate deploy   # izveido tabulas
npm run dev                 # http://localhost:4000
```

> **Secība ir svarīga.** `npm install` palaiž `prisma generate`, kas ielādē
> `prisma.config.ts`, un tas pieprasa `DATABASE_URL`. Ja `.env` vēl nav
> izveidots, instalēšana apstājas ar `PrismaConfigEnvError`. Vari arī padot
> mainīgo tieši: `DATABASE_URL=... npm install`.

Pārbaude: `curl http://localhost:4000/health`

#### 3. Mobilā aplikācija

```bash
cd mobile
npm install
npm start
```

Skenē QR kodu ar **Expo Go** vai palaid emulatorā (`a` — Android, `i` — iOS).

Ātrai apskatei bez telefona der arī pārlūks:

```bash
npm run web
```

> **Pārlūkā sesija nesaglabājas.** `expo-secure-store` ir tikai vietējām
> platformām, tāpēc pēc lapas pārlādes jāpiesakās no jauna. Uz telefona tas
> strādā normāli. Web režīms ir domāts UI apskatei, nevis lietošanai.

### Plūsma aplikācijā

1. **Sākt** → izveido kontu ar e-pastu un paroli
2. **Savienot kontu** → mock režīmā "banka" uzreiz atgriež atpakaļ
3. Darījumi tiek ielādēti, tad Claude tos analizē
4. Pārskatā redzami atrastie abonementi; atver kādu no tiem un nospied
   **Atcelt abonementu**, lai saņemtu vēstules melnrakstu

Testa dati ietver Ziggo, Essent, Netflix (ar cenas kāpumu 13.99 → 15.49 €),
Spotify, Basic-Fit, Zilveren Kruis un Vodafone, kā arī troksni (veikali, alga,
pārskaitījumi starp saviem kontiem), ko AI **nedrīkst** atzīmēt kā abonementu.

---

## Vides mainīgie (`backend/.env`)

| Mainīgais | Obligāts | Apraksts |
|---|---|---|
| `DATABASE_URL` | jā | PostgreSQL savienojums |
| `JWT_SECRET` | jā | Vismaz 32 rakstzīmes; īsāks aptur startu |
| `ANTHROPIC_API_KEY` | AI funkcijām | Bez tās analīze un melnraksti atgriež 503 |
| `ANTHROPIC_MODEL` | nē | Noklusējums `claude-opus-5` |
| `ENABLE_BANKING_MOCK` | nē | `true` = testa dati bez īsta bankas savienojuma |
| `TOKEN_ENCRYPTION_KEY` | jā | 64 hex rakstzīmes; bankas un Gmail tokenu šifrēšanai. `.env.example` satur izstrādes atslēgu, ko produkcijā noraida |
| `GMAIL_MOCK` | nē | `true` = testa vēstules bez Google Cloud projekta |
| `GMAIL_CLIENT_ID` / `_SECRET` | īstam Gmail | Google OAuth klients |
| `GMAIL_REDIRECT_URI` | nē | Kur Google atgriež lietotāju |
| `EMAIL_SYNC_MONTHS` | nē | Cik mēnešus atpakaļ lasīt vēstules, noklusējums `3` |
| `ENABLE_BANKING_CLIENT_ID` / `_SECRET` | ražošanai | Enable Banking atslēgas |
| `ENABLE_BANKING_API_URL` | nē | Noklusējums `https://api.enablebanking.com` |
| `ENABLE_BANKING_REDIRECT_URI` | nē | Kur banka atgriež lietotāju |
| `APP_REDIRECT_URL` | nē | Deep link uz aplikāciju, noklusējums `abonementi://bank-callback` |
| `TRANSACTION_SYNC_MONTHS` | nē | Cik mēnešus ielādēt, noklusējums `3` |
| `PORT`, `NODE_ENV`, `CORS_ORIGIN`, `JWT_EXPIRES_IN` | nē | Standarta iestatījumi |

Konfigurācija tiek validēta ar zod aplikācijas startā — trūkstošs vai nederīgs
mainīgais dod skaidru kļūdu uzreiz, nevis `undefined` pusceļā.

---

## E-pasts (Gmail)

Bankas izraksts rāda tikai to, kas jau ir noticis. E-pastā ir tas, kas vēl
notiks: bezmaksas izmēģinājumi, kas drīz sāks maksāt, cenu paziņojumi pirms
norēķina, un gada atjaunošanas. Tur ir arī viltus rēķini.

**Atļaujas:** `gmail.readonly` un `gmail.send`.

`gmail.modify` apzināti **netiek** prasīts. Lietotne brīdina par krāpšanu, bet
nekad neaiztiek pastkastīti. Nepareizi mēstulēs aizmests īsts rēķins nozīmē
nokavētu maksājumu un soda naudu — tas ir sliktāk nekā brīdinājums, ko lietotājs
redz un izvērtē pats.

**Ko lietotne dara ar datiem:**

- lasa tikai vēstules, kas atbilst šauram meklēšanas vaicājumam (abonementu un
  rēķinu termini holandiešu un angļu valodā), nevis visu pastkastīti
- apgriež ķermeni līdz 2000 rakstzīmēm pirms sūtīšanas uz AI
- datubāzē glabā **tikai izvilktos faktus** — tirgotāju, summu, datumu,
  aizdomīguma pazīmes. Vēstuļu saturs netiek glabāts.

Mock režīmā (`GMAIL_MOCK=true`) visu var izmēģināt bez Google Cloud projekta.
Testa vēstules ietver izmēģinājumu, kas beidzas, Ziggo cenas paziņojumu, gada
atjaunošanu, un trīs krāpšanas paraugus: viltus ING rēķinu, Netflix pīķšķerēšanu
un abonementu slazdu.

Īstam Gmail: izveido Google Cloud projektu, ieslēdz Gmail API, izveido OAuth
klientu ar abām atļaujām, iekopē `GMAIL_CLIENT_ID` un `GMAIL_CLIENT_SECRET`
`.env` failā un iestati `GMAIL_MOCK=false`.

---

## Enable Banking (īstie bankas dati)

1. Reģistrējies [enablebanking.com](https://enablebanking.com/) un izveido aplikāciju
2. Sāc ar **Restricted Production** režīmu — tas ļauj strādāt ar savu paša kontu
3. Iekopē atslēgas `.env` un iestati `ENABLE_BANKING_MOCK=false`
4. `ENABLE_BANKING_REDIRECT_URI` jāsakrīt ar to, kas reģistrēts Enable Banking pusē
5. Pilnai produkcijai nepieciešams līgums un KYB process

> **Pārbaudi pirms produkcijas.** Enable Banking īstā autorizācija paraksta
> pieprasījumus ar RS256 JWT, kas veidots no aplikācijas ID un privātās
> atslēgas, nevis ar client_id/client_secret pāri. Kods ir strukturēts tā, ka
> jāmaina viena funkcija — `buildAuthHeader()` failā
> `backend/src/services/bank/enableBanking.service.ts`. Arī atbilžu lauku
> nosaukumi jāsalīdzina ar [viņu dokumentāciju](https://enablebanking.com/docs/api/).

---

## API endpointi

Visi, izņemot `/health`, `/api/auth/*` un `/api/bank/callback`, prasa
`Authorization: Bearer <token>`.

| Metode | Ceļš | Apraksts |
|---|---|---|
| `GET` | `/health` | Servisa statuss |
| `POST` | `/api/auth/register` | Reģistrācija (e-pasts + parole ≥ 8) |
| `POST` | `/api/auth/login` | Pieteikšanās |
| `GET` | `/api/auth/me` | Pašreizējais lietotājs |
| `POST` | `/api/bank/connect` | Sāk Enable Banking OAuth plūsmu |
| `GET` | `/api/bank/callback` | Bankas atgriešanās punkts → deep link uz app |
| `GET` | `/api/bank/connections` | Savienojumu saraksts |
| `DELETE` | `/api/bank/connections/:id` | Atvieno banku (dzēš arī darījumus) |
| `POST` | `/api/transactions/sync` | Ielādē darījumus no bankas |
| `GET` | `/api/transactions` | Darījumu saraksts |
| `POST` | `/api/subscriptions/analyze` | AI analīze → saglabā abonementus |
| `GET` | `/api/subscriptions` | Saraksts + kopsavilkums pa kategorijām |
| `GET` | `/api/subscriptions/:id` | Detaļas + maksājumu vēsture |
| `PATCH` | `/api/subscriptions/:id` | "Atzīmēt kā nevēlamu" |
| `POST` | `/api/subscriptions/:id/draft-cancel` | Atcelšanas melnraksts |
| `POST` | `/api/subscriptions/:id/draft-negotiate` | Pārrunu melnraksts |
| `PATCH` | `/api/subscriptions/drafts/:draftId` | Atzīmē melnrakstu kā nokopētu/atmestu |
| `POST` | `/api/subscriptions/drafts/:draftId/send` | Nosūta melnrakstu no lietotāja pasta |
| `POST` | `/api/email/connect` | Sāk Gmail OAuth plūsmu |
| `GET` | `/api/email/callback` | Google atgriešanās punkts |
| `GET` | `/api/email/connections` | E-pasta savienojumi |
| `DELETE` | `/api/email/connections/:id` | Atvieno pastu |
| `POST` | `/api/email/sync` | Lasa vēstules → AI analīze → atradumi |
| `GET` | `/api/email/findings` | Brīdinājumi un gaidāmie maksājumi |
| `PATCH` | `/api/email/findings/:id` | Atzīmē kā redzētu vai atmestu |
| `POST` | `/api/notifications/token` | Expo push tokena reģistrācija |
| `DELETE` | `/api/me` | Dzēš kontu un visus datus |

Kļūdas vienmēr atgriežas formā:

```json
{ "error": { "code": "not_found", "message": "Abonements nav atrasts" } }
```

---

## Datubāzes shēma

Shēma ir `backend/prisma/schema.prisma`, migrācijas —
`backend/prisma/migrations/`. Tabulas atbilst specifikācijai: `users`,
`bank_connections`, `transactions`, `subscriptions`, `draft_actions`.

Salīdzinājumā ar specifikācijas "vienkāršoto" SQL pievienoti šādi lauki, bez
kuriem prasītās funkcijas nav iespējamas:

| Tabula | Lauks | Kāpēc |
|---|---|---|
| `users` | `password_hash` | E-pasts + parole autentifikācijai (bcrypt) |
| `users` | `push_token` | Push paziņojumiem par cenu izmaiņām |
| `bank_connections` | `auth_state` | Vienreiz lietojams CSRF `state` OAuth callback |
| `bank_connections` | `session_id`, `access_token`, `token_expires_at` | Bez tokena nevar lasīt darījumus |
| `bank_connections` | `last_synced_at` | Rāda pēdējo sinhronizāciju iestatījumos |
| `transactions` | `external_id` (+ unique) | Atkārtota sinhronizācija nedublē darījumus |
| `subscriptions` | `occurrences`, `first_amount`, `last_amount` | Detaļu ekrāns un cenu izmaiņas |
| `subscriptions` | unique (`user_id`, `merchant_name`) | Atkārtota analīze atjauno, nevis dublē |

> **Prisma 7 piezīme:** kopš 7. versijas datubāzes URL vairs netiek norādīts
> `schema.prisma` failā — migrācijas to lasa no `prisma.config.ts`, bet klients
> izmanto `@prisma/adapter-pg` draiveri (`backend/src/lib/prisma.ts`).

Noderīgas komandas:

```bash
cd backend
npx prisma migrate dev --name <nosaukums>   # jauna migrācija izstrādē
npx prisma migrate deploy                   # migrācijas produkcijā
npx prisma studio                           # datu pārlūks pārlūkprogrammā
```

---

## AI integrācija

Modelis: **`claude-opus-5`** ar adaptīvo domāšanu.

- **Kategorizēšana** — sistēmas prompts ir specifikācijā norādītais. Atbilde
  iet caur `messages.parse()` + `zodOutputFormat()`, tāpēc JSON shēmu uzspiež
  API līmenī un nav jāpaļaujas uz instrukciju "atgriez tikai JSON".
- **Melnraksti** — atsevišķi prompti atcelšanai un cenas pārrunāšanai;
  vēstule tiek rakstīta holandiešu vai angļu valodā atkarībā no pakalpojuma
  sniedzēja.

**Privātums:** uz Claude API tiek sūtīts tikai `date`, `description` un
`amount`. Lietotāja e-pasts, ID, konta numurs un jebkuri citi personas dati
nekad neatstāj mūsu serveri. Kods: `backend/src/services/claude.service.ts`.

Ja `ANTHROPIC_API_KEY` nav iestatīta, AI endpointi atgriež `503` ar skaidru
paskaidrojumu — pārējā aplikācija turpina strādāt.

---

## Push paziņojumi

Paziņojums tiek sūtīts, kad analīze **pirmo reizi** konstatē konkrēta
abonementa cenas izmaiņu (atkārtota analīze to nedublē).

Lai tie strādātu, nepieciešama fiziska ierīce un EAS projekts:

```bash
cd mobile
npx eas init          # ieraksta projectId app.json failā
```

Bez tā aplikācija strādā normāli, tikai bez push paziņojumiem.

---

## CI

`.github/workflows/ci.yml` palaižas katrā pull request un push uz `main`.
Divi paralēli darbi:

| Darbs | Ko pārbauda |
|---|---|
| `backend` | `npm ci` → tipu pārbaude → testi → būvējums → `prisma migrate deploy` pret tīru PostgreSQL 16 → dūmu tests |
| `mobile` | `npm ci` → tipu pārbaude |

**Testi** (`node:test`, bez papildu atkarībām) sedz tīro loģiku:

```bash
cd backend
npm test
```

- `src/services/bank/normalize.test.ts` — Enable Banking atbilžu apstrāde.
  Šis ir produkcijas ceļš, ko mock režīms apiet pilnībā, tāpēc testi ir vienīgais,
  kas to izpilda, kamēr nav īsta PSD2 savienojuma.
- `src/services/subscriptions.summary.test.ts` — kopsummas un sadalījums pa
  kategorijām, t.i. skaitļi, ko lietotājs redz uz pārskata ekrāna.
- `src/services/bank/fixtures.test.ts` — testa datu īpašības: nav nākotnes
  datumu, bankas ID ir unikāli (no tā atkarīga dedublikācija), ir gan cenas
  izmaiņa, gan troksnis, ko AI nedrīkst atzīmēt kā abonementu.
- `src/services/claude.service.test.ts` — **privātuma garantija** (uz API aiziet
  tikai datums, apraksts un summa; jebkuri papildu lauki tiek nogriezti),
  atbildes shēmas validācija un SDK kļūdu pārtulkošana.

**Dūmu tests** (`backend/scripts/smoke-test.sh`) startē uzbūvēto serveri un
izbrauc visu plūsmu mock režīmā: reģistrācija, 401 bez tokena, bankas
savienojums, sinhronizācija, atkārtota sinhronizācija (nedrīkst dublēt),
abonementu saraksts un 404 nezināmam ID. Tas pārbauda to, ko tipu pārbaude
nevar — ka serveris tiešām startē un savienojas ar datubāzi.

To pašu skriptu var palaist lokāli pēc `npm run build` un migrācijām:

```bash
cd backend
./scripts/smoke-test.sh
```

AI endpointi CI netiek skarti — tiem vajadzīga `ANTHROPIC_API_KEY`, un tā
netiek glabāta repozitorijā.

## Backend uz Render

Vajadzīgs **B ceļam** (Expo Go pret internetu) un **C ceļam** (APK). Repozitorijā
ir `render.yaml`, tāpēc pietiek ar Blueprint:

1. [Render](https://render.com) → **New** → **Blueprint** → norādi šo repozitoriju
2. Render palūgs divus noslēpumus:
   - `TOKEN_ENCRYPTION_KEY` — ģenerē ar `openssl rand -hex 32`
   - `ANTHROPIC_API_KEY` — no [console.anthropic.com](https://console.anthropic.com)
3. **Apply**

Migrācijas tiek pielietotas automātiski pirms servera starta. Pārbaude:
`curl https://<tavs-serviss>.onrender.com/health`

Atbildē ir `"aiConfigured"`. Ja tur ir `false`, atslēga nav nonākusi līdz
serverim.

> **Svaigai izvietošanai datubāze ir tukša.** Pārskats būs tukšs, līdz esi
> reģistrējies, savienojis banku un sinhronizējis darījumus. Ja
> `ANTHROPIC_API_KEY` nav iestatīta, sinhronizācija izdodas, bet **analīze
> atgriež 503 un abonementu saraksts paliek tukšs** — lietotne izskatās salauzta,
> lai gan tikai trūkst atslēgas.

> **Bezmaksas plāni mēdz iemigt** pēc dīkstāves, tāpēc pirmais pieprasījums pēc
> pauzes var aizņemt ~minūti, un datubāzēm var būt derīguma termiņš. Pārbaudi
> aktuālos noteikumus, ja plāno uz to paļauties ilgāk.

---

## C ceļš: APK — lietotne bez datora

Expo Go der izmēģināšanai, bet tam vajag ieslēgtu datoru tajā pašā Wi-Fi. Lai
lietotne būtu ikona sākuma ekrānā un strādātu jebkur, vajag uzbūvētu APK un
backend internetā (sk. **Backend uz Render** augstāk).

Ja Render adrese atšķiras no `abonementi-api.onrender.com`, nomaini to
`mobile/eas.json` failā (`preview` profila `EXPO_PUBLIC_API_URL`).

```bash
cd mobile
npm install -g eas-cli
eas login                                  # vajag Expo kontu (bezmaksas)
eas init                                   # ieraksta projectId app.json failā
eas build -p android --profile preview     # dažas minūtes Expo serveros
```

Kad būvējums beidzies, EAS iedod lejupielādes saiti. Atver to telefonā, lejupielādē
APK un uzstādi — Android palūgs atļaut instalēšanu no šī avota.

> **Kāpēc `preview`, nevis `production`:** `preview` profils būvē `.apk`, ko var
> uzstādīt tieši. `production` būvē `.aab`, kas ir Play Store formāts un tieši
> uzstādīties nevar.

> **Pakotnes nosaukums ir `com.example.abonementuaudits`.** Sānielādēšanai der,
> bet Google Play `com.example.*` noraida. Ja plāno publicēt, nomaini to
> `app.json` failā uz savu domēnu apgrieztā secībā (piem. `lv.tavsdomens.abonementi`).
> Maiņa nozīmē pārinstalēt lietotni; dati ir serverī, tāpēc nekas nepazūd.

### Kas pagaidām paliek mock režīmā

`render.yaml` iestata `ENABLE_BANKING_MOCK=true` un `GMAIL_MOCK=true`, tāpēc
uzbūvētā lietotne strādā ar testa datiem. Īstiem bankas datiem vajag PSD2 līgumu;
īstam Gmail — Google OAuth klientu un, publiskiem lietotājiem, `gmail.readonly`
ierobežotās atļaujas drošības auditu. Abi ir līgumi un audits, ne kods.

### iOS

Tas pats EAS ceļš, bet ierīces instalēšanai vajag **Apple Developer** kontu
(99 $/gadā) un TestFlight. Bez tā iet tikai 7 dienu pagaidu instalācija caur
Xcode uz Mac.

---

## Kas apzināti nav MVP daļa

- Automātiska atcelšanas / pārrunu vēstuļu nosūtīšana
- Balss zvani pakalpojumu sniedzējiem
- Vairākas valstis un valodas vienlaikus
- Pilna budžeta un izdevumu izsekošana
- Maksājumu veikšana (bankas piekļuve ir tikai lasīšanas režīmā)

## Zināmie ierobežojumi

- Abonementa maksājumu vēsture tiek sasaistīta pēc tirgotāja nosaukuma
  meklēšanas darījumu aprakstos. Precīzai sasaistei vajadzīga atsevišķa
  saite starp `transactions` un `subscriptions`.
- Testi sedz tīro loģiku un HTTP plūsmu (dūmu tests), bet ne faktisko izsaukumu
  uz Claude API — tam vajadzīga `ANTHROPIC_API_KEY`. Pārbaudīts ir viss līdz
  pieprasījuma nosūtīšanai un viss pēc atbildes saņemšanas; nepārbaudīts paliek
  tas, cik precīzi modelis atpazīst abonementus. To var noskaidrot tikai ar īstu
  atslēgu.
- Maršrutu slānis nav pārklāts ar vienībtestiem — to daļēji sedz dūmu tests.
- Bankas un Gmail tokeni datubāzē tiek šifrēti ar AES-256-GCM
  (`TOKEN_ENCRYPTION_KEY`). Atslēga glabājas vidē, tāpēc produkcijā to labāk
  turēt atslēgu pārvaldībā (piem. KMS), nevis vides mainīgajā.
