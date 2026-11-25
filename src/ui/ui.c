#include "ui.h"

#include <stdio.h>
#include <string.h>

void ui_clear(void) {
  eadk_display_push_rect_uniform(eadk_screen_rect, AUTRE_COLOR_WHITE);
}

void ui_draw_title(const char *title) {
  eadk_rect_t bar = {0, 0, EADK_SCREEN_WIDTH, AUTRE_TITLE_H};
  eadk_display_push_rect_uniform(bar, AUTRE_COLOR_TITLE);
  /* Petite police centree verticalement dans la barre titre (style Epsilon) */
  ui_draw_string(5, 3, title, false, AUTRE_COLOR_BLACK, AUTRE_COLOR_TITLE);
}

void ui_begin_screen(const char *title) {
  ui_clear();
  ui_draw_title(title);
}

void ui_draw_string(int x, int y, const char *text, bool large, eadk_color_t fg, eadk_color_t bg) {
  eadk_display_draw_string(text, (eadk_point_t){(uint16_t)x, (uint16_t)y}, large, fg, bg);
}

void ui_draw_menu_list(const char *const *labels, int count, int selected, int top_y) {
  for (int i = 0; i < count; i++) {
    int y = top_y + i * AUTRE_ROW_H;
    eadk_color_t bg = AUTRE_COLOR_WHITE;
    if (i == selected) {
      eadk_rect_t row = {0, (uint16_t)y, EADK_SCREEN_WIDTH, AUTRE_ROW_H};
      eadk_display_push_rect_uniform(row, AUTRE_COLOR_YELLOW);
      bg = AUTRE_COLOR_YELLOW;
    }
    ui_draw_string(14, y + 9, labels[i], true, AUTRE_COLOR_BLACK, bg);
  }
}

void ui_draw_caption(int y, const char *text) {
  ui_draw_string(14, y, text, false, AUTRE_COLOR_GRAY, AUTRE_COLOR_WHITE);
}

void ui_draw_param_row(int y, const char *label, const char *value, bool selected) {
  eadk_color_t bg = AUTRE_COLOR_WHITE;
  if (selected) {
    eadk_rect_t row = {0, (uint16_t)y, EADK_SCREEN_WIDTH, AUTRE_ROW_H};
    eadk_display_push_rect_uniform(row, AUTRE_COLOR_YELLOW);
    bg = AUTRE_COLOR_YELLOW;
  }
  ui_draw_string(14, y + 9, label, true, AUTRE_COLOR_BLACK, bg);
  const char *shown = (value != NULL && value[0] != '\0') ? value : "0";
  /* Valeur alignee a droite approximativement */
  int len = (int)strlen(shown);
  int x = EADK_SCREEN_WIDTH - 14 - len * 10;
  if (x < 160) {
    x = 160;
  }
  ui_draw_string(x, y + 9, shown, true, AUTRE_COLOR_BLACK, bg);
}

void ui_draw_action_row(int y, const char *label, bool selected) {
  eadk_color_t bg = AUTRE_COLOR_WHITE;
  if (selected) {
    eadk_rect_t row = {0, (uint16_t)y, EADK_SCREEN_WIDTH, AUTRE_ROW_H};
    eadk_display_push_rect_uniform(row, AUTRE_COLOR_YELLOW);
    bg = AUTRE_COLOR_YELLOW;
  }
  ui_draw_string(14, y + 9, label, true, AUTRE_COLOR_BLACK, bg);
}

void ui_draw_result_lines(int y, const char *const *lines, int line_count, int highlight_line) {
  int line_y = y;
  for (int i = 0; i < line_count; i++) {
    if (lines[i] == NULL) {
      continue;
    }
    if (i == highlight_line) {
      eadk_rect_t row = {0, (uint16_t)(line_y - 4), EADK_SCREEN_WIDTH, AUTRE_ROW_H};
      eadk_display_push_rect_uniform(row, AUTRE_COLOR_YELLOW);
      ui_draw_string(14, line_y + 5, lines[i], true, AUTRE_COLOR_BLACK, AUTRE_COLOR_YELLOW);
      line_y += AUTRE_ROW_H + 4;
    } else {
      ui_draw_string(14, line_y, lines[i], true, AUTRE_COLOR_BLACK, AUTRE_COLOR_WHITE);
      line_y += 22;
    }
  }
}

