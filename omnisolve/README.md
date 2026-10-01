# OmniSolve

Projekta pamats. Darbs notiek soli pa solim.

## Struktūra

```
omnisolve/
├── src/     # pirmkods
├── tests/   # testi
└── docs/    # dokumentācija un piezīmes
```

## Darba vide

Pārbaudīts pieejamajā vidē:

- Python 3.11
- Node.js 22 / npm
- Git

Konkrētā tehnoloģija (Python vai Node.js) tiks izvēlēta nākamajā solī.

## Progress

- [x] 1. solis: projekta mape un vide
- [x] 2. solis: IP informācija (`/api/info`) un ātrie padomi (`/api/tips/{tip_id}`)
- [x] 3. solis: frontend (`frontend/index.html`) – IP statuss un ātro padomu pogas
- [x] 4. solis: soļu skaitītājs (DeviceMotion sensors, iPhone atļauja, dienas atiestatīšana, attālums m/km)
- [x] 5. solis: AI jautājumu lodziņš (`POST /api/ask` ar Claude; bez `ANTHROPIC_API_KEY` atbild pēc atslēgvārdiem) + `POST /api/ai/ask` (tikai atslēgvārdi)
- [x] 6. solis: Bizness un karjera – algas, pašnodarbinātā un cenas kalkulatori (`/api/business/...`, 2026. g. likmes)
- [x] 7. solis: CV veidotājs – forma, priekšskatījums, pārbaude (`/api/cv/check`), AI kopsavilkums (`/api/cv/improve`), drukāšana/PDF
- [x] 8. solis: lapa sadalīta cilnēs – Tech, Veselība, Bizness, CV (izvēlētā cilne saglabājas)
