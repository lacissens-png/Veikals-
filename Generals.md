# Ģenerāļu Sistēma — 9 Kungu Ģenerāļi

*Pilna specifikācija par jauno komandas sistēmu — 3 ģenerāļi per klase (9 kopā), aizvieto veco minjonu bezgalīgo armiju ar 3 spēcīgiem, personificētiem sabiedrotajiem.*

---

## 1. Kāpēc mainījām

Vecais minjonu system (10-30 minjoni per player) bija Overlord-flavored, bet cieta no:
- **Visual noise** — ekrāns pilns ar mazām figūrām, grūti sekot cīņai
- **Vājš bond** — minjoni nav individuāli, tikai numbers
- **Command friction** — sarežģīti komandēt tik daudzus vienlaikus
- **Balance grūtums** — buff visiem 30 = eksponenciāls broken potenciāls

Jauns **3 Ģenerāļu system** dod:
- **Fokusētu strateģisko dziļumu** — katrs ģenerālis matter
- **Emocionāls bond** — ģenerāļiem ir vārdi, personības, backstory
- **Skaidra komanda** — 1/2/3 keys izvēlas, RMB norāda mērķi
- **Balansēts kombu potenciāls** — 3 aktīvi = 3 kombu asis

---

## 2. Kopīgā mehānika

### 2.1. Bāzes struktūra

Katrs spēlētājs pastāvīgi ir ar **3 ģenerāļiem** aktīviem cīņā:
- Katrs ģenerālis ir **unikāla figūra** ar vārdu, klasi, personību
- Katrs ir **level-scaled** ar spēlētāju (vienmēr matched level)
- Katrs ir **fully equipped** ar 3 gear slotiem
- Katrs ir **fully upgradeable** caur Bond system

### 2.2. Gear slots (3 katram ģenerālim)

| Slot           | Kas iet                                    |
|----------------|--------------------------------------------|
| **Ierocis**    | Klases-atbilstošs ierocis                  |
| **Bruņas**     | Rumpis / ķivere kopā                       |
| **Aksesuārs**  | Amulets, gredzens, vai unikāla lieta       |

Ģenerāļi valkā **savas klases-atbilstošas mantas** — Tank ģenerāļi grib heavy bruņas, DPS grib crit gear, u.c. Gear rarity ir tāda pati kā spēlētājam (Parasts → Uber Mistiskais).

### 2.3. Komanda un vadība

| Keys / Actions        | Efekts                                                       |
|-----------------------|--------------------------------------------------------------|
| **1** / **2** / **3** | Izvēlas atsevišķu ģenerāli (par kuru RMB norāda)             |
| **RMB uz mērķa**      | Ģenerālis (vai visi 3 ja neviens izvēlēts) uzbrūk mērķim     |
| **RMB uz vietas**     | Ģenerāļi pārvietojas uz punkta                                |
| **F**                 | *Fall Back!* — visi ģenerāļi atgriežas pie tevis             |
| **G**                 | *Hold Position!* — ģenerāļi paliek uz vietas                  |
| **T**                 | Formation cycle (triangle → line → surround → default)       |
| **R** (kad selected)  | Ģenerāļa **Signāles Prasme** (skat. katrs individuāli)       |

### 2.4. Bond sistēma

Katrs ģenerālis sāk **Bond 1** un aug līdz **Bond 10**:

| Bond Lv | Kā uzkrāj                     | Ko atslēdz                                       |
|---------|-------------------------------|--------------------------------------------------|
| 1       | Sāc                           | Bāzes prasmes                                    |
| 2       | 10h kombinētā cīņa            | +10% dmg                                         |
| 3       | 25h                           | +15% HP                                          |
| 4       | 50h                           | Jauna passive perk (izvēle no 2)                 |
| 5       | 100h                          | **Ultimate Ability** unlocked                    |
| 6       | 150h                          | +25% AS                                          |
| 7       | 200h                          | Jauna passive perk (izvēle no 2)                 |
| 8       | 300h                          | +30% Crit Dmg                                    |
| 9       | 400h                          | Personal backstory quest atslēdzas               |
| 10      | 500h + backstory pabeigts     | **Legendary Perk** + kosmētisks skin             |

