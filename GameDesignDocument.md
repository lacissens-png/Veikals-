# Design dokuments — *Kalna Kungs* (darba nosaukums)

**Žanrs:** Isometrisks action RPG ar minjonu komandēšanu un klašu izvēli
**Iedvesma:** *Overlord* (2007) × *Diablo IV*
**Engine:** Unreal Engine 5
**Skats:** Top-down / isometrisks, ar rotējamu kameru
**Platforma:** PC (Windows), vēlāk konsoles

---

## 1. Koncepts vienā teikumā

Tu izvēlies **vienu no sešiem Kunga Zvērestiem**, katrs iemieso vienu no Overlord leģendas varenākajām figūrām, un komandē četru veidu **Ēnu minjonus** cauri sešām vizuāli atšķirīgām karaļvalstīm, izlaupot loot un pārbūvējot sabrukušu pasauli.

**Overlord daļa:** minjoni ir tavs galvenais ierocis; klases balstītas uz sākotnējiem varoņiem.
**Diablo 4 daļa:** blīvs loot, prasmju koki, dungeoni, world bosi, sezonas, endgame.
**Gothic daļa:** viss ir tumšs, drēgns, sakrāls, ar melno humoru.

---

## 2. Pasaule un lore

### Pasaules lore īsumā
- Pasaule saucas **Vaireja**.
- Pirms 300 gadiem **Pirmais Kungs** valdīja no **Melnā Kalna**. Viņa armija bija sadalīta septiņās vienībās, katru vadīja **Zvērests** — reinkarnētu varoņu līnijas, kas piesaistījās vienam grēkam.
- Pirmais Kungs krita. Zvērestu līnijas izklīda un savā vietā radīja **sešas karaļvalstis**, katra pēc sava Zvērests dabas.
- Pasaule sabruka. Baznīca sāka saukt Zvērestus par "seno ļaunumu". Karaļvalstis nolaidās, korumpējās, dažas krita "gaišo" ordeņu rokās.
- Tu esi **jaunais Zvērests-mantinieks** — pamodies Melnajā Kalnā ar vienu no sešiem Zvērestu Zīmoglaukumiem plaukstā. Tavs Zvērests izvēlas, kāda veida Kungs tu esi, kāda ir tava dzimtā karaļvalsts, kādas prasmes māca asinis.

### Vizuālais stils (kopīgais)
- **Palete kodolā:** ogļu melna, asins sarkana, alvas zilā, sveču zelts.
- **Katrai karaļvalstij ir sava piesātināta akcenta krāsa** (skat. klasēs).
- **Arhitektūra:** ziemeļu gotika kā pamats, katrā valstībā ar vietējo pieskaņu.
- **Gaisma:** stipri kontrasti — Lumen dinamiska, ēnas darbojas kā otrs aktieris.

---

## 3. Klases un karaļvalstis

Katra klase ir viens no sešiem **Zvērestiem** — reinkarnētu varoņu līnija ar savu grēku, prasmēm, minjonu afinitāti un dzimto karaļvalsti. Izvēle sākumā **ir pastāvīga** vienam char slotam (kā Diablo). Katra klase vizuāli atslēdz atšķirīgu **starta reģionu** un mazliet transformē **Melno Kalnu** (troņa telpas dekorācijas, minjonu kosmētika).

### 3.1. Melnais Bruņinieks — *Kritušā Krusta Zvērests*
> Iedvesma: Sir William the Black (Overlord I) — Debesu Virsotnes paladīns, kritušais no iekāres.

- **Stils:** smaga tuvcīņa, zobens+vairogs, defence, kontrol-cīņa. Lēns, sitīgs, izturīgs.
- **Signāles prasmes:** Krusta Zvans (AoE stun), Baznīcas Vairogs (block reflekts), Asinssvētība (heal, kad nogalina).
- **Minjonu afinitāte:** **Sūkšņi** — kopā veido **falangu** (šoulder-to-shoulder ar +bruņu buff).
- **Signāles resurss:** **Grēksūdze** — uzkrājas, kad saņem sitienus; izlādē par masveida heal grupai.
- **Karaļvalsts: Debesu Virsotne (*Debesu Virsotne*)**
  - **Vizuāli:** sarkani vitrāžu logi, augstas gotikas katedrāles, mūžīgi zvanu skaņa. Balti marmora ielas, kas iekrāsojušās asins krāsā. Ceriņu mākoņi.
  - **Ienaidnieki:** apsēsti mūki, asins nunas, vampīri altāros, kritušie krustneši.
  - **Boss (reģionā):** **Bīskaps Ansels** — sešroku vampīrs pie ērģelēm.

