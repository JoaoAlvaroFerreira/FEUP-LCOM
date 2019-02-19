// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
// Any header files included below this line should have been created by you
#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "timer3.h"
#include <stdbool.h>
#include <stdint.h>

extern uint32_t cnt;
extern int failed;
uint32_t scanCode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)(bool assembly) {

  int ipc_status;
  message msg;
  int bit = 1;
  int r;

  if (kb_subscribe_int(bit) == ERROR) {
    printf("ERROR: Subscribe failed");
  }

  while (scanCode != ESC_KEY_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification

      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: // hardware interrupt notification
        if (msg.m_notify.interrupts & BIT(bit)) { // subscribed interrupt

        scanCodeReader(assembly);

        }
        break;
      default:
        break; // no other notifications expected: do nothing
      }
    } else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  if(!assembly){
#ifdef LAB3
  kbd_print_no_sysinb(cnt);
#endif
  }

  if (kb_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }

  return 0;
}

int(kbd_test_poll)() {

  bool STOP = false;

  while (!STOP) {

    scanCodeReader(false);

    tickdelay(micros_to_ticks(DELAY_US));

    if (scanCode == ESC_KEY_BREAK)
      STOP = true;
  }

  #ifdef LAB3
  kbd_print_no_sysinb(cnt);
#endif
  

  if (enableIH() != OK) {
    printf("\n ERROR: EnableIH failed.");
    return 1;
  }

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {

  int ipc_status;
  message msg;

  int kb_bit_no = 1;
  uint8_t timer_bit_no = 0;

  int r;

  if (kb_subscribe_int(kb_bit_no) == ERROR) {
    printf("ERROR: Subscribe failed");
  }
  if (timer_subscribe_int3(timer_bit_no) == ERROR) {
    printf("ERROR: subscribe timer failed");
    return 1;
  }

  while ((counter < n * 60) && (scanCode != ESC_KEY_BREAK)) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification

      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: // hardware interrupt notification
        if (msg.m_notify.interrupts & BIT(kb_bit_no)) { // subscribed interrupt

          scanCodeReader(false);
          counter = 0;
        } else if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
          timer_int_handler();
        }
        break;
      default:
        break; // no other notifications expected: do nothing
      }
    } else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (kb_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe KB failed");
    return 1;
  }
  if (timer_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe timer failed");
    return 1;
  }
  return 0;
}