**Bond aug ar:**
- Kombinētu cīņu (viens tick per 5 min cīņā ar ģenerāli aktīvu)
- Kills ar ģenerāli klātesošu (mikro-XP)
- Kaltēšana viņa gearam pie amatnieka
- Backstory quest solīņu pabeigšana

### 2.5. Nāve / atgūšana

- Ģenerālis **knock-out** (nav permanent death, jo viņi ir stāsta figūras).
- Downed ģenerālis:
  - **Cīņā:** revive pēc 30s vai kad cīņa beidzas
  - **Ārpus cīņas:** revive brīvi
- **Player revive**: pieej pie downed ģenerāļa un turi F 2s

### 2.6. Ģenerāļu limits vienlaicīgi

- **3 aktīvi** vienlaikus (visi 3 tavas klases ģenerāļi)
- Nav swap-in/swap-out — visi 3 vienmēr aktīvi
- Nav "reserves" — ģenerāļi ir uzticīgi tavai klasei

---

## 3. Pirmais Kungs Ģenerāļi

### 3.1. **Rose** — Sardzes Māte (Tank)

> *Kritušā paladinu ordeņa mājot bija Rose — reiz Debesu Bīskapa meita, tagad Pirmā Kunga uzticīgā tanka.*

**Klase:** Tank / Melee
**Ierocis:** Zobens + Vairogs (dual wield defensive)

**Bāzes stati (Level 80):**
- HP: 80 000
- Bāzes dmg: 400 per swing
- AS: 0.9/sec
- Bruņas: 15 000

**Signāles Prasme (R):** **Sardzes Vairogs**
> 8s uz Rose absorbē 50% no visiem tavas grupas saņemtajiem bojājumiem. Cooldown: 45s.

**Bāzes Passive:** **Kritušā Ordeņa Sirds**
> Rose atgūst 5% max HP par katru ienaidnieka nāvi tuvāk 5m.

**Bond 5 Ultimate:** **Kritušā Kroni Piesaukt**
> Rose apstājas 10s, izsauc 5 spektrālus Kritušā Ordeņa bruņniekus, kuri cīnās ar tevi 15s.

**Bond 10 Legendary Perk:** **Nemirstīgais Sardze**
> Kad Rose HP zem 20%, viņa iegūst neatgriezenību 5s (1× per cīņa).

**Personality:** noble, corrupted paladin, formal speech. Referīvē pie Ordeņa Zvērestu ietvara.
**Backstory:** meita Debesu Bīskapa, pēc tam apsēsti pēc Melnā Torņa uzbrukuma; tagad kalpo brīvprātīgi ar mikslu no vaiņas un ziedošanās.

---

### 3.2. **Kalts** — Meistara Vecais (Melee DPS)

> *Kalts bija vecākais brūnais minjons, kas dzīvoja Melnajā Tornī jau pirms Pirmā Kunga pamodinājuma. Tagad viņš ir vairāk cilvēka nekā minjons — augsts, spēks, meistarīgs.*

**Klase:** Melee DPS / Aggro
**Ierocis:** Divi zobeni

**Bāzes stati (Level 80):**
- HP: 45 000
- Bāzes dmg: 800 per swing
- AS: 1.4/sec
- Bruņas: 6 000

**Signāles Prasme (R):** **Melnais Vaids**
> 360° spin ar 200% weapon dmg AoE + 2s stun visiem apkārtējiem. Cooldown: 30s.

**Bāzes Passive:** **Meistara Sirds**
> Pēc katras signāles prasmes izmantošanas, Kalts +25% dmg 10s.

**Bond 5 Ultimate:** **Divu Zobenu Kaskade**
> Kalts kļūst neapstādināms 8s, katrs sitiens ir crit un lec 3 tuvākajiem mērķiem.

**Bond 10 Legendary Perk:** **Vecās Asinis**
> Katrs Kalta kills dod tev +2% dmg buff 30s (max stack 10 = +20%).

**Personality:** gruff, loyal, ancient. Runā īsi, bieži citē vecās minjonu tradīcijas.
**Backstory:** dzīvoja Melnajā Tornī tūkstošgades, kalpoja daudziem Kungiem; korrumpēts par cilvēku-lielu formu pēc Pirmā Kunga rituāla.

