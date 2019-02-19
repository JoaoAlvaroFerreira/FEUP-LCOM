#pragma once
#include <minix/sysutil.h>
#include <lcom/lcf.h>


int video_get_mode_info(uint16_t mode, vbe_mode_info_t *vbe);
int vg_init_11a();
void vg_draw_pattern(uint8_t no_rectangles,uint32_t first,uint8_t step);
int vg_draw_pixmap(char* pixmap,uint16_t xi,uint16_t yi, int wd, int hg);
int clearScreen();
int clearScreenPos(int x_min, int x_max, int y_min, int y_max);
void doubleBuffering();
int read_vbe(vg_vbe_contr_info_t *info_p);
int getHorResolution();
int getVerResolution();
uint16_t* getGraphicsBuffer();
