# Rūnu Katalogs

*Visu 30 Rūnu (Glyphu) pilna specifikācija ar efektiem, sliekšņiem, drop avotiem un build ieteikumiem.*

Pamata Rūnu sistēma — skat. `GameDesignDocument.md` §9.1 un `ParagonBoards.md` §1.1.

---

## 1. Ievads

Rūnas ir mazi gotiski simboli, ko ievieto Paragon dēļu **Rūnu ligzdās**. Katra Rūna:

- Ir **klases-agnostika** (var izmantot jebkurš) vai **klases-specifiska**.
- Ir **līmenis 1–21** (max 21).
- Ir **rādiuss** (aug ar līmeni), kurā ietekmē tuvos Paragon mezglus.
- Dod bonusu pēc **mezglu tipa** vai **klases mehānikas** rādiusā.
- Var dot **papildu bonusu**, ja **stat sliekšņa** rādiusā ir sasniegts.

### 1.1. Rādiuss aug ar līmeni

| Rūnas Lv | Rādiuss     | Mezglu skaits rādiusā (vidēji) |
|----------|-------------|---------------------------------|
| 1        | 2×2         | ~4                              |
| 5        | 3×3         | ~9                              |
| 10       | 4×4         | ~16                             |
| 15       | 5×5         | ~25                             |
| 21       | 6×6         | ~36                             |

### 1.2. Drop avoti (kopīgi)

| Avots                          | Kas dropo                                        |
|--------------------------------|---------------------------------------------------|
| **Galvenā stāsta bosi**        | Pirmās 6 Rūnas (viena uz reģionu, klases-agnostika) |
| **Reģiona sekundārie bosi**    | Klases-agnostiskas Rūnas Lv 1                    |
| **Klases boss** (tavas klases Zvērests) | Tavas klases-specifiskā Rūna Lv 1        |
| **Murgu Dungeoni T1–T20**      | Random klases-agnostiskas Rūnas + Lv XP           |
| **Murgu Dungeoni T20–T60**     | Retākas Rūnas + upgrade materials + Lv XP        |
| **Murgu Dungeoni T60–T100**    | Legendārās Rūnas + Uber Rūnas + Lv XP eksponenciāli |
| **World Bosi**                 | Rets drops uz **Uber Rūnu** (~5% chance)         |

### 1.3. Upgrade cost (līmeņi)

| Lv       | XP prasība                | Materiāli                          |
|----------|---------------------------|------------------------------------|
| 1 → 5    | 1 Murgu Dungeon T1 katrs  | 500 zelta                          |
| 5 → 10   | 1 T10 katrs               | 5 000 zelta                        |
| 10 → 15  | 1 T30 katrs               | 25 000 zelta + Melnā Rune          |
| 15 → 20  | 1 T60 katrs               | 100 000 zelta + Zvana Sudrabs      |
| 20 → 21  | 1 T100 katrs              | 500 000 zelta + Zvaigznes Skabarga |

Pilnīga Rūna no 1 → 21 = **~40 Murgu Dungeon runs** vidēji 5-15 min = **~10-15 stundas per Rūna**. Endgame grinds.

---

# 2. Universālās Rūnas (18)

## 2.1. Node-Type Focused (5)

### #1. **Zvana Skaņa**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** Katrs **Parasts** mezgls rādiusā = +1% Crit Chance.
**Max (Lv 21):** Katrs **Parasts** mezgls rādiusā = +2% Crit Chance.

**Slieksnis:** Ja rādiusā ir 20+ Parasti mezgli, papildu +5% Crit Chance kopīgi.

**Drop:** Elite bosi jebkurā dungeon T1+.

**Ieteicamie buildi:** jebkuram, kas skaita uz crit — Vecais Burvis Realitātes Karaļvalsts, Vecais Burvis Torņa Ass, Otrs Kungs Hybrid Balance.

---

