#pragma once
#include <minix/sysutil.h>
#include <stdbool.h>

int k_subscribe_int(int bit_no);
int k_unsubscribe_int();
int enableIH();
unsigned int cmdByteRead();
unsigned int cmdByteWrite(unsigned int data);
void scanCodeReader();
void kb_ih();
