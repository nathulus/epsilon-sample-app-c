#ifndef AUTRE_UI_H
#define AUTRE_UI_H

#include <eadk.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Palette proche Epsilon / apps natives */
#define AUTRE_COLOR_WHITE eadk_color_white
#define AUTRE_COLOR_BLACK eadk_color_black
#define AUTRE_COLOR_YELLOW 0xFBC3       /* #FFB81B selection */
#define AUTRE_COLOR_TITLE 0xCE79        /* gris barre titre */
#define AUTRE_COLOR_GRAY 0x8410         /* texte secondaire */
#define AUTRE_COLOR_LINE 0xDEDB         /* separateur leger */

#define AUTRE_TITLE_H 18
#define AUTRE_ROW_H 35
#define AUTRE_LIST_TOP AUTRE_TITLE_H

void ui_clear(void);
void ui_draw_title(const char *title);
void ui_begin_screen(const char *title);

void ui_draw_string(int x, int y, const char *text, bool large, eadk_color_t fg, eadk_color_t bg);

/* Liste type Reglages : ligne pleine largeur, jaune si selectionnee */
void ui_draw_menu_list(const char *const *labels, int count, int selected, int top_y);

/* Ligne parametre : libelle a gauche, valeur a droite */
void ui_draw_param_row(int y, const char *label, const char *value, bool selected);

/* Ligne action (ex. Calculer) */
void ui_draw_action_row(int y, const char *label, bool selected);

/* Texte d'aide discret sous le titre */
void ui_draw_caption(int y, const char *text);

/* Resultat : lignes simples + ligne resultat en jaune */
void ui_draw_result_lines(int y, const char *const *lines, int line_count, int highlight_line);

void ui_draw_message_screen(const char *title, const char *line1, const char *line2);

eadk_event_t ui_wait_event(void);
void ui_wait_key_release(void);
bool ui_is_back(eadk_event_t event);
bool ui_is_confirm(eadk_event_t event);
bool ui_is_nav(eadk_event_t event);

void ui_format_number(char *buffer, size_t size, float value);
bool ui_parse_number(const char *text, float *out);

#endif