### 3.2. Berserkers — *Arēnas Zvērests*
> Iedvesma: Kahn the Warrior (Overlord I) — barbaru varonis, kritušais no dusmām.

- **Stils:** dubultā ieroci, ātrs, mobilitāte, rage-based. Cīņā, kad saņem bojājumus, kļūst stiprāks.
- **Signāles prasmes:** Kara Kliedziens (AoE bail + rage buff), Asins Trakums (attack speed no zaudētās dzīvības), Divu Zobenu Deja.
- **Minjonu afinitāte:** **Sārtaji** — uguns tavā tuvumā = viņi šauj ātrāk.
- **Signāles resurss:** **Rage** — aug, kad saņem bojājumus; izlādē par 5-sekunžu berserker režīmu.
- **Karaļvalsts: Asins Arēna (*Asins Arēna*)**
  - **Vizuāli:** iesaulis tuksnesis, milzīga akmens amfiteātris, ķēdes žogos, sarkans smilšu putekļu vējš, dzelzs statuas kritušajiem gladiatoriem.
  - **Ienaidnieki:** klanu karavīri, dresēti zvēri, gladiatoru dvēseles, arēnas kāzu ceremoniju paliekas.
  - **Boss:** **Trīsroku Karagalvenis Talos** — bijušais Zvērests, kas nepieņem tavu mantojumu.

### 3.3. Alkas Kungs — *Zelta Zvērests*
> Iedvesma: Goldo Golderson (Overlord I) — pundurķēniņš, kritušais no alkatības.

- **Stils:** hibrīds ranged/melee, sauc **Zelta Golemus** (temporārus konstruktus par zeltu), zelts = resurss un ierocis.
- **Signāles prasmes:** Zelta Lietus (met monētas kā projektiles), Ķērājs Golems (summonis 20s), Sausā Sirds (visi drops +30% zelta, bet -20% XP).
- **Minjonu afinitāte:** **Visi** — minjoni ap tevi savāc lūtu automātiski un dubultā ātrumā.
- **Signāles resurss:** **Zelts** — patērē kā manu īpašajām prasmēm; jo bagātāks, jo stiprāks.
- **Karaļvalsts: Zelta Raktuves (*Zelta Raktuves*)**
  - **Vizuāli:** milzīga pazemes zāle, magma tālumā, zelta dzīsles griestos, verdzeņu ķēdes, pundurķēniņu troņi grabaza dzīlēs, karsts.
  - **Ienaidnieki:** verdzenieku pārraugi, zeltā vārīti līķi (goldens), kolektoru golemi, pazemes bīskapi.
  - **Boss:** **Vecais Goldo** — pats tavs sen-vec-vec-tēvs Zvērests, tagad zelta apvalkā iesalis.

### 3.4. Mežonis Kungs — *Puves Zvērests*
> Iedvesma: Oberon Greenhaze (Overlord I) — Elfu Kungs, kritušais no slinguma un aizmirstības.

- **Stils:** dabas maģija sabojāta, indes DoT (damage over time), īslaicīgi pārvēršas par zvēru, augsts area denial.
- **Signāles prasmes:** Sakņu Slazds (roots erupt no zemes), Indes Migla (AoE DoT), Vilka Āda (10s transform).
- **Minjonu afinitāte:** **Zaļči** — inde no tavām prasmēm pārceļas uz Zaļču dunčiem.
- **Signāles resurss:** **Sula** — savāc no puvušas dzīvības (nogalinātās plantas/dzīvnieki); izmanto pārveidojumam un lielām prasmēm.
- **Karaļvalsts: Puvušais Mežs (*Puvušais Mežs*)**
  - **Vizuāli:** kaltu koku ēnas, milzīgas sēnes gaismos ceriņi/violeti, animētie sakņu tīkli, ledus migla, aizmirsti elfu tempļi apēsti zaļās puves.
  - **Ienaidnieki:** kaltušas dryādes, puves lāči, elfu ghouli, ķērājoši augi, aizmirsti mednieki.
  - **Boss:** **Elfu Karaliene Ninhera** — puse sieva, puse koks; ēna savā tumšajā birzī.

