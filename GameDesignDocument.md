# Design dokuments — *Kalna Kungs* (darba nosaukums)

**Žanrs:** Isometrisks action RPG ar ģenerāļu komandēšanu un klašu izvēli
**Iedvesma:** *Overlord* serialā (I, II, Raising Hell, Dark Legend, Fellowship of Evil) × *Diablo IV*
**Engine:** Unreal Engine 5
**Skats:** Top-down / isometrisks, ar rotējamu kameru
**Platforma:** PC (Windows), vēlāk konsoles

---

## 1. Koncepts vienā teikumā

Tu izvēlies **vienu no trīs Kungiem** — visstiprākos varoņus visā Overlord serialā (divi bijušie kungi + arch-mage) — un komandē **3 unikālus ģenerāļus** (tavas klases uzticīgais trio) cauri trīs vizuāli atšķirīgām karaļvalstīm, izlaupot loot un pārbūvējot sabrukušu pasauli.

**Overlord daļa:** ģenerāļu trio ir tavs galvenais spēks; klases balstītas uz Overlord serialā ikoniskākajām figūrām.
**Diablo 4 daļa:** blīvs loot, prasmju koki, dungeoni, world bosi, sezonas, endgame.
**Gothic daļa:** viss ir tumšs, drēgns, sakrāls, ar melno humoru.

---

## 2. Pasaule un lore

### Pasaules lore īsumā
- Pasaule saucas **Vaireja**.
- Pirms 300 gadiem **Pirmais Kungs** valdīja no **Melnā Tornī**, savukārt **Otrs Kungs** (viņa dēls) iekaroja Ziemeļu Impēriju. Trešais **Vecais Burvis** valdīja no floating arcane torņa un bija arguably visstiprākais no visiem.
- Visi trīs krita cīņā par varu — Pirmais nomira dabiski, Otrs krita glorius empire karā, Burvis pazuda savas paša reality rift laikā.
- Pasaule sabruka trīs sīkās, savstarpēji naidīgās karaļvalstīs, kurās tagad valda "gaišie" ordeņi, korumpēti bīskapi un fanātiskas mūku brālības.
- Tu esi **jaunais mantinieks** — pamodies ar vienu no trim **Kunga Zīmoglaukumiem** plaukstā. Tavs zīmoglaukums izvēlas, kāda veida Kungs tu esi, kāda ir tava dzimtā karaļvalsts, un kāda no trim leģendārām mantojumu līnijām plūst pa tavu asinīm.

### Vizuālais stils (kopīgais)
- **Palete kodolā:** ogļu melna, asins sarkana, alvas zilā, sveču zelts.
- **Katrai karaļvalstij ir sava piesātināta akcenta krāsa** (skat. klasēs).
- **Arhitektūra:** ziemeļu gotika kā pamats, katrā valstībā ar vietējo pieskaņu.
- **Gaisma:** stipri kontrasti — Lumen dinamiska, ēnas darbojas kā otrs aktieris.

---

## 3. Klases un karaļvalstis

Katra klase ir viens no trim **Kunga Zīmoglaukumiem** — reinkarnētu varoņu līnija ar savu mantojumu, prasmēm, unikālu **3 ģenerāļu trio** un dzimto karaļvalsti. Izvēle sākumā **ir pastāvīga** vienam char slotam (kā Diablo). Katra klase vizuāli atslēdz atšķirīgu **starta reģionu** un mazliet transformē **Melno Torni** (troņa telpas dekorācijas).

### 3.1. Pirmais Kungs — *Pirmā Zīmoglaukuma Mantinieks*
> Iedvesma: **The First Overlord** (Overlord I protagonists) — pamodinātais no viņa uzticīgā trio, sakāva visus 7 varoņus, valdīja no Melnā Torņa.

