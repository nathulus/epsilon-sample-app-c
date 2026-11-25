#include "formules.h"

#include "../../menu/menu.h"
#include "../../ui/ui.h"

#include <stdio.h>
#include <string.h>

typedef float (*autre_formula_compute_fn)(float a, float b);

typedef struct {
  const char *category;
  const char *title;
  const char *formula_text;
  const char *result_unit;
  char result_symbol;
  const char *var_a;
  const char *unit_a;
  const char *var_b;
  const char *unit_b;
  autre_formula_compute_fn compute;
} autre_formula_t;

static float compute_puissance(float u, float i) {
  return u * i;
}

static const autre_formula_t g_formulas[] = {
    {
        "Electricite",
        "Puissance",
        "P = U x I",
        "W",
        'P',
        "U",
        "V",
        "I",
        "A",
        compute_puissance,
    },
};

typedef struct {
  char u_text[16];
  char i_text[16];
} puissance_input_t;

static void puissance_draw_input(const puissance_input_t *input, int selected_field) {
  ui_begin_screen("Puissance");
  ui_draw_caption(AUTRE_LIST_TOP + 4, "P = U x I");
  ui_draw_param_row(AUTRE_LIST_TOP + 22, "U (V)", input->u_text, selected_field == 0);
  ui_draw_param_row(AUTRE_LIST_TOP + 22 + AUTRE_ROW_H, "I (A)", input->i_text,
                    selected_field == 1);
  ui_draw_action_row(AUTRE_LIST_TOP + 22 + 2 * AUTRE_ROW_H + 8, "Calculer",
                     selected_field == 2);
}

static bool append_digit(char *buffer, size_t size, char digit) {
  size_t len = strlen(buffer);
  if (len + 1 >= size) {
    return false;
  }
  buffer[len] = digit;
  buffer[len + 1] = '\0';
  return true;
}

static void backspace_digit(char *buffer) {
  size_t len = strlen(buffer);
  if (len == 0) {
    return;
  }
  buffer[len - 1] = '\0';
}

static int event_to_digit(eadk_event_t event) {
  switch (event) {
    case eadk_event_zero:
      return 0;
    case eadk_event_one:
      return 1;
    case eadk_event_two:
      return 2;
    case eadk_event_three:
      return 3;
    case eadk_event_four:
      return 4;
    case eadk_event_five:
      return 5;
    case eadk_event_six:
      return 6;
    case eadk_event_seven:
      return 7;
    case eadk_event_eight:
      return 8;
    case eadk_event_nine:
      return 9;
    default:
      return -1;
  }
}

static void handle_digit_event(puissance_input_t *input, int selected_field, eadk_event_t event) {
  char *active = selected_field == 0 ? input->u_text : input->i_text;
  size_t active_size = sizeof(input->u_text);

  int digit = event_to_digit(event);
  if (digit >= 0) {
    append_digit(active, active_size, (char)('0' + digit));
  } else if (event == eadk_event_dot) {
    if (strchr(active, '.') == NULL) {
      if (active[0] == '\0') {
        append_digit(active, active_size, '0');
      }
      append_digit(active, active_size, '.');
    }
  } else if (event == eadk_event_backspace) {
    backspace_digit(active);
  } else if (event == eadk_event_minus) {
    if (active[0] == '-') {
      memmove(active, active + 1, strlen(active));
      active[strlen(active)] = '\0';
    } else if (strlen(active) + 1 < active_size) {
      char temp[16];
      snprintf(temp, sizeof(temp), "-%s", active);
      strncpy(active, temp, active_size - 1);
      active[active_size - 1] = '\0';
    }
  }
}

static void puissance_show_result(const autre_formula_t *formula, float u, float i, float p) {
  char u_buf[16];
  char i_buf[16];
  char p_buf[16];
  char line1[40];
  char line2[40];
  char line3[40];
  char line4[40];
  char line5[40];

  ui_format_number(u_buf, sizeof(u_buf), u);
  ui_format_number(i_buf, sizeof(i_buf), i);
  ui_format_number(p_buf, sizeof(p_buf), p);

  snprintf(line1, sizeof(line1), "%s", formula->formula_text);
  snprintf(line2, sizeof(line2), "U = %s %s", u_buf, formula->unit_a);
  snprintf(line3, sizeof(line3), "I = %s %s", i_buf, formula->unit_b);
  snprintf(line4, sizeof(line4), "P = %s x %s", u_buf, i_buf);
  snprintf(line5, sizeof(line5), "P = %s %s", p_buf, formula->result_unit);

  const char *lines[] = {line1, line2, line3, line4, line5};

  ui_begin_screen("Resultat");
  ui_draw_result_lines(AUTRE_LIST_TOP + 10, lines, 5, 4);

  while (true) {
    eadk_event_t event = ui_wait_event();
    if (ui_is_back(event) || ui_is_confirm(event)) {
      return;
    }
  }
}

static void run_puissance_formula(const autre_formula_t *formula) {
  puissance_input_t input;
  strncpy(input.u_text, "12", sizeof(input.u_text));
  strncpy(input.i_text, "2", sizeof(input.i_text));
  input.u_text[sizeof(input.u_text) - 1] = '\0';
  input.i_text[sizeof(input.i_text) - 1] = '\0';

  int selected_field = 0;
  bool need_redraw = true;

  while (true) {
    if (need_redraw) {
      puissance_draw_input(&input, selected_field);
      need_redraw = false;
    }

    eadk_event_t event = ui_wait_event();
    if (ui_is_back(event)) {
      return;
    }
    if (event == eadk_event_up) {
      selected_field = (selected_field - 1 + 3) % 3;
      need_redraw = true;
    } else if (event == eadk_event_down) {
      selected_field = (selected_field + 1) % 3;
      need_redraw = true;
    } else if (ui_is_confirm(event) && selected_field < 2) {
      selected_field++;
      need_redraw = true;
    } else if (ui_is_confirm(event) && selected_field == 2) {
      float u = 0.0f;
      float i = 0.0f;
      if (!ui_parse_number(input.u_text, &u) || !ui_parse_number(input.i_text, &i)) {
        continue;
      }
      float p = formula->compute(u, i);
      puissance_show_result(formula, u, i, p);
      need_redraw = true;
    } else if (selected_field == 0 || selected_field == 1) {
      handle_digit_event(&input, selected_field, event);
      need_redraw = true;
    }
  }
}

static void menu_puissance_electricite(void) {
  run_puissance_formula(&g_formulas[0]);
}

static void formules_electricite_menu(void) {
  const autre_menu_item_t items[] = {
      {"Puissance P = U x I", menu_puissance_electricite},
  };
  autre_menu_run("Electricite", items, sizeof(items) / sizeof(items[0]));
}

void autre_formules_run(void) {
  const autre_menu_item_t items[] = {
      {"Electricite", formules_electricite_menu},
  };
  autre_menu_run("Formules", items, sizeof(items) / sizeof(items[0]));
}