### 3.5. Bezēnu Zaglis — *Nakts Zvērests*
> Iedvesma: Jewel the Thief (Overlord I) — Zaglis, kritušais no skaudības.

- **Stils:** stealth, backstab, ātri kritiskie sitieni, teleportācija, augsts single-target burst.
- **Signāles prasmes:** Ēnu Solis (teleports aiz mērķa + 3s invis), Naktsāda (kloāka ar automātisku backstab pirmajā sitienā), Kaklaslazds (dubultkritisks garantēts).
- **Minjonu afinitāte:** **Zaļči** — kļūst neredzami krūmos; backstab tavai grupai dubultbojājums.
- **Signāles resurss:** **Ēna** — uzkrājas, kad slepen slēpies vai kad kritušie neredz, izmanto lielām prasmēm.
- **Karaļvalsts: Tuksneša Bāzars (*Tuksneša Bāzars*)**
  - **Vizuāli:** oāzes pilsēta smilšu jūras vidū, purpurs zīds karājas starp šauri ielām, milzīgas smilšakmens torņi, karstums, slēptas tirgu ejas, ēnu ceriņu debesis vakarā.
  - **Ienaidnieki:** trīnu zagļi, čūsku burvji, kamielotu jātnieki, muzikanti-slepkavas, alķīmiķi.
  - **Boss:** **Melnās Kūkas Vezirs** — mūžsens karaliskais slepkava, 12 dunči, 3 ēnu-dvīņu klonis.

### 3.6. Vientuļais Sniegurgs — *Ledus Zvērests*
> Iedvesma: The Wizard (Overlord I torņa varonis) — Burvis, kritušais no lepnības.

- **Stils:** distances maģija, ledus + nekromantija, summonis skeletus no līķiem, area kontrole.
- **Signāles prasmes:** Sasalis Kaps (freeze area + līķi kļūst par skeletiem), Ledus Lauskas (kanālēts nuke), Torņa Redzējums (redzi visu karti 10s + slow ienaidniekiem).
- **Minjonu afinitāte:** **Zilspāri** — dubultā burvestību jauda, spēj celt vienreizējus skeletu sabiedrotos no jebkuriem līķiem.
- **Signāles resurss:** **Sala** — auksts uzkrājas ar katru zaudēto dzīvību un ledus prasmi; izlādē par mega freeze.
- **Karaļvalsts: Ledus Kroņi (*Ledus Kroņi*)**
  - **Vizuāli:** peldoši ledus salu fragmenti augstu debesīs, sasals dievnams, mēle balta gaisma, skeletu sardze aiz sasala vārtiem, mūžīgs sniegs, zils tumšs pavadonis debesīs.
  - **Ienaidnieki:** sasalusi ordeņa gvarde, ledus lauva, kritušie mācekļi, skeleta ērģelnieks, ledus vīnjkāzas.
  - **Boss:** **Ievas Turaņa** — savs veidols laikā, kad viņa bija vēl dzīva burve; pusskeletons pusledus.

---

## 4. Klašu izvēles ietekme uz pasauli

**Klases izvēle spēles sākumā maina:**