---

### 3.3. **Velvet** — Nolādētā Priestere (Magic / Support)

> *Velvet bija ceremoniālā priestere, kas jaunā vecumā izvēlējās Pirmā Kunga ceļu pār gaišā ordeņa. Viņa ir gan mistress, gan cīņas mag.*

**Klase:** Magic / Support / Healer
**Ierocis:** Zizli + gredzens

**Bāzes stati (Level 80):**
- HP: 35 000
- Bāzes dmg: 600 (spell dmg per cast)
- AS: 1.2/sec cast speed
- Bruņas: 4 000

**Signāles Prasme (R):** **Nolādētais Zvans**
> AoE 15m rādiuss — heal visai grupai 30% max HP + +20% dmg 8s. Cooldown: 60s.

**Bāzes Passive:** **Velvet's Grace**
> Visi grupas locekļi (spēlētājs + Rose + Kalts) regen 2% HP/sec cīņā.

**Bond 5 Ultimate:** **Nolādētā Sabiedrība**
> Velvet uz 10s uzstāda pastāvīgu debuff visiem ienaidniekiem 20m rādiusā (-30% dmg, +30% saņemtais dmg).

**Bond 10 Legendary Perk:** **Priesteres Vēls**
> Kad grupas loceklis mirst, Velvet automātiski revive viņu ar 50% HP (1× per cīņa, 5min cooldown).

**Personality:** charming, deadly, subtle. Priesteres balss, bet ar dziļu tumsu.
**Backstory:** bijusi Debesu Bīskapa māsa, kļuva par Pirmā Kunga mistress un pēc tam viņa augstākā priestere; komanda ir gan spirituāla, gan romantiska.

---

## 4. Otrs Kungs Ģenerāļi

### 4.1. **Kaspars** — Impērijas Karagalvenis (Tank)

> *Kaspars bija Impērijas Rītesāta 1. Karagalvenis, Nordbergas nogrāvu pastāvīgais aizstāvis. Kad Otrs Kungs iekaroja Empire, Kaspars pirmais nolieca zobenu.*

**Klase:** Heavy Tank / Warrior
**Ierocis:** Divkājs cirvis

**Bāzes stati (Level 80):**
- HP: 95 000
- Bāzes dmg: 500 per swing
- AS: 0.8/sec
- Bruņas: 18 000

**Signāles Prasme (R):** **Impēriskais Sitiens**
> Milzīgs melee AoE cirtiens 10m priekšā, 300% weap dmg + 3s stun. Cooldown: 40s.

**Bāzes Passive:** **Impēriskais Aizsargs**
> Kad HP zem 50%, Kaspars +30% bruņas un +25% DR.

**Bond 5 Ultimate:** **Nordbergas Kara Standarts**
> Kaspars ievieto karogu zemē 15s — visi ienaidnieki tur -20% dmg, visi tavas grupas +30% dmg.

**Bond 10 Legendary Perk:** **Impēriskais Priekšgals**
> Kaspars savā signāles prasmē pievieno 1 papildu charge (2 kopā).

**Personality:** proud, honorable warrior. Runā formāli, bieži ar imperātora frāzēm.
**Backstory:** Impērijas 20 gadu veterāns, viņa dēls krīta iekarošanas laikā; tagad viņš ir uzticīgs Otrajam Kungam kā viņa dēla lieciniekam.

---

### 4.2. **Fay** — Elfu Karaliene (Ranged DPS / Magic)

> *Fay bija Nordbergas mežu Elfu Karaliene, kas noraidīja Impēriju gan gaisajā, gan tumšajā formā. Otrs Kungs viņu pārliecināja ar respektu — nesa viņas mežā mazu upuri.*

**Klase:** Ranged Magic DPS
**Ierocis:** Elfu loks + arkānais dagas

**Bāzes stati (Level 80):**
- HP: 40 000
- Bāzes dmg: 900 (per bulta / cast)
- AS: 1.3/sec
- Bruņas: 5 500

**Signāles Prasme (R):** **Elfu Bulta Kaskade**
> 5 magic bultu cone 15m priekšā, katra 100% weap dmg + 30% freeze chance. Cooldown: 20s.

