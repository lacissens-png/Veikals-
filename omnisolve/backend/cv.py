"""CV rīks: CV datu pārbaude ar konkrētiem uzlabošanas padomiem."""

import re

from fastapi import APIRouter
from pydantic import BaseModel, Field

router = APIRouter(prefix="/api/cv", tags=["CV"])


class CV(BaseModel):
    name: str = Field("", max_length=200)
    title: str = Field("", max_length=200)
    email: str = Field("", max_length=200)
    phone: str = Field("", max_length=50)
    summary: str = Field("", max_length=3000)
    experience: str = Field("", max_length=10000)   # katra darba vieta jaunā rindā
    education: str = Field("", max_length=5000)
    skills: str = Field("", max_length=2000)         # atdalītas ar komatu


def lines(text: str) -> list[str]:
    return [line.strip() for line in text.splitlines() if line.strip()]


def skill_list(text: str) -> list[str]:
    return [skill.strip() for skill in text.split(",") if skill.strip()]


@router.post("/check")
def check_cv(cv: CV):
    tips = []

    if not cv.name.strip():
        tips.append("Ieraksti savu vārdu un uzvārdu.")
    if not cv.title.strip():
        tips.append("Pievieno amatu, uz kuru pretendē – tas ir pirmais, ko darba devējs redz.")
    if not cv.email.strip() and not cv.phone.strip():
        tips.append("Pievieno kontaktus: e-pastu vai tālruni.")
    elif cv.email.strip() and not re.fullmatch(r"[^@\s]+@[^@\s]+\.[^@\s]+", cv.email.strip()):
        tips.append("E-pasta adrese izskatās nepareiza.")

    summary_words = len(cv.summary.split())
    if summary_words == 0:
        tips.append("Uzraksti īsu kopsavilkumu (2–4 teikumi) par sevi un to, ko vari dot darba devējam.")
    elif summary_words < 20:
        tips.append("Kopsavilkums ir ļoti īss – pievieno savu pieredzi un stiprās puses.")
    elif summary_words > 80:
        tips.append("Kopsavilkums ir garš – saīsini līdz 2–4 teikumiem.")

    jobs = lines(cv.experience)
    if not jobs:
        tips.append("Pievieno darba pieredzi (arī prakse, brīvprātīgais darbs vai projekti skaitās).")
    elif not any(re.search(r"\d", job) for job in jobs):
        tips.append("Pieredzē norādi gadus un, ja vari, rezultātus skaitļos (piem., “+20% pārdošanā”).")

    if not lines(cv.education):
        tips.append("Pievieno izglītību.")

    skills = skill_list(cv.skills)
    if len(skills) < 3:
        tips.append("Norādi vismaz 3–5 prasmes, atdalot tās ar komatu.")
    elif len(skills) > 15:
        tips.append("Prasmju ir ļoti daudz – atstāj svarīgākās (līdz 10–12).")

    checks = 8
    score = round(100 * (checks - min(len(tips), checks)) / checks)
    return {"score": score, "tips": tips or ["CV izskatās labi! Pielāgo to katram darba sludinājumam."]}
