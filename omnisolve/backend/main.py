import anthropic
from fastapi import FastAPI, HTTPException, Request
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, Field

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


# Datu struktūra ienākošajam AI jautājumam
class AIQuery(BaseModel):
    question: str


def keyword_answer(question: str) -> str:
    # Gatavās atbildes pēc atslēgvārdiem (strādā arī bez AI API atslēgas)
    user_question = question.lower()
    if "riepu" in user_question:
        return "Kā nomainīt riepu: 1. Nostādiet mašīnu uz līdzenas virsmas un pavelciet rokas bremzi. 2. Atskrūvējiet skrūves, pirms ceļat mašīnu ar domkratu. 3. Paceliet auto, noņemiet riteni un uzlieciet rezerves riteni."
    if "inflācija" in user_question:
        return "Inflācija ir vispārējs preču un pakalpojumu cenu līmeņa piepaugums ekonomikā, kā rezultātā naudas pirktspēja samazinās."
    return f"AI Atbilde: Jautājums '{question}' ir saņemts. OmniSolve AI dzinējs analizē datus un sniedz tiešu atbildi bez reklāmām!"


# Atbildes pēc atslēgvārdiem (testam, bez Claude)
@app.post("/api/ai/ask")
def ask_ai_keywords(query: AIQuery):
    return {"question": query.question, "answer": keyword_answer(query.question)}


# AI jautājumu lodziņš (Claude)
AI_MODEL = "claude-opus-5-5"
AI_SYSTEM_PROMPT = (
    "Tu esi OmniSolve Tech Hub palīgs. Atbildi latviešu valodā, īsi un saprotami, "
    "ar konkrētiem soļiem, ja jautājums ir par tehniku, internetu vai ikdienas problēmām. "
    "Ja nezini atbildi, pasaki to godīgi."
)


class Question(BaseModel):
    question: str = Field(min_length=1, max_length=2000)


_ai_client = None


def get_ai_client():
    # Klientu veidojam tikai pirmajā pieprasījumā, lai serveris startē arī bez API atslēgas.
    # Atgriež None, ja AI nav konfigurēts.
    global _ai_client
    if _ai_client is None:
        try:
            _ai_client = anthropic.Anthropic()
        except anthropic.AnthropicError:
            return None
    return _ai_client


def offline_answer(question: str):
    return {"answer": keyword_answer(question), "status": "offline"}


@app.post("/api/ask")
def ask_ai(body: Question):
    client = get_ai_client()
    if client is None:
        return offline_answer(body.question)
    try:
        response = client.beta.messages.create(
            model=AI_MODEL,
            max_tokens=16000,
            system=AI_SYSTEM_PROMPT,
            output_config={"effort": "low"},
            betas=["server-side-fallback-2026-07-01"],
            fallbacks="default",
            messages=[{"role": "user", "content": body.question}],
        )
    except TypeError:
        # SDK neatrada nevienu autentifikācijas veidu (nav ANTHROPIC_API_KEY) – atbildam pēc atslēgvārdiem
        return offline_answer(body.question)
    except anthropic.AuthenticationError:
        raise HTTPException(status_code=503, detail="AI API atslēga nav derīga.")
    except anthropic.RateLimitError:
        raise HTTPException(status_code=429, detail="Pārāk daudz jautājumu. Pamēģini pēc brīža.")
    except anthropic.APIStatusError as e:
        raise HTTPException(status_code=502, detail=f"AI servisa kļūda ({e.status_code}).")
    except anthropic.APIConnectionError:
        raise HTTPException(status_code=502, detail="Neizdevās sazināties ar AI servisu.")

    if response.stop_reason == "refusal":
        return {"answer": "Uz šo jautājumu es nevaru atbildēt.", "status": "refused"}

    answer = "".join(block.text for block in response.content if block.type == "text")
    return {"answer": answer, "status": "success"}