### #2. **Asinssaite**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** Katrs **Maģisks** mezgls rādiusā = +2% Barveža (minjonu) dmg.
**Max (Lv 21):** Katrs **Maģisks** mezgls rādiusā = +4% Barveža dmg.

**Slieksnis:** Ja rādiusā ir 10+ Maģisks mezgli, minjoni +10% AS.

**Drop:** Reģiona bosi (Bornlejs, Solariuss).

**Ieteicamie buildi:** minjonu buildi — Pirmais Kungs Meistars pāri Visiem, Pirmais Kungs Kunga Impērija, Vecais Burvis Zilspāri Storm.

---

### #3. **Ģerbonis**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** Katrs **Rets** mezgls rādiusā = +2% pret elementārajiem bojājumiem.
**Max (Lv 21):** Katrs **Rets** mezgls rādiusā = +5% pret elementārajiem bojājumiem un +2% dmg.

**Slieksnis:** Ja rādiusā ir 4+ Reti mezgli, viens papildu Rets mezgls "aktivējas" (dubultā bāzes efekts).

**Drop:** Murgu Dungeoni T15+.

**Ieteicamie buildi:** Pirmais Kungs Sardzes Zvērests (defensīvs), Otrs Kungs Permanent Empress.

---

### #4. **Kroņa Roka**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** Ja **Leģendārais** mezgls rādiusā, tā efekts +10%.
**Max (Lv 21):** Ja **Leģendārais** mezgls rādiusā, tā efekts +40%.

**Slieksnis:** Ja rādiusā ir arī 3+ Reti mezgli, Leģendāra efekts +papildu 20%.

**Drop:** Murgu Dungeoni T30+.

**Ieteicamie buildi:** visi endgame — būtiski nostiprināt Kritušā Krusta / Trakuma Zvēresti u.c. keystone efektus.

---

### #5. **Mezglu Kalums**

**Tips:** Universāla
**Rarity:** Leg (leģendāra)

**Bāzes (Lv 1):** Rādiuss tiek pielīdzināts ar +1 kārtu (Lv 1 = 3×3 vietā 2×2).
**Max (Lv 21):** Rādiuss +2 kārtas + katrs mezgls rādiusā dod +1% dmg neatkarīgi no tipa.

**Slieksnis:** Ja rādiusā ir mezgli no visiem 4 tipiem (Parasts, Maģisks, Rets, Leģendārs), viss dmg +10%.

**Drop:** Uber Bosi (rets).

**Ieteicamie buildi:** min-max endgame visiem.

---

## 2.2. Elementāra (5)

### #6. **Ugunslīnija**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** Fire dmg +5%.
**Max (Lv 21):** Fire dmg +25%.

**Slieksnis:** Ja rādiusā ir 4+ Uguns/Physical mezgli, papildu +15% Fire dmg.

**Drop:** Nordberga reģiona bosi (Solariuss).

**Ieteicamie buildi:** Otrs Kungs Hybrid Balance (Sūkšņi + Zilspāri sync).

---

### #7. **Sasala Zīme**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** Frost dmg +5%.
**Max (Lv 21):** Frost dmg +25%.

**Slieksnis:** Ja rādiusā ir 4+ Frost/Ledus mezgli, freeze ilgums +30%.

**Drop:** Vecais Tornis reģiona bosi.

**Ieteicamie buildi:** Vecais Burvis visi 3 buildi (frost + arkānais sinergs).

---

### #8. **Zibens Solis**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** Lightning dmg +5%.
**Max (Lv 21):** Lightning dmg +25%.

**Slieksnis:** Ja rādiusā ir 4+ Lightning mezgli, uzbrukumi 5% chance ķēdē uz 2 tuvākiem mērķiem.

**Drop:** Nordberga reģiona bosi (Solariuss).

**Ieteicamie buildi:** Otrs Kungs Chain Lightning (Melnās Zibens ķēdes).

---

