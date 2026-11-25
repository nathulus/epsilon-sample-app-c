#include "app.h"

#include <stddef.h>

#include "menu/menu.h"
#include "modules/formules/formules.h"

static void autre_main_menu_loop(void) {
  const autre_menu_item_t items[] = {
      {"Formules", autre_formules_run},
      {"Electricite", NULL},
      {"Mecanique", NULL},
      {"Physique", NULL},
      {"Conversions", NULL},
      {"Mes programmes", NULL},
  };
  autre_menu_run("Autre", items, sizeof(items) / sizeof(items[0]));
}

void autre_app_run(void) {
  autre_main_menu_loop();
}