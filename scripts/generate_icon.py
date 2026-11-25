#!/usr/bin/env python3
"""Icone 55x56 style apps natives Epsilon."""

from pathlib import Path

from PIL import Image, ImageDraw

W, H = 55, 56
WHITE = (255, 255, 255)
SHADOW = (217, 217, 217)
YELLOW = (255, 184, 27)
PRIMARY = (62, 63, 69)

root = Path(__file__).resolve().parents[1]
out = root / "assets" / "icon" / "icon.png"
out.parent.mkdir(parents=True, exist_ok=True)

img = Image.new("RGB", (W, H), WHITE)
draw = ImageDraw.Draw(img)

# Carre arrondi jaune (tuile d'accueil)
draw.rounded_rectangle((2, 2, W - 3, H - 5), radius=12, fill=YELLOW)

# Trois lignes horizontales (menu) + pastille
draw.rounded_rectangle((14, 16, 41, 21), radius=2, fill=PRIMARY)
draw.rounded_rectangle((14, 26, 41, 31), radius=2, fill=PRIMARY)
draw.rounded_rectangle((14, 36, 34, 41), radius=2, fill=PRIMARY)
draw.ellipse((37, 35, 44, 42), fill=WHITE)

draw.line([(3, H - 3), (W - 4, H - 3)], fill=SHADOW, width=1)
img.save(out, optimize=True)
print(f"Icone : {out}")