### #9. **Ēnu Zīme**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** Shadow dmg +7%.
**Max (Lv 21):** Shadow dmg +30%.

**Slieksnis:** Ja rādiusā ir Ēna vai Shadow mehānika mezgls, tavas prasmes uzliek Vulnerable 4s.

**Drop:** Bezēnu Zagļa reģiona boss (Solariuss) — arī pieejams Murgu T30+.

**Ieteicamie buildi:** Vecais Burvis visi 3 buildi (Shadow spell sinergs).

---

### #10. **Svētais Vaids**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** Holy dmg +7%.
**Max (Lv 21):** Holy dmg +30%.

**Slieksnis:** Ja rādiusā ir 3+ Ticības mezgli, prasmes dziedina tevi 2% HP per hit.

**Drop:** Melnais Tornis reģiona boss (Bornlejs).

**Ieteicamie buildi:** Pirmais Kungs Sardzes Zvērests (holy sinergs).

---

## 2.3. Damage-Type Focused (4)

### #11. **Kritušā Vaids**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** Physical dmg +5%.
**Max (Lv 21):** Physical dmg +25%.

**Slieksnis:** Ja rādiusā ir 10+ Str mezgli, Physical Reizinātājs +10%.

**Drop:** Elite bosi visos reģionos.

**Ieteicamie buildi:** Pirmais Kungs, Otrs Kungs (Hybrid Balance).

---

### #12. **Puves Zvana**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** DoT dmg +7%.
**Max (Lv 21):** DoT dmg +35%.

**Slieksnis:** Ja rādiusā ir 3+ Prāta mezgli, DoT tick 20% ātrāk.

**Drop:** Vecais Tornis reģiona boss (Selriss).

**Ieteicamie buildi:** Vecais Burvis Realitātes Karaļvalsts (freeze DoT), Otrs Kungs Chain Lightning DoT sinergs.

---

### #13. **Bezdibeņa Zīme**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** Vulnerable dmg +7%.
**Max (Lv 21):** Vulnerable dmg +30%.

**Slieksnis:** Ja rādiusā ir 5+ Vek mezgli, tavas prasmes uzliek Vulnerable +2s.

**Drop:** Murgu T20+.

**Ieteicamie buildi:** Vecais Burvis (Vulnerable + freeze), Otrs Kungs (Impēriskais debuff), jebkuram DPS.

---

### #14. **Vecā Vaids**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** Overpower dmg +10%.
**Max (Lv 21):** Overpower dmg +40%.

**Slieksnis:** Ja rādiusā ir 5+ Izturība mezgli, Overpower chance +3%.

**Drop:** Murgu T30+.

**Ieteicamie buildi:** Pirmais Kungs Sardzes Zvērests (Overpower ar Fortify farm), Otrs Kungs Permanent Empress.

---

## 2.4. Utility (4)

### #15. **Slēgtais Vārds**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** CDR +2% ja Prāts rādiusā 200+.
**Max (Lv 21):** CDR +8% ja Prāts rādiusā 400+.

**Slieksnis:** Ja Prāts rādiusā 800+, papildu -15% cooldown visām prasmēm.

**Drop:** Elite bosi Murgu T5+.

**Ieteicamie buildi:** Vecais Burvis (Prāts primary), Otrs Kungs (hybrid Prāts).

---

### #16. **Vainaga Māc**

**Tips:** Universāla
**Rarity:** Reta

**Bāzes (Lv 1):** Ult (100pt) cooldown -5%.
**Max (Lv 21):** Ult cooldown -20% + ult ilgums +2s.

**Slieksnis:** Ja rādiusā ir Leģendārs mezgls, ult prasme dubultā charges reizē.

**Drop:** Murgu T40+.

**Ieteicamie buildi:** Vecais Burvis (Reality Rift), Otrs Kungs (Impērija Vaidēs), Pirmais Kungs (Kunga Zvans).

---

### #17. **Miglas Zīme**

