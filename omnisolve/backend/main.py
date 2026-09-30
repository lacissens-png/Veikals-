from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI(title="OmniSolve API", version="1.0")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.get("/")
def read_root():
    return {"message": "OmniSolve backend darbojas veiksmīgi!"}

# Uzlabots IP adreses un pieprasījuma punktu galamērķis
@app.get("/api/info")
def get_user_info(request: Request):
    # Iegūstam klienta IP adresi no pieprasījuma galvenajām rindām
    client_host = request.client.host
    
    # Pārbaudām, vai aizmugursistēma saņem datus
    return {
        "ip_address": client_host,
        "status": "active",
        "tool": "Tech Hub - IP & Network"
    }

# Pamācību datu bāze (ātrajiem jautājumiem / life hacks)
@app.get("/api/tips/{tip_id}")
def get_quick_tip(tip_id: str):
    tips = {
        "router": "Kā restartēt rūteri: 1. Izvelciet strāvas vadu no rozetes. 2. Pagaidiet 30 sekundes. 3. Pieslēdziet atpakaļ un pagaidiet 2 minūtes.",
        "screenshot": "Ekrānuzņēmums: Windows (Win + Shift + S), Mac (Cmd + Shift + 3), Telefons (Barošana + Klusāk pogas).",
        "eggs": "Olu vārīšanas laiks: Mīkstas (4 minūtes), Vidējas (6 minūtes), Cietas (9-10 minūtes)."
    }
    
    result = tips.get(tip_id.lower(), "Padoms netika atrasts. Mēģiniet: router, screenshot, vai eggs.")
    return {"tip_id": tip_id, "instruction": result}
