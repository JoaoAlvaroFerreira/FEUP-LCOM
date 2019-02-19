// IMPORTANT: you must include the following line in all your C files
#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard_standalone.h"
#include "timer3.h"

uint32_t scanCode;
uint16_t modeGlobal;
int extern Counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  vg_init(mode);

  tickdelay(micros_to_ticks(delay * GRAPHIC_SECONDS_DELAY));

  if (vg_exit() != OK) {
    printf("ERROR: Issues changing to text mode.");
    return 1;
  }
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  vg_init(mode);

  int ipc_status;
  message msg;
  int bit = 1;
  int r;

  vg_draw_rectangle(x, y, width, height, color);

  if (k_subscribe_int(bit) == ERROR) {
    printf("ERROR: Subscribe failed");
  }

  while (scanCode != ESC_KEY_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              // hardware interrupt notification
          if (msg.m_notify.interrupts & BIT(bit)) { // subscribed interrupt

            scanCodeReader();
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (k_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }

  vg_exit();
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  vg_init(mode);

  int ipc_status;
  message msg;
  int bit = 1;
  int r;

  vg_draw_pattern(no_rectangles, first, step);

  if (k_subscribe_int(bit) == ERROR) {
    printf("ERROR: Subscribe failed");
  }

  while (scanCode != ESC_KEY_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              // hardware interrupt notification
          if (msg.m_notify.interrupts & BIT(bit)) { // subscribed interrupt

            scanCodeReader();
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (k_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }

  vg_exit();
  return 0;
}

int(video_test_xpm)(const char *xpm[], uint16_t xi, uint16_t yi) {

  vg_init(0x105);

  int ipc_status;
  message msg;
  int bit = 1;
  int r;

  int wd, hg;
  char *pixmap = NULL;
  pixmap = read_xpm(xpm, &wd, &hg);

  if (vg_draw_pixmap(pixmap, xi, yi, wd, hg) != OK) {
    printf("Failed to draw pixmap\n");
    return 1;
  }

  if (k_subscribe_int(bit) == ERROR) {
    printf("ERROR: Subscribe failed");
  }

  while (scanCode != ESC_KEY_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                              // hardware interrupt notification
          if (msg.m_notify.interrupts & BIT(bit)) { // subscribed interrupt

            scanCodeReader();
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (k_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }

  vg_exit();
  return 0;
}

int(video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {

  vg_init(0x105);

  int ipc_status;
  message msg;
  int keyboard_bit = 1;
  int timer_bit = 0;
  int r;

  int wd, hg;
  char *pixmap = NULL;
  pixmap = read_xpm(xpm, &wd, &hg);

  unsigned int y = yi;
  unsigned int x = xi;

  if (vg_draw_pixmap(pixmap, xi, yi, wd, hg) != OK) {
    printf("Failed to draw pixmap\n");
    return 1;
  }

  if (k_subscribe_int(keyboard_bit) == ERROR) {
    printf("ERROR: Subscribe failed");
  }

  if (timer_subscribe_int3(timer_bit) == ERROR) {
    printf("ERROR: subscribe timer failed");
    return 1;
  }

  while (scanCode != ESC_KEY_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                       // hardware interrupt notification
          if (msg.m_notify.interrupts & BIT(keyboard_bit)) { // subscribed interrupt
             scanCodeReader();

          }
          else if (msg.m_notify.interrupts & BIT(timer_bit)) {
              timer_int_handler();
            
                 if(x < xf) {

                  for(int frames = 0; frames < fr_rate; frames++){
             vg_draw_pixmap(pixmap, x, y, wd, hg);
              doubleBuffering();
              
               }
               x = x + speed;
               
            }

            else if(y < yf) {

for(int frames = 0; frames < fr_rate; frames++){
             vg_draw_pixmap(pixmap, x, y, wd, hg);
               doubleBuffering();
            }
            y = y + speed;
           
           
          }
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (timer_unsubscribe_int() == ERROR) {
    printf("ERROR: subscribe timer failed");
    return 1;
  }

  if (k_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }

  vg_exit();
  return 0;
}


int (video_test_controller)()
{ 
  vg_vbe_contr_info_t info_p;
  memset(&info_p, 0, sizeof(vg_vbe_contr_info_t));
  read_vbe(&info_p);

  vg_display_vbe_contr_info(&info_p);
  return 0;
}