- **Stils:** balansēts warrior ar spēcīgu 3 ģenerāļu trio (tank + DPS + support). Vidēja tuvcīņa + Rose/Kalts/Velvet backup.
- **Vienā teikumā:** *"Es esmu pirmais un man visi kalpo."*
- **Signāles prasmes:** Kunga Sitiens (LMB melee ar ģenerāļu buff), Master's Call (AoE ģenerāļu regroup + buff), Domination Aura (paranormāla klātbūtne, ģenerāļi +30% dmg), Kunga Zvans (100pt ult: visi 3 ģenerāļi pēkšņi max buffed, tu iegūsti barjeru).
- **Ģenerāļu trio:** **Rose** (paladin tank), **Kalts** (dual-wield DPS), **Velvet** (priestere healer/mage). Katrs +15% dmg un +10% AS ap Pirmo Kungu. Detaļas — skat. `Generals.md`.
- **Signāles resurss:** **Dominance** — uzkrājas ar ģenerāļu kills (3 pt / kill) un tavu personīgo kills (5 pt / kill). Izzūd 1 pt/sec ārpus cīņas.
- **Karaļvalsts: Melnais Tornis (*Melnais Tornis*)**
  - **Vizuāli:** kolosāls gotiskais tornis melnākajā mežā. Iekšā — 3 ģenerāļu kambari (Rose, Kalts, Velvet), kolīdes valodas, un troņa zāle ar Pirmā Kunga bijušajām relikvijām.
  - **Ienaidnieki:** kritušie krustneši, sardzes, apsēsti mūki, iebrukumi no gaisajiem ordeņiem.
  - **Boss:** **Sardzes Kapteinis Bornlejs** — pēdējais gaisais paladins, kurš vēl mēģina Melno Torni "attīrīt".

### 3.2. Otrs Kungs — *Otrā Zīmoglaukuma Mantinieks*
> Iedvesma: **The Second Overlord** (Overlord II protagonists) — pirmā dēls, iekaroja Nordberga un Glorius Empire, hibrīds warrior-mage.

- **Stils:** hibrīds tuvcīņa + tumšas maģijas prasmes. Domina lielas grupas, atslēdz karus.
- **Vienā teikumā:** *"Mans tēvs bija pirmais. Es esmu labākais."*
- **Signāles prasmes:** Impēriskais Sitiens (LMB, kombinē fizisko + shadow), Melnās Zibenis (Q, chain lightning ar shadow), Impērisks Zvans (E, buff apkārtējiem ģenerāļiem +50% dmg 8s), Impērija Vaidēs (100pt ult: masveida shadow AoE + tu iegūsti maģisku formu 15s).
- **Ģenerāļu trio:** **Kaspars** (Impērijas heavy tank), **Fay** (Elfu Karaliene ranged mage), **Juno** (Nordbergas assassin). Kaspars +25% armor auras, Fay +25% party crit, Juno +20% party MS. Detaļas — skat. `Generals.md`.
- **Signāles resurss:** **Impēriskā Dusma** — uzkrāj kad iznīcina "gaišā" ordeņa ienaidniekus (10 pt / kill) vai kad iekaro nozīmīgu vietu (piem., dungeon boss). Izzūd ārpus cīņas.
- **Karaļvalsts: Nordberga Impērija (*Nordberga Impērija*)**
  - **Vizuāli:** sniegots ziemeļu ciems, sabrukusi cietoksnis, imperators dekorācijas (aizrautīgas krāsas), stipri kontrasti starp balts sniega un melnas Otrā Kunga bruņu.
  - **Ienaidnieki:** Glorius Empire paliekas (soldieri Romas stilā), Nordberga sacēlās vietējie, apsēsti bīskapi, elfu klejotāji.
  - **Boss:** **Emperors Solariuss** — Overlord 2 kanonā antagonists, Glorius Empire vecais imperators, tagad kā līķis-imperators.

### 3.3. Vecais Burvis — *Torņa Zīmoglaukuma Mantinieks*
> Iedvesma: **The Wizard** (Overlord I torņa boss) — visstiprākais no 7 varoņiem, arch-mage, kritušais no lepnības.

