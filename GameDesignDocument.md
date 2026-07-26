# Design dokuments — *Kalna Kungs* (darba nosaukums)

**Žanrs:** Isometrisks action RPG ar minjonu komandēšanu un klašu izvēli
**Iedvesma:** *Overlord* serialā (I, II, Raising Hell, Dark Legend, Fellowship of Evil) × *Diablo IV*
**Engine:** Unreal Engine 5
**Skats:** Top-down / isometrisks, ar rotējamu kameru
**Platforma:** PC (Windows), vēlāk konsoles

---

## 1. Koncepts vienā teikumā

Tu izvēlies **vienu no trīs Kungiem** — visstiprākos varoņus visā Overlord serialā (divi bijušie kungi + arch-mage) — un komandē četru veidu **Ēnu minjonus** cauri trīs vizuāli atšķirīgām karaļvalstīm, izlaupot loot un pārbūvējot sabrukušu pasauli.

**Overlord daļa:** minjoni ir tavs galvenais ierocis; klases balstītas uz Overlord serialā ikoniskākajām figūrām.
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

Katra klase ir viens no trim **Kunga Zīmoglaukumiem** — reinkarnētu varoņu līnija ar savu mantojumu, prasmēm, minjonu afinitāti un dzimto karaļvalsti. Izvēle sākumā **ir pastāvīga** vienam char slotam (kā Diablo). Katra klase vizuāli atslēdz atšķirīgu **starta reģionu** un mazliet transformē **Melno Torni** (troņa telpas dekorācijas, minjonu kosmētika).

### 3.1. Pirmais Kungs — *Pirmā Zīmoglaukuma Mantinieks*
> Iedvesma: **The First Overlord** (Overlord I protagonists) — pamodinātais no minjoniem, sakāva visus 7 varoņus, valdīja no Melnā Torņa.

- **Stils:** balansēts warrior ar spēcīgu minjonu komandu. Vidēja tuvcīņa + minjonu armija.
- **Vienā teikumā:** *"Es esmu pirmais un man visi kalpo."*
- **Signāles prasmes:** Kunga Sitiens (LMB melee ar minion buff), Master's Call (AoE minion command + buff), Domination Aura (paranormāla klātbūtne, minjoni +30% dmg), Kunga Zvans (100pt ult: visi minjoni pēkšņi max buffed, tu iegūsti barjeru).
- **Minjonu afinitāte:** **Visi 4 tipi** — Pirmais Kungs komandē tos visus vienlīdz labi. Katrs tips ap viņu iegūst +15% dmg un +10% AS.
- **Signāles resurss:** **Dominance** — uzkrājas ar minjonu kills (3 pt / kill) un tavu personīgo kills (5 pt / kill). Izzūd 1 pt/sec ārpus cīņas.
- **Karaļvalsts: Melnais Tornis (*Melnais Tornis*)**
  - **Vizuāli:** kolosāls gotiskais tornis melnākajā mežā. Iekšā — visu 4 minjonu bedres, kolīdes valodas, un troņa zāle ar Pirmā Kunga bijušajām relikvijām.
  - **Ienaidnieki:** kritušie krustneši, sardzes, apsēsti mūki, iebrukumi no gaisajiem ordeņiem.
  - **Boss:** **Sardzes Kapteinis Bornlejs** — pēdējais gaisais paladins, kurš vēl mēģina Melno Torni "attīrīt".

### 3.2. Otrs Kungs — *Otrā Zīmoglaukuma Mantinieks*
> Iedvesma: **The Second Overlord** (Overlord II protagonists) — pirmā dēls, iekaroja Nordberga un Glorius Empire, hibrīds warrior-mage.

- **Stils:** hibrīds tuvcīņa + tumšas maģijas prasmes. Domina lielas grupas, atslēdz karus.
- **Vienā teikumā:** *"Mans tēvs bija pirmais. Es esmu labākais."*
- **Signāles prasmes:** Impēriskais Sitiens (LMB, kombinē fizisko + shadow), Melnās Zibenis (Q, chain lightning ar shadow), Impērisks Zvans (E, buff apkārtējiem minjoniem +50% dmg 8s), Impērija Vaidēs (100pt ult: masveida shadow AoE + tu iegūsti maģisku formu 15s).
- **Minjonu afinitāte:** **Sūkšņi + Zilspāri** (Overlord 2 stils — brutālais warrior + spellcaster). Sūkšņi ap viņu iegūst +25% dmg; Zilspāri iegūst +50% spell power.
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
- **Minjonu afinitāte:** **Zilspāri** (spellcaster minjoni). Ap Vecajo Burvi Zilspāri iegūst **triple spell power** un **50% cast speed**. Var pat īslaicīgi mainīt reality (teleport uz tevi vai zibens hits).
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
3. **Minjonu kosmētika** — tava klase iedod minjoniem klases-tematiskus lūkus.
4. **Klases quest līnija** — īpaša stāsta arka par tavas mantojumu, kas notiek paralēli galvenajam sižetam.
5. **Klases boss** — sava reģiona boss ir bijušais tavs zīmoglaukuma turētājs, kurš atsakās tevi atzīt.
6. **Dialogu opcijas** — NPC reaģē citādi (imperiskais Otrs Kungs dabū saluts no ordeņa paliekas, Burvis dabū baiļu skatienus u.c.).