**Tips:** Universāla
**Rarity:** Kopīga

**Bāzes (Lv 1):** Barrier gen +10%.
**Max (Lv 21):** Barrier gen +40% + Barrier ilgums +3s.

**Slieksnis:** Ja rādiusā ir 3+ Izturība/HP mezgli, Barrier daļa (30%) konvertējas uz permanent HP.

**Drop:** Murgu T20+.

**Ieteicamie buildi:** Pirmais Kungs Sardzes Zvērests, Otrs Kungs Permanent Empress.

---

### #18. **Nolādētais Sirdsdedzis**

**Tips:** Universāla
**Rarity:** Uber (leģendāra)

**Bāzes (Lv 1):** Visi rādiuss bonusi ×1.5, bet tu zaudē 1% max HP/sec cīņā.
**Max (Lv 21):** Visi rādiuss bonusi ×2.5, bet tu zaudē 2% max HP/sec cīņā.

**Slieksnis:** Nav sliekšnis — vienmēr aktīvs kad Rūna uzstādīta.

**Drop:** Uber Bosi (rets), World Bosi (ļoti rets).

**Ieteicamie buildi:** Pirmais Kungs Sardzes Zvērests (minjonu shared dmg kompensē), Vecais Burvis Realitātes Karaļvalsts (Ark. Enerģija resurss).

---

# 3. Klases-Specifiskās Rūnas (6)

*2 per klase × 3 klases.*

## 3.1. Pirmais Kungs

### #19. **Dominance Zīme**

**Tips:** Klases-specifiska (Pirmais Kungs)
**Rarity:** Reta

**Bāzes (Lv 1):** Dominance uzkrāj +10% ātrāk.
**Max (Lv 21):** Dominance uzkrāj +35% ātrāk + Dominance Aura ilgums +5s.

**Slieksnis:** Ja rādiusā ir Barveža mezgls, tavu minjonu bonuss no auras vēl +15%.

**Drop:** Sardzes Kapteinis Bornlejs (Pirmā Kunga klases boss) — pirmreizējs kill.

**Ieteicamie buildi:** Pirmais Kungs visi 3 buildi.

---

### #20. **Kunga Kronis**

**Tips:** Klases-specifiska (Pirmais Kungs)
**Rarity:** Leg (leģendāra)

**Bāzes (Lv 1):** Master's Call cooldown -10%, aura efekts +5%.
**Max (Lv 21):** Master's Call cooldown -30%, aura efekts +25%, Kunga Zvana cooldown -20%.

**Slieksnis:** Ja rādiusā ir Leģendārais mezgls "Meistars pāri Visiem Kronis", aura pastāvīga (bezgalīga uptime).

**Drop:** Murgu Dungeon "Meistara Kripta" (Melnais Tornis reģions) T40+.

**Ieteicamie buildi:** Meistars pāri Visiem, Kunga Impērija.

---

## 3.2. Otrs Kungs

### #21. **Impērijas Zīme**

**Tips:** Klases-specifiska (Otrs Kungs)
**Rarity:** Reta

**Bāzes (Lv 1):** Impēriskā Dusma uzkrāj +15% ātrāk.
**Max (Lv 21):** Impēriskā Dusma uzkrāj +40% ātrāk + Impēriskā Dusma nekad neizzūd cīņā.

**Slieksnis:** Ja rādiusā ir Nolādēts mezgls, tavas prasmes pret "gaišajiem" ordeņiem +25% dmg.

**Drop:** Emperors Solariuss (Otrā Kunga klases boss) — pirmreizējs kill.

**Ieteicamie buildi:** Otrs Kungs visi 3 buildi.

---

### #22. **Nordbergas Solis**

**Tips:** Klases-specifiska (Otrs Kungs)
**Rarity:** Leg (leģendāra)