1. **Starta reģions** — tu sāc savas klases karaļvalstī (nevis universālā tutoriālā).
2. **Melnais Kalns hub** — troņa telpa ir dekorēta pēc tavas klases (Bruņinieka gadījumā — asins vitrāža; Alkas — zelta gulta; Bezēnu — melnas zīda drapērijas u.c.).
3. **Minjonu kosmētika** — tava klase iedod minjoniem klases-tematiskus lūkus (Berserker Sūkšņiem ir sarkanas kara krāsas u.c.).
4. **Klases quest līnija** — īpaša stāsta arka par tavas Zvērestu līnijas izcelšanos, kas notiek paralēli galvenajam sižetam.
5. **Klases boss** — sava reģiona boss ir bijušais tavs Zvērests, kurš atsakās tevi atzīt.
6. **Dialogu opcijas** — NPC reaģē citādi (Bāzara tirgotāji Bezēnu Zaglim iedod atlaides, Bruņiniekam — bīstas u.c.).

Bet **visas 6 karaļvalstis apmeklē visi** — vienkārši savā secībā, ar savu perspektīvu. Diablo 4 stila open world; tikai starta reģions atšķiras.

---

## 5. Kopīgais septītais reģions

- **Melnais Kalns** — hub un fināla reģions. Nav klases-specifisks, aug pēc katras klases pieskaņas.

Kopā: **1 hub + 6 klašu karaļvalstis = 7 reģioni**.

---

## 6. Galvenā mehānika — Minjonu komandēšana

### Četru veidu minjoni (Overlord DNS ar gotisku pieskaņu)

| Tips        | Krāsa      | Loma                    | Klase, kas viņus stiprina lielākoties |
|-------------|------------|-------------------------|----------------------------------------|
| **Sūkšņi**  | Brūni      | Tuvcīņa, tanks          | Melnais Bruņinieks (falanga)           |
| **Sārtaji** | Sarkani    | Distance, uguns         | Berserkers (uguns + rage)              |
| **Zaļči**   | Zaļi       | Zaglība, inde           | Bezēnu Zaglis / Mežonis (backstab+DoT) |
| **Zilspāri**| Zili       | Maģija, dziedināšana    | Vientuļais Sniegurgs (necro-maģija)    |

**Alkas Kungs** ir izņēmums — nav afinitātes ar vienu tipu, bet **visi tipi** ap viņu savāc lūtu ātrāk.

Sākumā tev pieejami tikai **Sūkšņi**. Katrus citus atklāj, atgriežot minjonu bedri no izmirušas (viena bedre uz reģionu, tātad 4 no 6 reģioniem dod jaunu tipu; pārējie divi dod klases prasmju punktus).

### Komandēšanas mehānika
- **RMB uz vietas** — *"Ejiet tur!"* — minjoni skrien uz punktu.
- **RMB uz mērķa** — *"Uzbrūkiet!"* — koncentrējas uz vienu ienaidnieku vai objektu.
- **Shift + RMB** — *"Turieties tālāk"* — minjoni atkāpjas pie tevis.
- **Q / E / R / T** — pārslēdz aktīvo minjonu tipu, kuriem sekot pavēlei.
- **F** — *"Savāc!"* — minjoni lasa loot norādītajā vietā.
- **G** — *"Ekipējieties!"* — minjons paceļ ieroci/bruņas no zemes un lieto pats.

### Minjonu limits
- Sākumā: **10 minjoni** kopā.
- Uzlabojot Melno Kalnu: līdz **30 minjoniem**.
- Minjoni mirst. Audzē jaunus par savāktu **Dvēseļu enerģiju**.

---

## 7. Cīņas sistēma

- **LMB** — tavs personiskais uzbrukums (klases specifiks).
- **RMB** — minjonu komanda.
- **Space** — izvairīšanās (i-frames).
- **Q / E / R** — trīs aktīvās prasmes (klases + ekipējuma + koka).
- **Shift** — sprints.
- **Klases signāles** — signāles resurss aug caur cīņu, izlādē par klases-specifisku ultimātu.

Cīņas ritms: ne tu, ne minjoni viens pats netiek galā ar boss. Uzvara — pareizā komandu kombinācija.

---

## 8. Loot un ekipējums (Diablo 4 stils)

### Retums
- **Parasts** (pelēks)
- **Rūdīts** (zils) — 1 affix
- **Sens** (violets) — 2–3 affixi
- **Reliktu** (zelts, "legendary") — unikāls power
- **Nolādēts** (asins sarkans, "unique") — power ar sāpēm

