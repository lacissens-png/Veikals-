from fastapi import FastAPI

app = FastAPI(title="OmniSolve")


@app.get("/")
def root():
    return {"status": "ok", "project": "OmniSolve"}
