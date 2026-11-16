"""Ģenerē multenes fona mūziku (mūzikas lādītes skaņa).

Palaist no video/ mapes:
  python3 tools/make_music.py          -> public/cepums-muzika.mp3      (5 min, 2D multenei)
  python3 tools/make_music.py --short  -> public/cepums-muzika-90s.mp3  (1:30, 3D multenei)
"""
import sys
import subprocess
import wave

import numpy as np

SHORT = "--short" in sys.argv
NAME = "public/cepums-muzika-90s" if SHORT else "public/cepums-muzika"
SR = 32000
TOTAL = 90.0 if SHORT else 300.0
BPM = 96
BEAT = 60 / BPM
BAR = BEAT * 4

# (sākums, beigas) sekundēs, kad skan skumjākā minora tēma — sakrīt ar ainām.
WISTFUL = [(14, 30), (40, 46), (63, 75)] if SHORT else [(36, 88), (118, 138), (192, 229)]

MAJOR = [("C", [60, 64, 67]), ("G", [59, 62, 67]), ("Am", [57, 60, 64]), ("F", [57, 60, 65])]
MINOR = [("Am", [57, 60, 64]), ("F", [57, 60, 65]), ("Dm", [57, 62, 65]), ("E", [56, 59, 64])]
BASS = {"C": 48, "G": 43, "Am": 45, "F": 41, "Dm": 50, "E": 40}


def freq(m):
    return 440.0 * 2 ** ((m - 69) / 12)


def note(buf, start, midi, dur, vol, bright=1.0):
    n = int(dur * SR)
    i0 = int(start * SR)
    if i0 >= len(buf):
        return
    n = min(n, len(buf) - i0)
    t = np.arange(n) / SR
    f = freq(midi)
    env = np.exp(-t * 3.2) * np.minimum(1, t * 400)
    tone = np.sin(2 * np.pi * f * t) + 0.35 * bright * np.sin(2 * np.pi * 2 * f * t) + 0.12 * bright * np.sin(2 * np.pi * 4.02 * f * t)
    buf[i0 : i0 + n] += vol * env * tone


def main():
    rng = np.random.default_rng(7)
    buf = np.zeros(int(TOTAL * SR))
    bars = int(TOTAL / BAR) + 1
    prev = 2
    for b in range(bars):
        t0 = b * BAR
        sad = any(a <= t0 < e for a, e in WISTFUL)
        name, chord = (MINOR if sad else MAJOR)[b % 4]
        note(buf, t0, BASS[name], BAR * 0.9, 0.30, 0.4)
        note(buf, t0 + 2 * BEAT, BASS[name] + 7, BEAT * 1.8, 0.18, 0.4)
        for beat in (1, 3):
            for m in chord:
                note(buf, t0 + beat * BEAT, m, BEAT * 1.5, 0.07, 0.5)
        pool = [m + 12 for m in chord] + [chord[0] + 24]
        steps = [0, 2, 4, 6] if sad else [0, 1, 2, 3, 4, 6, 7]
        for s in steps:
            if not sad and rng.random() < 0.18:
                continue
            idx = int(np.clip(prev + rng.integers(-1, 2), 0, len(pool) - 1))
            prev = idx
            note(buf, t0 + s * BEAT / 2, pool[idx], BEAT * (2.2 if sad else 1.4), 0.26)
    # Vienkārša atbalss, lai skan mīkstāk.
    d = int(0.33 * SR)
    buf[d:] += 0.28 * buf[:-d]
    buf /= np.max(np.abs(buf)) * 1.15
    fade = int(2.5 * SR)
    buf[:fade] *= np.linspace(0, 1, fade)
    buf[-fade:] *= np.linspace(1, 0, fade)
    with wave.open(NAME + ".wav", "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes((buf * 32767).astype(np.int16).tobytes())
    subprocess.run(
        ["ffmpeg", "-loglevel", "error", "-y", "-i", NAME + ".wav", "-b:a", "96k", NAME + ".mp3"],
        check=True,
    )
    subprocess.run(["rm", NAME + ".wav"], check=True)


if __name__ == "__main__":
    main()
