#include "menu.h"

#include "../ui/ui.h"
#include <stddef.h>
#include <stdio.h>

static void show_stub(const char *name) {
  char line[48];
  snprintf(line, sizeof(line), "%s", name);
  ui_draw_message_screen("Autre", line, "Bientot disponible");
  while (true) {
    eadk_event_t event = ui_wait_event();
    if (ui_is_back(event)) {
      return;
    }
  }
}

void autre_menu_run(const char *title, const autre_menu_item_t *items, int count) {
  if (count <= 0) {
    return;
  }
  int selected = 0;
  const char *labels[16];
  int label_count = count > 16 ? 16 : count;
  bool need_redraw = true;

  for (int i = 0; i < label_count; i++) {
    labels[i] = items[i].label;
  }

  while (true) {
    if (need_redraw) {
      ui_begin_screen(title);
      ui_draw_menu_list(labels, label_count, selected, AUTRE_LIST_TOP);
      need_redraw = false;
    }

    eadk_event_t event = ui_wait_event();
    if (ui_is_back(event)) {
      return;
    }
    if (event == eadk_event_up) {
      selected = (selected - 1 + label_count) % label_count;
      need_redraw = true;
    } else if (event == eadk_event_down) {
      selected = (selected + 1) % label_count;
      need_redraw = true;
    } else if (ui_is_confirm(event)) {
      if (items[selected].action != NULL) {
        items[selected].action();
      } else {
        show_stub(items[selected].label);
      }
      need_redraw = true;
    }
  }
}