- **Stils:** tīrs arch-mage. Distances maģija, reality manipulation, teleports, glass cannon.
- **Vienā teikumā:** *"Realitāte ir mans nastīgs materiāls."*
- **Signāles prasmes:** Arkānā Lauska (LMB projectile), Realitātes Plaisa (Q, teleports + AoE), Arkāns Zvans (E, dubultā tavas nākošās 3 prasmes), Reality Rift (100pt ult: 15s time slow apkārtējiem, tavas prasmes 2× dmg un 2× ātrāk).
- **Ģenerāļu trio:** **Torins** (spellblade hybrid), **Sasla** (Ledus Māte frost mage), **Vera** (Realitātes Kalēja teleport support). Torins reset spēlētāja spell buffs, Sasla ice turrets, Vera party teleport. Detaļas — skat. `Generals.md`.
- **Signāles resurss:** **Arkānā Enerģija** — uzkrājas caur kritiskiem uzbrukumiem (5 pt / crit) un maģisko efektu izmantošanu. Nekad neizzūd kaujā.
- **Karaļvalsts: Vecais Tornis (*Vecais Tornis*)**
  - **Vizuāli:** peldošs arkānais tornis debesīs virs Melnā Kalna, apkārt kluss chaos — mainīgas gravitācijas telpas, mainīgi ceļi, ilūzijas.
  - **Ienaidnieki:** Torņa arch-mages sekotāji, sasala golemi, reality distortions (elementāri, kas nespēj nomirt normālā ceļā), bijušie Burvja studenti.
  - **Boss:** **Vecākais Māceklis Selriss** — Vecā Burvja pēdējais students, kurš mēģina turēt Torni pēc meistra pazušanas.

---

## 4. Klašu izvēles ietekme uz pasauli

**Klases izvēle spēles sākumā maina:**

1. **Starta reģions** — tu sāc savas klases karaļvalstī.
2. **Melnais Tornis hub** — troņa telpa ir dekorēta pēc tavas klases (Pirmais Kungs — klasiskais tumšais tornis; Otrs Kungs — impēriskais dekorējums; Vecais Burvis — arkānais tornis).
3. **Ģenerāļu kosmētika un skins** — tava klase atslēdz Bond 10 kosmētiskos skins.
4. **Klases quest līnija** — īpaša stāsta arka par tavas mantojumu, kas notiek paralēli galvenajam sižetam.
5. **Klases boss** — sava reģiona boss ir bijušais tavs zīmoglaukuma turētājs, kurš atsakās tevi atzīt.
6. **Dialogu opcijas** — NPC reaģē citādi (imperiskais Otrs Kungs dabū saluts no ordeņa paliekas, Burvis dabū baiļu skatienus u.c.).

Bet **visas 3 karaļvalstis apmeklē visi** — vienkārši savā secībā, ar savu perspektīvu.

---

## 5. Kopīgais ceturtais reģions

- **Melnais Kalns** — hub un fināla reģions. Nav klases-specifisks, aug pēc katras klases pieskaņas.

Kopā: **3 klašu karaļvalstis + 1 hub Melnais Kalns = 4 reģioni**.

---

## 6. Galvenā mehānika — 3 Ģenerāļi

**Katrs spēlētājs pastāvīgi ir ar 3 unikāliem ģenerāļiem** — nevis armija no minjoniem. Katrs ģenerālis ir stāsta figūra ar vārdu, personību, backstory, gearu, prasmju koku un Bond progresi.

Pilnas detalizētas specifikācijas skatīt: **`Generals.md`**.

### 6.1. Ģenerāļu trio katrai klasei

| Klase          | Ģenerālis 1 (Tank)     | Ģenerālis 2 (DPS)       | Ģenerālis 3 (Support/Utility)  |
|----------------|------------------------|-------------------------|--------------------------------|
| Pirmais Kungs  | **Rose** (paladin tank)| **Kalts** (dual-wield)  | **Velvet** (priestere/healer)  |
| Otrs Kungs     | **Kaspars** (heavy warrior) | **Fay** (elfu ranged mage) | **Juno** (assassin)       |
| Vecais Burvis  | **Torins** (spellblade)| **Sasla** (frost mage)  | **Vera** (teleport support)    |

Katras klases 3 ģenerāļi ir dizainēti kā **complementary trio** — kombinē tank + DPS + support role.

### 6.2. Komandēšanas mehānika

- **RMB uz mērķa** — visi 3 ģenerāļi (vai konkrētais, ja izvēlēts) uzbrūk tam
- **RMB uz vietas** — ģenerāļi pārvietojas uz punkta
- **1 / 2 / 3** — izvēlas atsevišķu ģenerāli (nākošā RMB norāda tikai viņam)
- **F** — *Fall Back!* — visi ģenerāļi atgriežas pie tevis
- **G** — *Hold Position!* — ģenerāļi paliek uz vietas
- **T** — formation cycle (triangle / line / surround / default)
- **R** (kad izvēlēts ģenerālis) — viņa Signāles Prasme