Bet **visas 3 karaļvalstis apmeklē visi** — vienkārši savā secībā, ar savu perspektīvu.

---

## 5. Kopīgais ceturtais reģions

- **Melnais Kalns** — hub un fināla reģions. Nav klases-specifisks, aug pēc katras klases pieskaņas.

Kopā: **3 klašu karaļvalstis + 1 hub Melnais Kalns = 4 reģioni**.

---

## 6. Galvenā mehānika — Minjonu komandēšana

### Četru veidu minjoni (Overlord DNS ar gotisku pieskaņu)

| Tips        | Krāsa      | Loma                    | Klase, kas viņus stiprina lielākoties |
|-------------|------------|-------------------------|----------------------------------------|
| **Sūkšņi**  | Brūni      | Tuvcīņa, tanks          | Pirmais Kungs (all), Otrs Kungs        |
| **Sārtaji** | Sarkani    | Distance, uguns         | Pirmais Kungs (all)                    |
| **Zaļči**   | Zaļi       | Zaglība, inde           | Pirmais Kungs (all)                    |
| **Zilspāri**| Zili       | Maģija, dziedināšana    | Otrs Kungs, Vecais Burvis (kritiskā)   |

**Pirmais Kungs** ir vienīgais, kas komandē visus 4 tipus vienlīdz labi. Otrs Kungs un Vecais Burvis fokusējas uz konkrētiem tipiem.

Sākumā tev pieejami tikai **Sūkšņi**. Katrus citus atklāj, atgriežot minjonu bedri no izmirušas (bedres ir Melnajā Tornī, atslēdzas ar reģiona bosu clearing).

### Komandēšanas mehānika
- **RMB uz vietas** — *"Ejiet tur!"* — minjoni skrien uz punktu.
- **RMB uz mērķa** — *"Uzbrūkiet!"* — koncentrējas uz vienu ienaidnieku vai objektu.
- **Shift + RMB** — *"Turieties tālāk"* — minjoni atkāpjas pie tevis.
- **Q / E / R / T** — pārslēdz aktīvo minjonu tipu, kuriem sekot pavēlei.
- **F** — *"Savāc!"* — minjoni lasa loot norādītajā vietā.
- **G** — *"Ekipējieties!"* — minjons paceļ ieroci/bruņas no zemes un lieto pats.

### Minjonu limits
- Sākumā: **10 minjoni** kopā.
- Uzlabojot Melno Torni: līdz **30 minjoniem** (Pirmais Kungs) vai **25** (Otrs Kungs) vai **20** (Vecais Burvis — glass cannon fokuss).
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
- **Prasmju koks** trīs zaros: **Kungs** (personiskais), **Barvedis** (minjoni), **Melnais Māksla** (nolādēts).
- Katrai klasei **savs prasmju koks** ar 6–8 unikāliem "keystone" perkiem.

Detaliēzēta Paragon sistēma — skat. `ParagonBoards.md` (**needs refactor to 3 classes**).

---

## 10. Melnā Torņa cietoksnis (hub, aug)

- **Tronis** (klases-tematiska dekorācija)
- **Minjonu bedres** (4, atslēdzas ar reģionu clearing)
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
- **Sūkšņu** minjoni ar pilnu komandu sistēmu.
- Pamata cīņa + 3 aktīvās prasmes + Dominance signāles resurss.
- Divi ienaidnieku tipi + viens mini-boss (Sardzes Kapteinis Bornlejs).
- Viens dungeon (5 telpas, boss beigās).
- Melnais Tornis kā vienkāršs hub (tronis, minjonu bedre, kalējs).
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
- Katrai klasei: `AKungsPirmaisKungs`, `AKungsOtrsKungs`, `AKungsVecaisBurvis`.
- **`UClassOathComponent`** apstrādā signāles resursu, klases prasmes, minjonu afinitātes bonusus.

---

## 15. Māksla un audio

### Māksla
- **Stilizēti low-poly** ar spēcīgu apgaismojumu.
- Katrai karaļvalstij savs materiālu un krāsu profils.
- Klasēm — atšķirīgi silueti pat no isometriskā skata.

### Audio
- Klusa, minimāla mūzika ar reģionāliem instrumentiem (Melnais Tornis — ērģeles + ķēde; Nordberga — kori + akordeons; Vecais Tornis — arkānā vokāls + skanoša metallophone).
- Minjoni murmina; Kunga balss izvēles brīžos.

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