### Slotes
Spēlētājs: galva, rumpis, rokas, kājas, ierocis, sekundārais, 2× gredzens, amulets = **9 slotes**.
Katrs minjons: **3 slotes** (ķivere, ierocis, aksesuārs).

### Klasei-specifiski legendāri
Katrai klasei ir **~8 klases-eksklīvi legendāri** ar tikai tai domātām saliktām spēlēm (piem., Berserker "Divi Zobeni Vienā" — pēc katra hita 5% chance uzsvērums cīņas laikā).

---

## 9. Progresija

- **Līmeņi 1–80** (max level 80), pēc tam atslēdzas **Paragon sistēma** ar max **350 Paragon punktiem**.
- **Prasmju koks** trīs zaros: **Kungs** (personiskais), **Barvedis** (minjoni), **Melnais Māksla** (nolādēts).
- Katrai klasei **savs prasmju koks** ar 6–8 unikāliem "keystone" perkiem.

---

### 9.1. Paragon sistēma (Diablo 4 stils)

**Atslēdzas:** Kad sasniedz līmeni **50**. Pēc tam katra iegūta XP porcija turpina dot **līmeņus** līdz 80. Sasniedzot 80, visa turpmākā XP dod **Paragon punktus**. Max **350 Paragon punktus** vienam varonim.

**Kur tos ieguldīt:** klases **Paragon Dēļos** (*Paragon Boards*) — vizuāli lielos 21×21 režģa dēļos ar ceļiem, kas savieno mezglus.

#### Paragon Dēļi (Boards)

Katrai klasei ir **1 sākuma dēlis** (klases emblēma centrā) un **~10 pieejamie papildu dēļi**, no kuriem izmanto **5**. Papildu dēlis pievienojas caur **Vārtu Mezglu** (Gate Node) uz sākuma dēļa malas — vari **rotēt** to par 0°/90°/180°/270°, mainot, kā tā mezgli sakrīt ar tavu ceļu.

Kopīgi tematiskie dēļi (visām klasēm pieejami, bonusi mainās pēc klases):

| Dēlis                 | Fokuss                                      |
|-----------------------|---------------------------------------------|
| **Zvērests**          | Sākuma dēlis, klases identitāte, balansēts  |
| **Karagājienis**      | Ofensīvs — bojājums, kritiskais, tempo     |
| **Tvirtīgums**        | Defensīvs — dzīvība, bruņas, resists       |
| **Barvedis**          | Minjonu buffi — skaits, spēks, DoT         |
| **Nolādēts**          | Riska/atlīdzības — spēcīgi, ar cenu        |
| **Troņa Māksla**      | Utility — cooldown, resurss, kustība       |
| **Melnais Kaisms**    | Klases-eksklīvs 6. dēlis, atslēdzas kampaņas fināla |

Katram dēlim ir **~80 mezgli** kopā. Lai sasniegtu izejas **Vārtu Mezglu**, jāizceļo caur **60–70 mezgliem** (atkarībā no ceļa). Ar **350 punktiem** vidēji aizpildi **5 pilnus dēļus** (~70 punkti katrs).

#### Mezglu tipi

| Tips           | Krāsa       | Efekts                                                                                     |
|----------------|-------------|--------------------------------------------------------------------------------------------|
| **Parasts**    | Balts       | Neliels stats: **+5 primārā stats** vai **+2% minoritārs bonuss**                          |
| **Maģisks**    | Zils        | Vidējs stats: **+10 primārā stats** vai **+3% bojājums pret ienaidnieku tipu**             |
| **Rets**       | Dzeltens    | Liels bonuss + **statistiku slieksni**: piem., *"+15% Sūkšņu bojājums; ja tev ir 300+ Spēka, papildu +15%"* |
| **Leģendārs**  | Zelts       | **Viens uz dēli**, klases-specifisks, spēles-maiņas efekts                                 |
| **Vārtu**      | Sudrabs     | Uz dēļa malas — pievieno tur nākošo dēli                                                  |
| **Rūnu ligzda**| Purpurs     | Vieta, kur ievieto **Rūnu** (skat. tālāk); dod area-of-effect bonusu ap sevi              |