### 6.3. Bond un progresija

Katrs ģenerālis sāk **Bond 1** un aug līdz **Bond 10** caur kombinētu cīņu:
- **Bond 5:** atslēdz Ultimate Ability
- **Bond 9:** atslēdz Personal Backstory Quest
- **Bond 10:** atslēdz Legendary Perk + kosmētisks skin

### 6.4. Gear slots

Katram ģenerālim ir **3 gear slots** (Ierocis, Bruņas, Aksesuārs), kur var ekipēt full retumu spektrā līdz **klases-specifiskam Uber Mistiskajam** (9 unikāli Uber Mistiskie kopā, viens per ģenerālis).

### 6.5. Nāve un atgūšana

Ģenerāļi **nav permanent death** — knock-out ar revive pēc 30s cīņā vai instant ārpus cīņas. Player var arī revive early (F 2s pieejot pie downed ģenerāļa).

---

## 7. Cīņas sistēma

- **LMB** — tavs personiskais uzbrukums (klases specifiks).
- **RMB** — ģenerāļu komanda (visi 3 vai izvēlētais viens).
- **Space** — izvairīšanās (i-frames).
- **Q / E / R** — trīs aktīvās prasmes (klases + ekipējuma + koka).
- **Shift** — sprints.
- **Klases signāles** — signāles resurss aug caur cīņu, izlādē par klases-specifisku ultimātu.

Cīņas ritms: ne tu, ne 3 ģenerāļi viens pats netiek galā ar boss. Uzvara — pareizā trio kombu (tank agro + DPS burst + support heal/buff).

---

## 8. Loot, ekipējums un amatniecība (Diablo 4 stils)

*Nemainīgs kopš iepriekšējām versijām — skat. detalizētu specifikāciju §8.1–8.14.*

Kopsavilkumā:
- **Item Power (iP)** 1-925, atbilst spēlētāja līmenim
- **6 retumi:** Parasts / Rūdīts / Sens / Reliktu / Nolādēts / Uber Mistiskais
- **Great Affix (Lielā Zīme ★)** — ~+50% stat, 4 GA = perfekts item
- **Multiplicative damage pipeline:** Crit, Physical, DoT, Vulnerable + Reizinātāji
- **Aspekti + Kodekss:** ~80 aspekti (24 klases-specifiski jaunajā 3-klašu setup, 8 per klase)
- **Setu Komplekti:** 9 kopā (3 per klase, 2/4/6 daļas)
- **Uber Mistiskie:** ~6 (nemainīts)
- **Gemi:** 6 tipi, 7 tieri, Uber Mistiskie Gemi Ēnu slotā
- **5 amatnieki:** Kalējs (Masterworking), Dziedniece, Laboratorija (Tempering), Okultisti (Aspects), Juvelieris (gems)
- **Wardrobe** ar transmog un 6 build presets
- **Loot avoti:** Kunga Lāde pie bosiem, Elite Bosi, Pazemes Dungeoni, World Bosi, Uber Bosi

---

## 9. Progresija

- **Līmeņi 1–80** (max level 80), pēc tam atslēdzas **Paragon sistēma** ar max **350 Paragon punktiem**.
- **Prasmju koks** trīs zaros: **Kungs** (personiskais), **Ģenerāļu Sardzes** (trio buffi), **Melnais Māksla** (nolādēts).
- Katrai klasei **savs prasmju koks** ar 6–8 unikāliem "keystone" perkiem.

Detaliēzēta Paragon sistēma — skat. `ParagonBoards.md` (**needs refactor to 3 classes**).

---

## 10. Melnā Torņa cietoksnis (hub, aug)

- **Tronis** (klases-tematiska dekorācija)
- **Ģenerāļu kambari** (3, viens katram tavas klases ģenerālim)
- **Bruņotava** / **Alķīmiķa tornis**
- **Karte** (planē reidus)
- **Trofeju halle**
- **Melnais tronis** (Kunga tonu izvēles → Tirāns/Mecēns morāles ass)

---

## 11. Endgame (Diablo 4 stils)

- **Murgu Dungeoni** — grūtāki, affix mods, augsts loot.
- **Iekarojuma karte** — idle progresija.
- **Pasaules bosi**.
- **Sezonas** — jauns twist ik 3 mēneši.
- **Leaderboardi**.
- **Klašu jaudas turnīri** — kurš Kungs iet dziļāk?