**Bāzes Passive:** **Elfu Redzējums**
> Grupa (spēlētājs + 3 ģenerāļi) +25% crit chance.

**Bond 5 Ultimate:** **Māte Meža**
> Fay uz 15s izsauc 6 root vines apkārt viņai, kuras auto-uzbruk ienaidniekiem un uzliek Vulnerable.

**Bond 10 Legendary Perk:** **Elfu Karalienes Zvans**
> Kad Fay iesit kritisko, ir 25% chance nogalināt non-boss ienaidnieku instantly.

**Personality:** cold, wise, mysterious. Runā mazi vārdi, dziļš toņs.
**Backstory:** simtiem gadu vec, redzējusi trīs Overlord ērās; viņas motivi ir sarežģītas — daļu spēlē kalpo, daļu izpēta.

---

### 4.3. **Juno** — Nordbergas Sniedzniece (Melee DPS / Assassin)

> *Juno bija Nordbergas sacēlās vietējā, kas cīnījās pret Empire ar dunčiem un tumsu. Pēc Otrā Kunga iekarošanas, viņa izvēlējās viņu par savu jauno saimnieku — brīvāku par Empire.*

**Klase:** Melee Assassin / Rogue DPS
**Ierocis:** Divi dunči

**Bāzes stati (Level 80):**
- HP: 35 000
- Bāzes dmg: 750 per swing (crit-heavy)
- AS: 1.7/sec
- Bruņas: 4 000

**Signāles Prasme (R):** **Sniedznieces Zibens**
> Teleport-backstab uz mērķi 20m ietvarā, 250% weap dmg + 5s bleed (10% max HP/sec). Cooldown: 25s.

**Bāzes Passive:** **Nordbergas Solis**
> Grupa (spēlētājs + 3 ģenerāļi) +20% MS aura.

**Bond 5 Ultimate:** **Sniedznieces Klons**
> Juno atstāj 2 klonus, kas cīnās 10s (katra sits ar 50% no viņas dmg).

**Bond 10 Legendary Perk:** **Sacēlās Sirds**
> Katrs Juno backstab kills reset viņas signāles prasmes cooldown.

**Personality:** rebellious, fierce, loyal. Runā uz atriebību un brīvību.
**Backstory:** Nordbergas maza ciema meita, Empire nogalināja viņas ģimeni; sākumā mēģināja slepkavot Otrs Kungs, bet viņa cieņa iekaroja viņu.

---

## 5. Vecais Burvis Ģenerāļi

### 5.1. **Torins** — Vecākais Spellblade (Melee Hybrid)

> *Torins bija Vecā Burvja 1. skolēns pirms Torņa krita. Kad pārējie mācekļi izklīda vai kļuva par dēmoniem, Torins turpināja studēt savu meistaru — un tagad ir viņa jauns roka.*

**Klase:** Spellblade Hybrid (melee + spell)
**Ierocis:** Arkānais glaive

**Bāzes stati (Level 80):**
- HP: 55 000
- Bāzes dmg: 700 per swing (mixed physical + arcane)
- AS: 1.2/sec
- Bruņas: 8 000

**Signāles Prasme (R):** **Arkānais Sitiens**
> Melee + spell kombinēts uzbrukums 5m rādiusā, 200% weap dmg + 100% spell dmg. Cooldown: 25s.

**Bāzes Passive:** **Torņa Ass**
> Pēc Torina sitiena, spēlētāja nākošā prasme 5s laikā +50% dmg.

**Bond 5 Ultimate:** **Kroņa Māc**
> Torins caur reality teleports uz tavas pozīcijas + uzsit visiem 15m rādiusā ar 500% weap dmg AoE.

**Bond 10 Legendary Perk:** **Meistara Sirds**
> Kad tu izmanto Reality Rift, Torins arī iegūst dubultā dmg buff.

**Personality:** disciplined, cold, focused. Runā ar arkāno terminoloģiju.
**Backstory:** 30 gadi studēja pie Vecā Burvja; sākumā skolēns, tagad kolēģis; motivi — cerība atgūt sava meistara pilno formu.

---

### 5.2. **Sasla** — Ledus Māte (Frost Mage)