**Bāzes (Lv 1):** Sūkšņi + Zilspāri sinerģijas bonuss +10%.
**Max (Lv 21):** Sūkšņi + Zilspāri sinerģijas bonuss +30% + kad melee un spell hit 0.5s laikā, tava nākošā prasme bezmaksas.

**Slieksnis:** Ja rādiusā ir 5+ Prāta mezgli, Melnās Zibenis chain +3 lēcieni papildu.

**Drop:** Murgu "Nordbergas Cietoksnis" (Nordberga reģions) T40+.

**Ieteicamie buildi:** Hybrid Balance (kritiskā), Permanent Empress.

---

## 3.3. Vecais Burvis

### #23. **Realitātes Zīme**

**Tips:** Klases-specifiska (Vecais Burvis)
**Rarity:** Reta

**Bāzes (Lv 1):** Reality Rift ilgums +2s + Ark. Enerģija patēriņš -10%.
**Max (Lv 21):** Reality Rift ilgums +8s + Ark. Enerģija patēriņš -35%.

**Slieksnis:** Ja rādiusā ir Leģendārais mezgls, Reality Rift laikā tavas prasmes triple dmg (nevis 2×).

**Drop:** Vecākais Māceklis Selriss (Vecā Burvja klases boss) — pirmreizējs kill.

**Ieteicamie buildi:** Realitātes Karaļvalsts (kritiskā), Torņa Ass.

---

### #24. **Torņa Vaids**

**Tips:** Klases-specifiska (Vecais Burvis)
**Rarity:** Leg (leģendāra)

**Bāzes (Lv 1):** Zilspāri spell power +25% papildu (kopā ×3.5 baseline).
**Max (Lv 21):** Zilspāri spell power +75% papildu (kopā ×4.5) + Zilspāri max +2.

**Slieksnis:** Ja rādiusā ir 5+ Prāta mezgli, Zilspāri var attālināti ceļ skeletus no jebkuriem līķiem.

**Drop:** Murgu "Torņa Šūpuļa" (Vecais Tornis reģions) T40+.

**Ieteicamie buildi:** Zilspāri Storm, Realitātes Karaļvalsts.

---


# 4. Rūnu ieteikumi pa buildiem

## 4.1. Pirmais Kungs

| Build                      | Slot 1 (kritiskā)   | Slot 2 (dfns/utility) | Slot 3 (endgame)      |
|----------------------------|---------------------|------------------------|-----------------------|
| A. Meistars pāri Visiem    | Dominance Zīme      | Asinssaite             | Kroņa Roka            |
| B. Sardzes Zvērests        | Kunga Kronis        | Ģerbonis               | Miglas Zīme           |
| C. Kunga Impērija          | Dominance Zīme      | Asinssaite             | Kroņa Roka            |

## 4.2. Otrs Kungs

| Build                  | Slot 1              | Slot 2                 | Slot 3                |
|------------------------|---------------------|------------------------|-----------------------|
| A. Hybrid Balance      | Nordbergas Solis    | Kritušā Vaids          | Slēgtais Vārds        |
| B. Chain Lightning     | Impērijas Zīme      | Zibens Solis           | Zvana Skaņa           |
| C. Permanent Empress   | Nordbergas Solis    | Nolādētais Sirdsdedzis | Vainaga Māc           |

## 4.3. Vecais Burvis

| Build                      | Slot 1              | Slot 2                 | Slot 3                |
|----------------------------|---------------------|------------------------|-----------------------|
| A. Realitātes Karaļvalsts  | Realitātes Zīme     | Nolādētais Sirdsdedzis | Vientuļā Sirds bonuss |
| B. Zilspāri Storm          | Torņa Vaids         | Asinssaite             | Zibens Solis          |
| C. Torņa Ass Double Cast   | Realitātes Zīme     | Vainaga Māc            | Zvana Skaņa           |

---

# 5. Rūnu iegūšanas ceļvedis (kur farm)

## 5.1. Reģionu bosi (klases-agnostiskie Lv 1)

