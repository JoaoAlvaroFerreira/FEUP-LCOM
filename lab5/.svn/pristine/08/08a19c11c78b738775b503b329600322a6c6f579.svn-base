#pragma once
#include <minix/sysutil.h>

int video_get_mode_info(uint16_t mode, vbe_mode_info_t *vbe);
void vg_draw_pattern(uint8_t no_rectangles,uint32_t first,uint8_t step);
int vg_draw_pixmap(char* pixmap,uint16_t xi,uint16_t yi, int wd, int hg);
int clearScreen();
void doubleBuffering();
int read_vbe(vg_vbe_contr_info_t *info_p);
