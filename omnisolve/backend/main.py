from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI(title="OmniSolve API", version="1.0")

# Atļaujam savienojumus no jebkuras vietas (noderēs, kad pieslēgsim frontend)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Tech Hub rīku saraksts (vēlāk papildināsim ar jauniem rīkiem)
TOOLS = [
    {"id": "ip", "name": "Mana IP adrese", "description": "Parāda tavu publisko IP adresi", "endpoint": "/api/ip"},
    {"id": "device", "name": "Ierīces informācija", "description": "Parāda pārlūku un ierīces datus", "endpoint": "/api/device"},
]


def get_client_ip(request: Request) -> str:
    # Ja serveris ir aiz starpniekservera (hostingā), īstā IP ir galvenē X-Forwarded-For
    forwarded = request.headers.get("x-forwarded-for")
    if forwarded:
        return forwarded.split(",")[0].strip()
    real_ip = request.headers.get("x-real-ip")
    if real_ip:
        return real_ip.strip()
    return request.client.host if request.client else "nezināma"


@app.get("/")
def read_root():
    return {"message": "OmniSolve backend darbojas veiksmīgi!"}


@app.get("/api/ip")
def get_user_ip(request: Request):
    return {"ip": get_client_ip(request), "status": "success"}


@app.get("/api/device")
def get_device_info(request: Request):
    return {
        "ip": get_client_ip(request),
        "user_agent": request.headers.get("user-agent", "nezināms"),
        "language": request.headers.get("accept-language", "nezināma"),
        "status": "success",
    }


@app.get("/api/tools")
def list_tools():
    return {"tools": TOOLS, "count": len(TOOLS)}
