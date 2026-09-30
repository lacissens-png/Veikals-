"""Biznesa un karjeras modulis: algas, pašnodarbinātā un cenas kalkulatori.

Nodokļu likmes Latvijā 2026. gadā (VID, LV portāls). Aprēķini ir aptuveni –
tie neņem vērā VSAOI maksimālo apmēru, solidaritātes nodokli un citus
retākus gadījumus.
"""

from fastapi import APIRouter, Query

router = APIRouter(prefix="/api/business", tags=["Bizness un karjera"])

MIN_WAGE = 780.0                     # minimālā mēneša alga
NON_TAXABLE_MIN = 550.0              # neapliekamais minimums mēnesī
DEPENDENT_RELIEF = 250.0             # atvieglojums par katru apgādājamo mēnesī
IIN_RATE = 0.255                     # IIN līdz 105 300 € gadā
IIN_HIGH_RATE = 0.33                 # IIN virs 105 300 € gadā
IIN_HIGH_THRESHOLD = 105_300 / 12    # tas pats slieksnis mēnesī (8775 €)
VSAOI_EMPLOYEE = 0.105
VSAOI_EMPLOYER = 0.2359
RISK_FEE = 0.36                      # uzņēmējdarbības riska valsts nodeva mēnesī
VSAOI_SELF_FULL = 0.3107             # pašnodarbinātā pilnā likme (no minimālās algas)
VSAOI_SELF_PENSION = 0.10            # pensiju iemaksa no pārējiem ienākumiem
VAT_RATE = 0.21

TAX_YEAR = 2026


def money(value: float) -> float:
    return round(value + 1e-9, 2)


def income_tax(taxable: float) -> float:
    taxable = max(0.0, taxable)
    low = min(taxable, IIN_HIGH_THRESHOLD)
    high = max(0.0, taxable - IIN_HIGH_THRESHOLD)
    return low * IIN_RATE + high * IIN_HIGH_RATE


@router.get("/salary")
def salary(
    gross: float = Query(..., ge=0, le=1_000_000, description="Bruto alga mēnesī, €"),
    dependents: int = Query(0, ge=0, le=20, description="Apgādājamo skaits"),
):
    vsaoi = money(gross * VSAOI_EMPLOYEE)
    iin = money(income_tax(gross - vsaoi - NON_TAXABLE_MIN - dependents * DEPENDENT_RELIEF))
    employer_vsaoi = gross * VSAOI_EMPLOYER
    return {
        "gross": money(gross),
        "vsaoi_employee": money(vsaoi),
        "income_tax": money(iin),
        "net": money(gross - vsaoi - iin),
        "employer_vsaoi": money(employer_vsaoi),
        "employer_total_cost": money(gross + employer_vsaoi + RISK_FEE),
        "tax_year": TAX_YEAR,
    }


@router.get("/self-employed")
def self_employed(
    income: float = Query(..., ge=0, le=1_000_000, description="Ieņēmumi mēnesī, €"),
    expenses: float = Query(0, ge=0, le=1_000_000, description="Izdevumi mēnesī, €"),
    has_job: bool = Query(False, description="Neapliekamais minimums jau izmantots algotā darbā"),
):
    profit = max(0.0, income - expenses)
    if profit >= MIN_WAGE:
        vsaoi = money(MIN_WAGE * VSAOI_SELF_FULL + (profit - MIN_WAGE) * VSAOI_SELF_PENSION)
    else:
        vsaoi = money(profit * VSAOI_SELF_PENSION)
    iin = money(income_tax(profit - vsaoi - (0 if has_job else NON_TAXABLE_MIN)))
    return {
        "income": money(income),
        "expenses": money(expenses),
        "profit": money(profit),
        "vsaoi": money(vsaoi),
        "income_tax": money(iin),
        "net": money(profit - vsaoi - iin),
        "full_social_insurance": profit >= MIN_WAGE,
        "tax_year": TAX_YEAR,
    }


@router.get("/price")
def price(
    materials: float = Query(0, ge=0, le=10_000_000, description="Materiālu/izejvielu izmaksas, €"),
    hours: float = Query(0, ge=0, le=10_000, description="Darba stundas"),
    hourly_rate: float = Query(0, ge=0, le=10_000, description="Stundas likme, €"),
    margin: float = Query(20, ge=0, le=1000, description="Uzcenojums, %"),
    vat: bool = Query(False, description="Pievienot PVN 21%"),
):
    cost = money(materials + hours * hourly_rate)
    price_without_vat = money(cost * (1 + margin / 100))
    vat_amount = money(price_without_vat * VAT_RATE) if vat else 0.0
    return {
        "cost": money(cost),
        "profit": money(price_without_vat - cost),
        "price_without_vat": money(price_without_vat),
        "vat": money(vat_amount),
        "price": money(price_without_vat + vat_amount),
    }