> *Sasla bija Vecā Torņa cietekšņa ledus rituālu meistariene. Kad Torņis krita, viņa izbēga uz kalniem; tavs pamodinājums pieaicināja viņu atpakaļ.*

**Klase:** Frost Mage / Zone Control
**Ierocis:** Ledus zizli + gredzens

**Bāzes stati (Level 80):**
- HP: 40 000
- Bāzes dmg: 850 (spell dmg per cast)
- AS: 1.1/sec cast speed
- Bruņas: 5 000

**Signāles Prasme (R):** **Sasala Torņa**
> Sasla ievieto 15s ledus torni zemē — auto-uzbrūk 8m rādiusā tuvākajiem mērķiem par 100% spell dmg katrā tickā (1/sec) + freeze pastāvīgi. Cooldown: 45s.

**Bāzes Passive:** **Sasala Aura**
> Ienaidnieki 8m rādiusā ap Saslu -20% MS + -10% dmg.

**Bond 5 Ultimate:** **Ziemas Māte**
> Sasla izmet 20m rādiusā ledus vēju 8s — visi ienaidnieki tur sasala (freeze) 4s + saņem 300% spell dmg per sec.

**Bond 10 Legendary Perk:** **Sasala Sirds**
> Kad Sasla ledus torni ievieto, tavs Reality Rift patēriņš -50% 15s.

**Personality:** quiet, wise, mysterious. Runā vecās frāzes, referīvē pie ziemas dievu.
**Backstory:** dzīvojusi ap 200 gadu, esoterisks ledus rituālu meistars; sagaida pareizu laiku atklāt viņas patieso jaudu.

---

### 5.3. **Vera** — Realitātes Kalēja (Teleport Support)

> *Vera bija Vecā Torņa jaunākā mācekle — un pati talantīgākā. Viņa specializējās reality manipulation, teleport, and space-bending. Vecais Burvis viņu bieži nosauca par savu potenciāļo nākošo veidolu.*

**Klase:** Teleport Support / Utility
**Ierocis:** Portable arkānais artefakts + zizli

**Bāzes stati (Level 80):**
- HP: 32 000
- Bāzes dmg: 500 (spell dmg per cast)
- AS: 1.4/sec cast speed
- Bruņas: 4 500

**Signāles Prasme (R):** **Realitātes Plaisa**
> Vera atver plaisu 15m uz mērķa vietas — visi grupas locekļi (tu + 3 ģenerāļi) teleport tur instantly. Cooldown: 30s.

**Bāzes Passive:** **Vera's Blink**
> Grupa (spēlētājs + 3 ģenerāļi) +15% dodge chance + 15% MS.

**Bond 5 Ultimate:** **Realitātes Chaos**
> Vera 8s rada reality distortion 25m rādiusā — visi ienaidnieki tur random teleport ik pēc 1s, saņem Vulnerable 4s.

**Bond 10 Legendary Perk:** **Kalēja Sirds**
> Kad tu izmanto Realitātes Plaisa (savu prasmi), Vera automātiski atkārto to bezmaksas 3s vēlāk.

**Personality:** eccentric, brilliant, chaotic. Runā ātri, mīl ekspermintus.
**Backstory:** jauna, tikai 22 gadus veca, bet aug ar ekstrēmu ātrumu; viņas potenciāls ir tas, kas nāk pēc Vecā Burvja atgriezšanas — vai nēmurstoša piedāvāta jauna versija.

---

## 6. Ģenerāļu ekipējuma sistēma

### 6.1. Kā ekipēt ģenerāli

Pie Melnā Torņa (vai jebkurā drošā telpā):
- Atveri Ģenerāļu Ekrānu (I taustiņš)
- Izvēlies ģenerāli
- Drag-and-drop viņa 3 slotos savu inventāra items (weapon, armor, accessory)
- Ģenerālis atbilst tikai konkrētiem items pēc klases (Rose var valkāt smago bruņu, Velvet var tikai vieglo)

### 6.2. Gear rarity un stat efekti

