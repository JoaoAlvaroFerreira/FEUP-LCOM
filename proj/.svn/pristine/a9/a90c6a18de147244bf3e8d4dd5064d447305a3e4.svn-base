#pragma once
#include <minix/sysutil.h>
#include <stdbool.h>

int(mouse_subscribe_int)(int bit_no);
int(mouse_unsubscribe_int)();
void(mouse_ih)();
int mouseByteReader();
int mouseByteReaderRemote();
int restoreStateDefault();
int dataReportingEnable();
int dataReportingDisable();
int remoteReadData();
int enableStreamMode();
float slope(int x, int y);


/*
struct packet {
    uint8_t bytes[3];
    bool rb;
    bool mb;
    bool lb;
    int16_t delta_x;
    int16_t delta_y;
    bool x_ov;
    bool y_ov;
}; */
