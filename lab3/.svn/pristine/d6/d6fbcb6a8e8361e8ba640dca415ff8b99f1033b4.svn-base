#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"

int (kbd_test_scan)(bool assembly);

int (kbd_test_poll)();

int (kbd_test_timed_scan)(uint8_t n);

void (kbc_asm_ih)(void);

void (kbc_ih)(void);

int (kbd_print_scancode)(bool make, uint8_t size, uint8_t *bytes);

int (kbd_print_no_sysinb)(uint32_t cnt);