Ģenerāļu gear izmanto to pašu retumu sistēmu kā spēlētāja:
- **Parasts** — bāzes stati
- **Rūdīts** (1 affix), **Sens** (4 affixi), **Reliktu** (4 affixi + Aspekts)
- **Nolādēts** (unique, specific to a general)
- **Uber Mistiskais** (per-general)

**Ģenerāļu-specifiskie Uber Mistiskie:**
- **Rose:** "Kritušā Kroni Vairogs" — Sardzes Vairogs cooldown -50%, ilgums +5s
- **Kalts:** "Meistara Zobens" — Melnais Vaids dubultā radius
- **Velvet:** "Priesteres Vēls" — Nolādētais Zvans reset cooldown ja grupas loceklis mirst
- **Kaspars:** "Impērijas Kronis" — Impēriskais Sitiens +100% dmg pret bosiem
- **Fay:** "Elfu Karalienes Bulta" — bultas ienesamas pret bruņām
- **Juno:** "Sniedznieces Duncis" — backstab crit chance 100%
- **Torins:** "Arkānais Glaive" — Arkānais Sitiens + spell dmg vēl 100%
- **Sasla:** "Ledus Kroņis" — Sasala Torņa 3 torņi vienlaikus
- **Vera:** "Realitātes Vaids" — Realitātes Plaisa teleport 30m + slow

### 6.3. Kalēšana ģenerāļu gearam

Pie Kalēja (spēlētāja Masterworking sistēma):
- Ģenerāļa gear var **Masterwork** 12 līmeņu (tāpat kā spēlētāja)
- **Tempering** un **Aspekta imprint** arī pieejams
- Var izmantot to pašu materiālu inventāru

---

## 7. Ģenerāļu prasmju koks

Katram ģenerālim ir **mini prasmju koks** ar **~15 punktiem**, kurus atslēdz caur Bond levels un individuāli quest-atpakaļu.

**Piemērs — Rose prasmju koks:**

**Level 1 (Bond 1):**
- Sardzes Instinkts (+10% blockChance)
- Vairoga Māstrija (+15% Sardzes Vairogs efektivitāte)

**Level 4 (Bond 4) izvēle:**
- **Sardzes Solis** — kad viņa blockē, tavs kritiskais bojājums +10% 5s
- **Nemirstīgā Sardze** — Rose HP regen +50% cīņā

**Level 7 (Bond 7) izvēle:**
- **Kritušā Uguns** — Rose blockēti hits atspoguļo 50% dmg atpakaļ
- **Sardzes Vairogs Sirds** — Sardzes Vairogs pastāvīgi (bez cooldown), bet 50% zemāks absorb

**Level 10 (Bond 10) — Legendary:**
- **Nemirstīgais Sardze** (default Bond 10 perk)

Šī ir personalizācija — spēlētājs pielāgo katru ģenerāli savam buildam.

---

## 8. Ģenerāļu backstory quest system

Katram ģenerālim ir **personal backstory quest**, kas atslēdzas **Bond 9**:
- 3 quest solīnās per ģenerālis
- Ievedā jaunas dungeons, cīņas ar bijušajām saitēm, morāles izvēles
- Kad pabeigts: **Bond 10** + kosmētisks skin + Legendary Perk

**9 backstory quests kopā** (viens per ģenerālis) = **~15-20h saturs endgame beyond main story**.

---

## 9. Ģenerāļi klasēm — kā tas mainās cīņā

Vecais minjonu system: spēlētājs uzsāka cīņu ar 10-30 mazi minjoni, kas mira ātri un tika replaced.
Jaunais ģenerāļu system: spēlētājs uzsāka cīņu ar 3 spēcīgiem ģenerāļiem, kuri ir viņa **stāsta figūras**, ne cannon fodder.

### 9.1. Cīņas ritms

**Vec:** RMB uz mērķa → 30 minjoni skrien pie tā, chaos.
**Jauns:** RMB → 3 ģenerāļi organizēti pieej mērķim, katrs pēc savas taktikas (Tank pirmais, DPS aizmugurē, Support kanālē).

**Kombu piemērs (Pirmais Kungs):**
1. Rose iebrauc → Sardzes Vairogs → tu iegūsti damage reduction
2. Tu izmanto Kunga Sitiens combo
3. Kalts pēkšņi teleports uz galveno mērķi → Melnais Vaids AoE stun
4. Velvet Nolādētais Zvans → grupas heal + dmg buff
5. Boss zaudē 50% HP 5 sekundēs no perfekta combo

