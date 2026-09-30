from fastapi import FastAPI
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

@app.get("/")
def read_root():
    return {"message": "OmniSolve backend darbojas veiksmīgi!"}

@app.get("/api/ip")
def get_user_ip():
    # Šeit vēlāk pievienosim reālo IP nolasīšanu
    return {"ip": "192.168.1.1", "status": "success"}
