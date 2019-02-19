#include <lcom/lcf.h>
#include "timer3.h"
#include <stdint.h>
#include "i8254.h"


static int hook_id = 0;
int counter = 0;

/**
 * @brief sets the timer's frequency
 * 
 */

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if(!(freq > MINIMUM_FREQUENCY)){
    printf("ERROR: Frequency must be above 18. \n");
    return 1;
  }


  if(!(freq < MAXIMUM_FREQUENCY)){
    printf("ERROR: Frequency must be below 65,537. \n");
    return 1;
  }

  if(!(timer == 0 || timer == 1 || timer == 2)){
    printf("ERROR: Timer must be 0, 1 or 2. \n");
    return 1;
  }

  uint16_t frequency = TIMER_FREQ/freq;
  uint8_t TIMER_AUX = TIMER_0 + timer;
  uint8_t TIMER_SEL_AUX = TIMER_SEL0 + timer;
  uint32_t st;
  uint8_t lsb = 0;
  uint8_t msb = 0;

  uint8_t oc;
  timer_get_conf(timer, &oc);

  util_get_LSB(frequency, &lsb);
  util_get_MSB(frequency, &msb);

  st = (uint16_t) oc;

  st = (st & 0x0f) | TIMER_SEL_AUX | TIMER_LSB_MSB;


  if(sys_outb(TIMER_CTRL, st) != OK)
    printf("ERROR: Failed to write to control register. \n");

  if (sys_outb(TIMER_AUX, lsb) != OK)
    printf("ERROR: Failed to write to the specific timer (LSB). \n");

  if (sys_outb(TIMER_AUX, msb) != OK)
    printf("ERROR: Failed to write to the specific timer (MSB). \n");


  return 1;
}

/**
 * @brief subscribes timer interrupts
 * 
 */
int (timer_subscribe_int3)(uint8_t bit_no) {

  hook_id = bit_no;
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK)
    return 1;

  return 0;
}

/**
 * @brief unsubscribes timer interrupts
 * 
 */
int (timer_unsubscribe_int)() {

  if (sys_irqrmpolicy(&hook_id) != OK)
    return 1;

  return 0;
}
/**
 * @brief increases the timer counter
 * 
 */

void (timer_int_handler)() {


 counter++;


}

/**
 * @brief gets timer configuration
 * 
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  char TIMER_AUX;
  uint32_t stAux;

  if(!(timer == 0 || timer == 1 || timer == 2)){
    printf("ERROR: Timer must be 0, 1 or 2");
    return 1;
  }

  switch(timer){ //Chooses correct timer

    case 0:
      TIMER_AUX = TIMER_0;
      break;
    case 1:
      TIMER_AUX = TIMER_1;
      break;
    case 2:
      TIMER_AUX = TIMER_2;
      break;
    default:
      printf("ERROR: Timer must be 0, 1 or 2");
      return 1;
  }

  unsigned long RBCMD = TIMER_RB_COUNT_ | TIMER_RB_CMD | TIMER_RB_SEL(timer);

  if(sys_outb(TIMER_CTRL, RBCMD) != OK)
    return 1;

  if(sys_inb(TIMER_AUX, &stAux) != OK)
    return 1;

  *st = (uint8_t)stAux;


  return 1;
}

/**
 * @brief displays timer configuration
 * 
 */

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  uint8_t modo = st & MODE_COMPARISON;
  modo = modo>>1;

  if(modo == UNEXISTENT_MODE_1)
    modo = RATE_GENERATOR;
  else if(modo == UNEXISTENT_MODE_2)
    modo = SQUARE_WAVE_MODE;

  unsigned int base_numero = st & TIMER_BCD;
  union timer_status_field_val valor;

    if(field == all) //Displays status byte in hexadecimal
      valor.byte = st;


    if(field == initial){ //Displays initialization mode
     if((st | COMPARE_VALUE) == LSB_ONLY_MODE)
       valor.in_mode = LSB_only;
     else if((st | COMPARE_VALUE) == MSB_ONLY_MODE)
       valor.in_mode = MSB_only;
     else if((st | COMPARE_VALUE) == MSB_AFTER_LSB_MODE)
       valor.in_mode = MSB_after_LSB;
     else
      valor.in_mode = INVAL_val;}

   if(field == mode) //Displays counting mode
    valor.count_mode = modo;

   if(field == base) //Displays counting base
    valor.bcd = base_numero;



  timer_print_config(timer, field, valor);
  return 0;

}
