# Compatibilité N0120 — Epsilon 23.2.6

## Vérifications effectuées (sans flash sur l’appareil)

| Point | Statut |
| --- | --- |
| Format de sortie `.nwa` (EADK / API level 0) | OK — généré par `nwlink@0.0.19` |
| Validation binaire `nwa-bin` | OK — `output/app.bin` produit sans erreur |
| Cible processeur (`cortex-m7`, float hard) | OK — imposé par `eadk-cflags-device` |
| Symboles requis (`eadk_app_name`, `eadk_app_icon`, `eadk_api_level`) | OK |
| Icône 55×56 PNG | OK |
| Fonctions batterie / USB EADK | **Non utilisées** (non garanties sur matériel) |

## Installation

Procédure officielle uniquement :

1. https://my.numworks.com/apps
2. Connexion + câble USB
3. Envoi de `output/app.nwa`

Aucun firmware personnalisé, aucune réinitialisation requise.

## Remarques N0120

- Le N0120 partage la même plateforme applicative externe (NWA) que les modèles récents sous Epsilon 23.x.
- En **mode examen**, les apps externes sont masquées (comportement Epsilon).
- Après **réinitialisation** ou certains crashs, les apps externes peuvent être supprimées : réinstaller `app.nwa` si besoin.