void ui_draw_message_screen(const char *title, const char *line1, const char *line2) {
  ui_begin_screen(title);
  ui_draw_string(14, 80, line1, true, AUTRE_COLOR_BLACK, AUTRE_COLOR_WHITE);
  if (line2 != NULL) {
    ui_draw_string(14, 110, line2, false, AUTRE_COLOR_GRAY, AUTRE_COLOR_WHITE);
  }
}

static bool ui_event_is_useful(eadk_event_t event) {
  if (event == eadk_event_up || event == eadk_event_down || event == eadk_event_left ||
      event == eadk_event_right || event == eadk_event_ok || event == eadk_event_exe ||
      event == eadk_event_back || event == eadk_event_backspace || event == eadk_event_dot ||
      event == eadk_event_minus) {
    return true;
  }
  if (event == eadk_event_zero || event == eadk_event_one || event == eadk_event_two ||
      event == eadk_event_three || event == eadk_event_four || event == eadk_event_five ||
      event == eadk_event_six || event == eadk_event_seven || event == eadk_event_eight ||
      event == eadk_event_nine) {
    return true;
  }
  return false;
}

void ui_wait_key_release(void) {
  eadk_timing_msleep(180);
}

eadk_event_t ui_wait_event(void) {
  while (true) {
    int32_t timeout = -1;
    eadk_event_t event = eadk_event_get(&timeout);
    if (!ui_event_is_useful(event)) {
      eadk_timing_msleep(10);
      continue;
    }
    ui_wait_key_release();
    return event;
  }
}

bool ui_is_back(eadk_event_t event) {
  return event == eadk_event_back;
}

bool ui_is_confirm(eadk_event_t event) {
  return event == eadk_event_ok || event == eadk_event_exe;
}

bool ui_is_nav(eadk_event_t event) {
  return event == eadk_event_up || event == eadk_event_down || event == eadk_event_left ||
         event == eadk_event_right;
}

void ui_format_number(char *buffer, size_t size, float value) {
  if (size == 0) {
    return;
  }
  int whole = (int)value;
  int tenths = (int)((value - (float)whole) * 10.0f);
  if (tenths < 0) {
    tenths = -tenths;
  }
  if (tenths > 9) {
    tenths = 9;
  }
  if (value < 0.0f && whole == 0) {
    snprintf(buffer, size, "-0.%d", tenths);
  } else if (tenths == 0) {
    snprintf(buffer, size, "%d", whole);
  } else {
    snprintf(buffer, size, "%d.%d", whole, tenths);
  }
}

bool ui_parse_number(const char *text, float *out) {
  if (text == NULL || text[0] == '\0') {
    *out = 0.0f;
    return true;
  }
  int sign = 1;
  int index = 0;
  if (text[0] == '-') {
    sign = -1;
    index = 1;
  }
  int whole = 0;
  int frac = 0;
  int frac_digits = 0;
  while (text[index] >= '0' && text[index] <= '9') {
    whole = whole * 10 + (text[index] - '0');
    index++;
  }
  if (text[index] == '.') {
    index++;
    while (text[index] >= '0' && text[index] <= '9' && frac_digits < 3) {
      frac = frac * 10 + (text[index] - '0');
      frac_digits++;
      index++;
    }
  }
  float value = (float)whole;
  if (frac_digits > 0) {
    float divisor = 1.0f;
    for (int i = 0; i < frac_digits; i++) {
      divisor *= 10.0f;
    }
    value += (float)frac / divisor;
  }
  *out = (float)sign * value;
  return true;
}