Katrā dēlī: **~50 parasti**, **~20 maģiski**, **~6 reti**, **1 leģendārs**, **1 rūnu ligzda**, **1–4 vārti** malās.

#### Leģendārie mezgli (piemēri pa klasēm)

- **Melnais Bruņinieks** — *"Krustnesis"*: kad iedarbini Krusta Zvanu, visi Sūkšņi tuvumā iegūst 5s neatgriezenības un dubulto uzbrukumu bojājumu.
- **Berserkers** — *"Nekad Vairs Mierā"*: Rage nekad neizzūd cīņas laikā; kritums no dzīvības zem 30% iedarbina 3s berserker automātiski.
- **Alkas Kungs** — *"Zelts Ir Asinis"*: 20% no savāktā zelta konvertējas uz temporāro dzīvību (uzkrājas līdz 2× max dzīvība).
- **Mežonis Kungs** — *"Puve Nekad Nemirst"*: inde no tavām prasmēm spread uz 2 tuvākajiem ienaidniekiem, kad mērķis nomirst.
- **Bezēnu Zaglis** — *"Divi Ēnu Klonis"*: Ēnu Solis atstāj ilūziju, kas veic vienu automātisku sitienu.
- **Vientuļais Sniegurgs** — *"Sasala Kronis"*: sasaldētie ienaidnieki, saplīstot, dod +10 Sala un ceļ vājo skeletu.

#### Rūnas (mūsu Glyphs)

**Rūnas ir mazi gotiski simboli**, kurus ievieto Paragon Dēļa **Rūnu ligzdās**. Katra Rūna:
- Ir **klases-agnostika** (var izmantot jebkurš) vai **klases-specifiska**.
- Ir **līmenis 1–21** (max 21). Sāk 1, augst caur **Murgu Dungeoniem**.
- Ir **rādiuss** (2×2 → 4×4 → 6×6 mezgli ap ligzdu), kas aug ar līmeni.
- Dod bonusu pēc **mezglu tipa** rādiusā — piem., *"Katrs Maģiskais mezgls tavā rādiusā dod +2% Sūkšņu bojājumu"*.
- Dod **papildu bonusu**, ja **stats sliekšnis** rādiusā ir sasniegts — piem., *"Ja tavā rādiusā ir 40+ Veiklības, bonuss × 2"*.

**Rūnu piemēri:**

| Rūna              | Efekts                                                                          |
|-------------------|---------------------------------------------------------------------------------|
| **Ģerbonis**      | Katrs Rets mezgls rādiusā dod +3% pret elementārajiem bojājumiem                |
| **Asinssaite**    | Katrs Maģisks mezgls rādiusā dod +2% Barveža bojājumu                          |
| **Slēgtais Vārds**| Ja rādiusā ir 100+ Prāta, prasmju cooldown -15%                               |
| **Zvana Skaņa**   | Katrs Parasts mezgls rādiusā dod +1% kritiskā sitiena šansei                   |
| **Nolādētais Sirdsdedzis** | Rūna arī iznīcina 1% max dzīvības 5s cīņas laikā, bet dubulto visus rādiuss bonusus |

**Rūnu iegūšana:**
- Pirmās 5 Rūnas — no galvenā stāsta bosiem (viena uz reģionu).
- Pārējās (līdz ~30) — random dropi no **Murgu Dungeoniem** (skat. Endgame).

**Rūnu līmeņa celšana:**
- Katra pabeigta Murgu Dungeon iedod **XP** izvēlētajai Rūnai (ne visām).
- Tier XP: **Murgu D. Tier 1** = maza XP, **Tier 100** = milzīga XP. Kāpumi eksponenciāli.
- Level 15 → 21 prasa augsto tieru dungeonus (endgame grinds).

#### Paragon ekonomija — kā tas jūtas

