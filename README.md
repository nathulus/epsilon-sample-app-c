# NumWorks Autre

Application externe (**NWA / EADK**) pour NumWorks N0100–N0120, testée en compilation pour **Epsilon 23.2.6** (API EADK niveau 0).

> **Ne modifie pas le firmware.** Le fichier produit `output/app.nwa` s’installe via le [site officiel NumWorks](https://my.numworks.com/apps) (uploader d’applications externes).

## Contenu (v0.1)

- Menu principal **Autre** (6 entrées ; seul **Formules** est actif)
- Module **Formules → Électricité → Puissance P = U × I**
- Saisie au clavier, calcul détaillé, icône 55×56 px

## Prérequis (Windows)

1. **Node.js** (v20+ ; testé avec v24 — utiliser `nwlink@0.0.19`)
2. **GNU Arm Embedded Toolchain** — par ex. `winget install Arm.GnuArmEmbeddedToolchain`
3. (Optionnel) **GNU make** sous MSYS2 pour `make build`
4. **Python 3** + Pillow pour régénérer l’icône

## Compiler

### PowerShell (recommandé sans MSYS2)

```powershell
python scripts/generate_icon.py
.\scripts\build.ps1
```

Le binaire est dans `output/app.nwa`.

### Make (Linux / MSYS2)

```bash
make clean && make build
```

## Installer sur la calculatrice (N0120, Epsilon 23.2.6)

**Important :** on installe le fichier **`app.nwa`**, pas les scripts Python du dossier `scripts/`.  
La commande `nwlink install-nwa` depuis PowerShell **ne fonctionne en général pas sous Windows** (`no devices found`) : l’installation passe par le **navigateur** (WebUSB).

1. Compiler : `.\scripts\build.ps1` → `output\app.nwa`
2. Raccourci : `.\scripts\ouvrir-installateur.ps1` (ouvre la page officielle + copie le chemin du fichier)
3. **Chrome** ou **Edge** uniquement (pas Firefox), **un seul onglet** ouvert
4. Calculatrice **allumée**, câble **données** (pas charge seule), connectée en USB
5. Se connecter sur https://my.numworks.com/apps (compte NumWorks gratuit)
6. **Connect** → choisir « NumWorks Calculator » → ajouter `app.nwa` → **Install**
7. Si la calculatrice n’apparaît pas : réparer le pilote Windows (voir [aide NumWorks](https://www.numworks.com/support/handheld/detect/)), autre port USB, redémarrer le PC

Sur la page d’apps, **re-sélectionnez aussi les applis déjà installées** que vous voulez garder : la liste envoyée remplace l’ancienne.

**Important :** les apps externes sont effacées en cas de réinitialisation ou de crash système — c’est le comportement officiel Epsilon.

## Structure du code

```
src/
  main.c              # Métadonnées EADK
  app.c               # Menu principal
  menu/               # Navigation liste
  ui/                 # Affichage et événements
  modules/formules/   # Formules (extensible)
assets/icon/          # icon.png 55×56
scripts/build.ps1     # Build Windows
output/app.nwa        # Application installable
```

## Ajouter une formule

Dans `src/modules/formules/formules.c`, compléter le tableau `g_formulas` et ajouter une entrée de menu.

## API EADK utilisée

- Affichage : `eadk_display_*`
- Clavier : `eadk_event_get`
- Pause : `eadk_timing_msleep` (non utilisé en v0.1)

Non utilisées (simulation / absentes sur matériel réel) : fonctions batterie et USB.

## Licence

Projet dérivé du modèle [epsilon-sample-app-c](https://github.com/numworks/epsilon-sample-app-c) (BSD). Voir `LICENSE`.