| Reģions            | Boss                | Rūna dropi                              |
|--------------------|---------------------|------------------------------------------|
| Melnais Tornis     | Sardzes Kapteinis Bornlejs | Svētais Vaids, Ugunslīnija         |
| Nordberga          | Emperors Solariuss  | Zibens Solis, Ēnu Zīme                  |
| Vecais Tornis      | Vecākais Māceklis Selriss  | Sasala Zīme, Puves Zvana           |
| Melnais Kalns      | Nāves Vēstnesis     | Kritušā Solis, Bezdibeņa Zvana           |

## 5.2. Klases-specifiskās (pirmreizējs klases boss kill)

Katras klases pirmais klases boss dropo klases-specifisko Rūnu (Lv 1):
- **Pirmais Kungs:** Bornlejs → Dominance Zīme
- **Otrs Kungs:** Solariuss → Impērijas Zīme
- **Vecais Burvis:** Selriss → Realitātes Zīme

## 5.3. Murgu Dungeoni

| Tier      | Kas dropo                          | XP uz Rūnu             |
|-----------|-------------------------------------|------------------------|
| T1–T20    | Random universālas Rūnas Lv 1      | Lv 1→5 progress        |
| T20–T40   | Retākas + upgrade materiāli        | Lv 5→10                |
| T40–T60   | Leģendārās (klases-specifiskās Lv 1)| Lv 10→15              |
| T60–T80   | Leg + upgrade materiāli            | Lv 15→18               |
| T80–T100  | Uber Rūnu shanc + Zvaigznes Skabarga| Lv 18→21             |

## 5.4. Uber Rūnas (Nolādētais Sirdsdedzis, Mezglu Kalums)

**Iegūst tikai no:**
- Uber Bosi (endgame, atslēdz ar rituālu 5× Melnā Rune + Zvaigznes Skabarga)
- World Bosi (~1% chance uz Uber Rūnu drop)

**Grūtums:** viens Uber Rūnu drop = **~50 stundu grinds** vidēji.

---

# 6. Rūnu ekonomika

## 6.1. Cik daudz Rūnas var izmantot vienlaicīgi?

Skat. `ParagonBoards.md` §6.2:

- **Pirmais Kungs:** 17 slots
- **Otrs Kungs:** 15 slots
- **Vecais Burvis:** 17 slots

Vidēji **spēlētājs izmanto 5–8 unikālās Rūnas** — pārējie sloti paliek tukši vai ar mazāk stipru Rūnu, kamēr endgame farm progressē.

## 6.2. Ideāls Rūnu setup ceļvedis

| Progresas posms  | Rūnu setup                                           |
|------------------|------------------------------------------------------|
| Level 50 (Paragon atslēdzas) | 1-2 klases-specifiskās Rūnas Lv 5-10  |
| Paragon 100      | 3-4 Rūnas Lv 10                                     |
| Paragon 200      | 5-6 Rūnas Lv 15                                      |
| Paragon 300      | 6-7 Rūnas Lv 18                                      |
| Paragon 350      | 6-8 Rūnas Lv 21 (max, endgame chase)                 |

---

# 7. Kopsavilkums

- **30 Rūnas kopā** (18 universālas + 12 klases-specifiskas).
- **Rūnu retumi:** ~15 kopīgas, ~10 retas, ~4 leģendāras, ~1 Uber.
- **Iegūšana:** viena Rūna par bosu, pārējās no Murgu Dungeoniem.
- **Upgrade:** Lv 1 → 21 = ~40 Murgu runs = 10-15 stundas per Rūna.
- **Endgame chase:** pilnas 8 Rūnas Lv 21 = **~100 stundu grinds**.

Rūnas ir viens no galvenajiem endgame retention mehānikas — ne visi spēlētāji sasniedz Lv 21 uz visām rūnām, tas ir top 1% chase.

*Skaitļi ir sākuma balansa piedāvājums. Tuning notiek pēc playtesting.*