- **Pirmie 50 punkti (līdz 130 kopā):** aizpildi sākuma dēli, sāc pirmo papildu dēli.
- **150–200 punkti:** trešā dēļa ceļš, sāk parādīties spēles-maiņas leģendārie mezgli.
- **250–300 punkti:** ceturtā/piektā dēļa optimizācija, Rūnu min-maxing.
- **300–350 punkti:** perfekcijas grind — pēdējie Rūnu līmeņi, min-max ceļu maiņa (kombinācijas).

**Cīņa nemainās pēc jaudas skaitliski (nav jautri), bet pēc kombinācijām un buildu identitātēm.** Diablo 4 stila **build diversity** — divi Berserkeri ar 300 Paragon punktiem izskatās un jūtas pilnīgi atšķirīgi.

#### Respec (pārtīrīšana)

- **Bezmaksas** pa mezglam pa mezglam (nomainīt vienu punktu = 100 zelta agrīnā spēlē, aug ar līmeni).
- **Pilna Paragon respec** — vienu reizi nedēļā bezmaksas, tad par pieaugošu **Dvēseļu enerģijas** cenu.
- Klases prasmju koka respec ir atsevišķs, dārgāks.

---

## 10. Melnā Kalna cietoksnis (hub, aug)

Nemainīgs kopš iepriekšējās versijas:
- **Tronis** (klases-tematiska dekorācija)
- **Minjonu bedres**
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

- **Viena klase:** Melnais Bruņinieks (kā vienkāršākais paradigms).
- **Viens reģions:** Debesu Virsotne.
- **Sūkšņu** minjoni ar pilnu komandu sistēmu.
- Pamata cīņa + 3 aktīvās prasmes + Grēksūdzes signāles resurss.
- Divi ienaidnieku tipi + viens mini-boss (Bīskapa novices).
- Viens dungeon (5 telpas, boss beigās).
- Melnais Kalns kā vienkāršs hub (tronis, minjonu bedre, kalējs).
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
- **Behavior Trees** minjonu un ienaidnieku AI.
- **Enhanced Input** vadībai.
- **Niagara** partiklu efektiem.
- **Metasounds** audio.

### Klašu arhitektūra kodā
- `AKungsCharacterBase` (abstract UE C++ klase).
- Katrai klasei: `AKungsMelnaisBrunininks`, `AKungsBerserkers`, `AKungsAlkasKungs`, `AKungsMezonisKungs`, `AKungsBezenuZaglis`, `AKungsVientulaisSniegurgs`.
- **`UClassOathComponent`** apstrādā signāles resursu, klases prasmes, minjonu afinitātes bonusus.

---

## 15. Māksla un audio

### Māksla
- **Stilizēti low-poly** ar spēcīgu apgaismojumu.
- Katrai karaļvalstij savs materiālu un krāsu profils (skat. sadaļu 3).
- Klasēm — atšķirīgi silueti pat no isometriskā skata (Bruņinieks blīvs+kvadrāts; Zaglis šauras+ass; Sniegurgs slaids+garš mētelis u.c.).

### Audio
- Klusa, minimāla mūzika ar reģionāliem instrumentiem (Debesu Virsotne — ērģeles; Arēna — bungas; Zelta Raktuves — pundurķēniņu kolieris; Puvušais Mežs — īsti koku vaidoni; Bāzars — čūskas svilpes+string; Ledus Kroņi — vientuļa vijole).
- Minjoni murmina; Kunga balss izvēles brīžos.

---

## 16. Nākamie soļi

1. **Šis dokuments** ✅
2. **Klašu jaudas balansa melnraksts** — tabulas ar statisko sākuma bāzi.
3. **Vienas klases (Bruņinieka) pilns prasmju koks** — kā etalons.
4. **C++ arhitektūra:** `AKungsCharacterBase` + `AKungsMelnaisBrunininks` + `UClassOathComponent` skelets.
5. **Sākuma aina** — spēlētājs + 3 minjoni + 1 ienaidnieks Debesu Virsotnes gotiskā vitrāžas telpā.
6. **UI melnraksts** klases izvēles ekrānam.

---

*Šis ir dzīvs dokuments. Katrs elements ir apspriežams — pastāsti, kas nepatīk, un mainām.*
