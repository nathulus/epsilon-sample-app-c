#ifndef AUTRE_MENU_H
#define AUTRE_MENU_H

typedef struct {
  const char *label;
  void (*action)(void);
} autre_menu_item_t;

/* Affiche une liste et appelle action sur OK. action NULL = écran « bientôt ». */
void autre_menu_run(const char *title, const autre_menu_item_t *items, int count);

#endif