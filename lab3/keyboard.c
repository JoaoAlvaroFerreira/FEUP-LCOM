#include "keyboard.h"
#include "i8042.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdint.h>

// int hook_id = 1;

uint32_t cnt = 0;
extern uint32_t scanCode;
int double_byte_flag = 0;
uint8_t bytes[2];
bool failure;
int size;
bool make;
static int hook_id;

void(kbc_asm_ih)();
unsigned long asmFailure;
unsigned long codeScanned;

#ifdef LAB3
int sys_inb_count(port_t port, uint32_t *byte) {
  cnt++;
  return sys_inb(port, byte);
}
#else
#define sys_inb_count(p, q) sys_inb(p, q)
#endif

int kb_subscribe_int(int bit_no) {

  hook_id = bit_no;
  if (sys_irqsetpolicy(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)
    return 1;

  return 0;
}

int kb_unsubscribe_int() {

  if (sys_irqrmpolicy(&hook_id) != OK)
    return 1;

  return 0;
}

unsigned int cmdByteRead() {

  uint32_t data = 0;
  uint32_t stat = 0;

  if (sys_outb(KBC_CMD_REG, READ_CMD_BYTE) != OK) {
    printf("\n ERROR: Writing to KBC_CMD_REG failed.");
    return ERROR;
  }

  while (1) { //Loops until the data is read from the OUT_BUF, at which point it returns

    sys_inb(STAT_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if (stat & OBF) {
      sys_inb_count(OUT_BUF, &data); /* assuming it returns OK */
      return data;
    }

    tickdelay(micros_to_ticks(DELAY_US));
  }

  return ERROR;
}

unsigned int cmdByteWrite(unsigned int data) {

  if (sys_outb(KBC_CMD_REG, WRITE_CMD_BYTE) != OK) {
    printf("\n ERROR: Reading from KBC_CMD_REG failed.");
    return ERROR;
  }

  if (sys_outb(WRITE_CMD_BYTE, data) != OK) {
    printf("\n ERROR: Writing to WRITE_CMD_BYTE failed.");
    return ERROR;
  }

  return 0;
}

int enableIH() {

  unsigned long data;

  data = cmdByteRead();

  data = data | BIT(0);

  if (cmdByteWrite(data) != OK) {
    printf("\n ERROR: Writing to KBC_CMD_REG failed.");
    return ERROR;
  }

  return OK;
}


void scanCodeReader(bool assembly) {

  if(assembly){ //To use the IH written in Assembly
  kbc_asm_ih();
  scanCode = codeScanned;
  if (asmFailure)
    return;
  }

  else{ //To use the IH written in C
  kbc_ih();
  if (failure)
    return;
  }

  if ((scanCode & BREAKCODE_CHECK) == BREAKCODE_CHECK)//Checks if a ScanCode is a Make or BreakCode
    make = false;
  else
    make = true;

  if ((scanCode == DOUBLE_BYTE_SCAN1) ||
   (scanCode == DOUBLE_BYTE_SCAN2)) { //Checks if it's a ScanCode that is two bytes long or not
    double_byte_flag = 1;
    bytes[0] = scanCode;
    size = 2;
    return;
  } else {
    if (double_byte_flag == 1) {
      double_byte_flag = 0;
      bytes[1] = scanCode;
      size = 2;
    } else {
      bytes[0] = scanCode;
      size = 1;
    }
  }

#ifdef LAB3
  kbd_print_scancode(make, size, bytes); //Prints the scancode
#endif
}

void(kbc_ih)(void) {

  uint32_t stat;

  sys_inb_count(STAT_REG, &stat); /* assuming it returns OK */
  /* loop while 8042 output buffer is empty */
  if ((stat & OBF) && (!(stat & AUX))) {

    if ((stat & (PAR_ERR | TO_ERR)) == 0) {
      sys_inb_count(OUT_BUF, &scanCode); /* assuming it returns OK */
      failure = false;
      return;
    } else {
      failure = true;
      return;
    }
  } else
    failure = true;

  return;
}