---

## 12. Stāsts un morāles ass

Nav "labais vs ļaunais" — ir **kāda veida ļaunais**.
- **Ass:** Tirāns ↔ Mecēns.
- **Trīs cēlieni:** Pamošanās → Iekarošana → Impērija vai Bezdibenis.
- **4 beigas:** Melnais Imperators / Cieņā turētais Kungs / Bezdibeņa Karotājs / Klusums.

---

## 13. MVP saraksts

- **Viena klase:** Pirmais Kungs (kā vienkāršākais paradigms un balansēts).
- **Viens reģions:** Melnais Tornis (starta klases karaļvalsts).
- **Rose** (Pirmā Kunga tank ģenerālis) ar pilnu komandu sistēmu.
- Pamata cīņa + 3 aktīvās prasmes + Dominance signāles resurss.
- Divi ienaidnieku tipi + viens mini-boss (Sardzes Kapteinis Bornlejs).
- Viens dungeon (5 telpas, boss beigās).
- Melnais Tornis kā vienkāršs hub (tronis, Rose kambaris, kalējs).
- Loot ar trim retumiem.
- Klases prasmju koks (samazināts, 15 punkti).
- Level up līdz 15.

**Laiks solo:** 6–9 mēneši MVP.
**Laiks komandai (3–5):** 3–4 mēneši MVP.

---

## 14. Tehniskais stack (UE5)

- **UE 5.4+**, Lumen, Nanite.
- **C++** kodolam: `KungsCharacterBase` (abstract, klases mantojas), `MinionBase`, `MinionAIController`, `MinionCommandComponent`, `LootManager`, `ClassOathComponent`, `SaveGameSystem`.
- **Blueprints** UI, klašu prasmes, quest scripts.
- **GAS** prasmēm un statiem — labi der klašu variācijām.
- **Behavior Trees** ģenerāļu un ienaidnieku AI (katram ģenerālim savs BT).
- **Enhanced Input** vadībai.
- **Niagara** partiklu efektiem.
- **Metasounds** audio.

### Klašu arhitektūra kodā
- `AKungsCharacterBase` (abstract UE C++ klase).
- Katrai klasei: `AKungsPirmaisKungs`, `AKungsOtrsKungs`, `AKungsVecaisBurvis`.
- **`UClassOathComponent`** apstrādā signāles resursu, klases prasmes, ģenerāļu trio bonusus.
- **`AGeneralCharacter`** (abstract, 9 mantojas) — Rose, Kalts, Velvet, Kaspars, Fay, Juno, Torins, Sasla, Vera.

---

## 15. Māksla un audio

### Māksla
- **Stilizēti low-poly** ar spēcīgu apgaismojumu.
- Katrai karaļvalstij savs materiālu un krāsu profils.
- Klasēm — atšķirīgi silueti pat no isometriskā skata.

### Audio
- Klusa, minimāla mūzika ar reģionāliem instrumentiem (Melnais Tornis — ērģeles + ķēde; Nordberga — kori + akordeons; Vecais Tornis — arkānā vokāls + skanoša metallophone).
- Ģenerāļi runā (voice acted); Kunga balss izvēles brīžos. Katram no 9 ģenerāļiem savs voice actor.

---

## 16. Nākamie soļi

1. **Šis dokuments** ✅
2. **Refactor pending doks:**
   - `ParagonBoards.md` — pārrakstīt 3 klasēm (18 dēļi vietā 42)
   - `Aspects.md` — pārrakstīt klases-specifisko sadaļu (24 vietā 48)
   - `Runes.md` — atjaunināt klases-specifisko sadaļu (6 vietā 12)
   - `DamageMath-AllBuilds.md` — pārrakstīt 9 buildiem vietā 18
   - `Dungeons.md` — atjaunināt klases dungeonus (3 vietā 6)
   - `Seasons.md` — atjaunināt klases atsauces
   - `BrokenBuilds.md` — atjaunināt build atsauces
3. **BuildGuide-*.md** — jauni 3 doki, veci 6 dzēsti ✅ (šai versijā)

---

*Šis ir dzīvs dokuments. Katrs elements ir apspriežams — pastāsti, kas nepatīk, un mainām.*
