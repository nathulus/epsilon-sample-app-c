#include <eadk.h>

#include "app.h"

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Autre";
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);
  autre_app_run();
  return 0;
}