### 9.2. Klases-specifiskās kombo sinerģijas

Katras klases 3 ģenerāļi ir dizainēti kā **complementary trio**:

| Klase          | Trio kompozīcija                    | Sinerģija                                        |
|----------------|-------------------------------------|--------------------------------------------------|
| Pirmais Kungs  | Tank (Rose) + Melee DPS (Kalts) + Support (Velvet) | Klasiskā MMO trinity |
| Otrs Kungs    | Tank (Kaspars) + Ranged DPS (Fay) + Assassin (Juno) | Formation warfare — Kaspars agro, Fay+Juno flanku |
| Vecais Burvis  | Spellblade (Torins) + Mage (Sasla) + Utility (Vera) | Arkānais Chaos — teleports, freeze towers, spellblade combos |

---

## 10. Ekonomika un progresija

### 10.1. Ģenerāļu iegūšana

- **Sākumā:** visi 3 tavas klases ģenerāļi ir pieejami no Level 1
- Bez unlocking mechanic — viņi ir tavas klases stāsta figūras
- Alternatīva: Level 1-15 ir tikai 1 ģenerālis; Level 16 unlock 2., Level 25 unlock 3.

### 10.2. Ģenerāļu gearing prioritāte

Endgame spēlētājs pavada **~30% no viņa gearing laika ģenerāļiem** (bija ~10% ar minjoniem, jo tie bija maināmi). Šī ir dziļāka investment, bet tur bond value.

### 10.3. Bond farming cikls

- 100h uz Bond 5 = normāla progresija
- Casual player sasniedz Bond 5 ~50-80h main content
- Hardcore player sasniedz Bond 10 pilnu 500h endgame

**Kopīgi 3 ģenerāļi Bond 10 = ~1500h investment** — tas ir top 5% player horizon.

---

## 11. Broken potenciāls jaunam systēmam

Ar ģenerāļiem, broken combos ir vairāk:
- **Personalizēti** — tavas 3 ģenerāļu specifiskās signāles + tavas prasmes
- **Bond-atkarīgi** — legendary perki dod build-defining efektus
- **Gear-atkarīgi** — 9 Uber Mistiskie ģenerāļu items + 6 spēlētāja = **15 uniques Uber pool**

Piemērs broken combo (Vecais Burvis):
- Tu: Realitātes Karaļvalsts keystone → Reality Rift pastāvīgs
- Vera Bond 10 Legendary: Realitātes Plaisa atkārtojas 3s vēlāk bezmaksas
- Sasla Bond 10: Sasala Torņa 3× vienlaikus
- Torins Bond 10: Reality Rift laikā viņš arī dubultā dmg buff

= **Time slow + 3 ice turrets + Torins burst + Vera double teleport** = potenciāls **~100G peak burst** (skat. BrokenBuilds.md par comparison).

---

## 12. Refactor impact — kas jāatjauna

Šī sistēmas maiņa ietekmē:
- ✅ `GameDesignDocument.md` §6 (Minjonu komandēšana → Ģenerāļi)
- ✅ 3 BuildGuides §1.4 (Minjonu afinitāte → Ģenerāļu trio kompozīcija)
- ⚠ `ParagonBoards.md` — Barvedis dēlis → **Ģenerāļu Sardzes** dēlis
- ⚠ `Aspects.md` — Minjonu aspekti (25-32) → **Ģenerāļu aspekti**
- ⚠ `Runes.md` — Asinssaite Rūna (Barveža bonusi) → Ģenerāļu bonusi
- ⚠ `DamageMath.md` — minjonu DPS kalkulācijas → ģenerāļu DPS
- ⚠ `BrokenBuilds.md` — piemēri ar minjoniem → ar ģenerāļiem
- ⚠ `Dungeons.md` — minor references

Skaidros gadījumos vārds "minjoni" tiks aizvietots ar "ģenerāļi" ar attiecīgu skaita adjustmenti (30 → 3).

---

*Skaitļi un dizains ir sākuma piedāvājums. Tuning notiek pēc playtesting.*
