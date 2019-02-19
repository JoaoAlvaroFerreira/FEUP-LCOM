#pragma once
#include <minix/sysutil.h>
#include <stdbool.h>

#ifdef LAB3
  extern uint32_t cnt;
#endif

int(kb_subscribe_int)(int bit_no);
int(kb_unsubscribe_int)();
int(writeKBC)();
int(readKBC)();
int enableIH();
unsigned int cmdByteRead();
unsigned int cmdByteWrite(unsigned int data);
void scanCodeReader(bool assembly);
void(kbc_asm_ih)(void);
void(kbc_ih)(void);
