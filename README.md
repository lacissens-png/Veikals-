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
| `mobile/` | React Native (Expo SDK 57) aplikācija ar 7 ekrāniem |
| `Index.html`, `Style.css` | Nesaistīts agrāks online veikala demo — MVP to neizmanto |

---

## Priekšnosacījumi

- Node.js 20 vai jaunāks
- PostgreSQL 14+ (lokāli vai Docker)
- Anthropic API atslēga — [console.anthropic.com](https://console.anthropic.com)
- Enable Banking konts — **nav obligāts**, sk. mock režīmu zemāk

---

## Ātrā palaišana (bez bankas līguma)

Ar `ENABLE_BANKING_MOCK=true` visa plūsma strādā ar reālistiskiem testa
darījumiem, tāpēc projektu var palaist pirmajā dienā, negaidot PSD2 līgumu.

### 1. Datubāze

```bash
# Ar Docker
docker run -d --name abonementi-db \
  -e POSTGRES_PASSWORD=postgres \
  -e POSTGRES_DB=abonementi \
  -p 5432:5432 postgres:16

# Vai izmanto jau esošu PostgreSQL un tikai izveido datubāzi:
createdb abonementi
```

### 2. Backend

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

### 3. Mobilā aplikācija

```bash
cd mobile
npm install
npx expo start
```

Skenē QR kodu ar Expo Go vai palaid emulatorā (`a` — Android, `i` — iOS).

Ātrai apskatei bez telefona der arī pārlūks:

```bash
EXPO_PUBLIC_API_URL=http://localhost:4000 npx expo start --web
```

> **Pārlūkā sesija nesaglabājas.** `expo-secure-store` ir tikai vietējām
> platformām, tāpēc pēc lapas pārlādes jāpiesakās no jauna. Uz telefona tas
> strādā normāli. Web režīms ir domāts UI apskatei, nevis lietošanai.

> **Uz fiziskas ierīces** `localhost` norāda uz pašu telefonu. Norādi datora IP:
> ```bash
> EXPO_PUBLIC_API_URL=http://192.168.1.10:4000 npx expo start
> ```
> Android emulatorā backend ir pieejams kā `http://10.0.2.2:4000` (jau noklusējums).

### 4. Plūsma aplikācijā

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
| `TOKEN_ENCRYPTION_KEY` | ar īstu banku | 64 hex rakstzīmes; bankas tokenu šifrēšanai. Mock režīmā nav vajadzīgs |
| `ENABLE_BANKING_CLIENT_ID` / `_SECRET` | ražošanai | Enable Banking atslēgas |
| `ENABLE_BANKING_API_URL` | nē | Noklusējums `https://api.enablebanking.com` |
| `ENABLE_BANKING_REDIRECT_URI` | nē | Kur banka atgriež lietotāju |
| `APP_REDIRECT_URL` | nē | Deep link uz aplikāciju, noklusējums `abonementi://bank-callback` |
| `TRANSACTION_SYNC_MONTHS` | nē | Cik mēnešus ielādēt, noklusējums `3` |
| `PORT`, `NODE_ENV`, `CORS_ORIGIN`, `JWT_EXPIRES_IN` | nē | Standarta iestatījumi |

Konfigurācija tiek validēta ar zod aplikācijas startā — trūkstošs vai nederīgs
mainīgais dod skaidru kļūdu uzreiz, nevis `undefined` pusceļā.

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

## Izvietošana (Railway / Render)

Backend:

1. Izveido PostgreSQL servisu, iekopē tā `DATABASE_URL`
2. Root direktorija: `backend`
3. Build komanda: `npm install && npm run build`
4. Start komanda: `npx prisma migrate deploy && npm start`
5. Pievieno visus vides mainīgos no tabulas augstāk
6. Health check ceļš: `/health`

Mobilā aplikācija tiek būvēta ar EAS Build (`npx eas build`).

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
- Bankas tokeni datubāzē glabājas atklātā tekstā. Pirms produkcijas tie
  jāšifrē (piem., ar KMS vai `pgcrypto`).
