#include <lcom/lcf.h>
#include <machine/int86.h>
#include "graphics.h"
#include "i8042.h"

static uint16_t modeAux;
uint16_t hres; /* XResolution */
uint16_t vres; /* YResolution */
static uint16_t bpp;
static uint16_t bytespp;
static uint8_t RedScreenMask;
static uint8_t RedFieldPosition;
static uint8_t BlueScreenMask;
static uint8_t BlueFieldPosition;
static uint8_t GreenScreenMask;
static uint8_t GreenFieldPosition;

static uint16_t *vmem;
static uint16_t *double_buffer;

/**
 * @brief initializes the graphical mode
 * 
 */
void *(vg_init)(uint16_t mode) {
  lm_init(false);
  modeAux = mode;
  struct reg86u reg86;

  memset(&reg86, 0, sizeof(reg86)); /* zero the structure */

  reg86.u.b.intno = 0x10;
  reg86.u.b.ah = 0x4F; /* BIOS video services */
  reg86.u.b.al = 0x02; /* Set Video Mode function */
  reg86.u.w.bx = 1 << 14 | mode;

  if (sys_int86(&reg86) != OK) {
    printf("\tvg_exit(): sys_int86() failed \n");
  }

  vbe_mode_info_t vbe;
  video_get_mode_info(mode, &vbe); //NÃO ESQUECER, IMPLEMENTAR DEPOIS

  int r;
  struct minix_mem_range mr;                                                           /* physical memory range */
  unsigned int vram_base = (unsigned int) vbe.PhysBasePtr;                             /* VRAM’s physical addresss */
  unsigned int vram_size = vbe.XResolution * vbe.YResolution * (vbe.BitsPerPixel / 8); /* VRAM’s size, but you can use the frame-buffer size, instead */
  void *video_mem;
  /* frame-buffer VM address */
  /* Allow memory mapping */

  vres = vbe.YResolution;
  hres = vbe.XResolution;
  bpp = vbe.BitsPerPixel;
  RedScreenMask = vbe.RedMaskSize;
  RedFieldPosition = vbe.RedFieldPosition;
  GreenScreenMask = vbe.GreenMaskSize;
  GreenFieldPosition = vbe.GreenFieldPosition;
  BlueScreenMask = vbe.BlueMaskSize;
  BlueFieldPosition = vbe.BlueFieldPosition;

  bytespp = bpp / 8;
  if (bpp % 8 > 0)
    bpp++;

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
  double_buffer = (uint16_t *) malloc(hres * vres * bytespp);

  if (video_mem == MAP_FAILED)
    panic("couldn’t map video memory");
  vmem = video_mem;
  return video_mem;
}

/**
 * @brief initializes graphical mode 0x11a
 * 
 * @return int 
 */
int vg_init_11a(){
  
  vg_init(0x11a);
  return 0;
}


/**
 * @brief gets the vbe info of graphical modes
 * 
 * @param mode 
 * @param vbe 
 * @return int 
 */

int video_get_mode_info(uint16_t mode, vbe_mode_info_t *vbe) {
  struct reg86u r;
  phys_bytes buf;

  memset(&r, 0, sizeof(struct reg86u)); /* zero the structure */


  mmap_t mem_map;
  lm_alloc(sizeof(vbe_mode_info_t), &mem_map);
  buf = mem_map.phys;

  r.u.w.ax = 0x4F01; 
  r.u.w.es = PB2BASE(buf); 
  r.u.w.di = PB2OFF(buf);  
  r.u.w.cx = mode;
  r.u.b.intno = 0x10;

   
  if (sys_int86(&r) != OK) {
    lm_free(&mem_map);
    return 1;
  }

  *vbe = *(vbe_mode_info_t *)mem_map.virt;

  lm_free(&mem_map);
  return 0;
} 

/**
 * @brief draws a rectangle on screen
 * 
 */
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  
  
  for (uint16_t h = y; h < y + height; h++) {

    if (vg_draw_hline(x, h, width, color) != OK) {
      printf("ERROR: Issue writing line at height = %d\n", h);
      return 1;
    }
  }

 
  return 0;
}

/**
 * @brief draws a line on screen
 * 
 */
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {


  for (uint16_t l = x; l < x + len; l++) {
    memcpy(vmem + l * bytespp + (y * hres * bytespp), &color, bytespp);
  }

  return 0;
}
/**
 * @brief clears the screen
 * 
 * @return int 
 */

int clearScreen() {

  memset(vmem, 0, hres*vres*bytespp);
  memset(double_buffer, 0, hres*vres*bytespp);
  return 0;
}

/**
 * @brief clears screen at specific positions
 * 
 * @param x_min 
 * @param x_max 
 * @param y_min 
 * @param y_max 
 * @return int 
 */

int clearScreenPos(int x_min, int x_max, int y_min, int y_max) {
  int width = x_max - x_min;
  int height = y_max - y_min;
  vg_draw_rectangle(x_min, y_min, width, height, 0xffffffff);
  
  return 0;
}

/**
 * @brief draws a pixmap on screen
 * 
 * @param pixmap 
 * @param xi 
 * @param yi 
 * @param wd 
 * @param hg 
 * @return int 
 */

int vg_draw_pixmap(char *pixmap, uint16_t xi, uint16_t yi, int wd, int hg) {

   if (clearScreen() != OK) {
    printf("ERROR: Failed to clear screen");
    return 0;
  }  

  int i = 0, j = 0;

  for (uint16_t k = yi; k < yi + hg; k++, i++) {
    for (uint16_t l = xi; l < xi + wd; l++, j++) {

      memcpy(double_buffer + l * bytespp + (k * hres * bytespp), (pixmap + i * wd + j), bytespp);
      
    }
  }

  return 0;
}

/**
 * @brief does a double buffer
 * 
 */
void doubleBuffering(){
   memcpy(vmem, double_buffer, hres * vres * bytespp);
}

/**
 * @brief draws a pattern on the screen
 * 
 * @param no_rectangles 
 * @param first 
 * @param step 
 */
void vg_draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step) {

  uint16_t rectangle_width = hres / no_rectangles;
  uint16_t rectangle_height = vres / no_rectangles;
  uint32_t color;

  uint16_t r, g, b, firstR, firstG, firstB;

  if (modeAux == 0x105) {

    for (unsigned int col = 0; col < no_rectangles; col++) {
      for (unsigned int row = 0; row < no_rectangles; row++) {

        color = (first + (row * no_rectangles + col) * step) % (1 << bpp);

        vg_draw_rectangle(col * rectangle_width, row * rectangle_height, rectangle_width, rectangle_height, color);
      }
    }
  }

  else if ((modeAux == 0x110) || (modeAux == 0x115) || (modeAux == 0x11A) || (modeAux == 0x14C)) {
    firstR = first >> RedFieldPosition;
    firstG = first >> GreenFieldPosition;
    firstB = first >> BlueFieldPosition;

    for (unsigned int col = 0; col < no_rectangles; col++) {
      for (unsigned int row = 0; row < no_rectangles; row++) {
        r = (firstR + col * step) % (1 << RedScreenMask);
        g = (firstG + row * step) % (1 << GreenScreenMask);
        b = (firstB + (col + row) * step) % (1 << BlueScreenMask);

        color = (b << BlueFieldPosition) + (g << GreenFieldPosition) + (r << RedFieldPosition);

        vg_draw_rectangle(col * rectangle_width, row * rectangle_height, rectangle_width, rectangle_height, color);
      }
    }
  }
  else {
    printf("ERROR: Wrong mode");
  }
}

/**
 * @brief reads the vbe info
 * 
 * @param info_p 
 * @return int 
 */

int read_vbe(vg_vbe_contr_info_t *info_p){
  
  struct reg86u r;
  phys_bytes buf;

  memset(&r, 0, sizeof(struct reg86u)); /* zero the structure */


  lm_init(false);
  mmap_t mem_map;
  lm_alloc(sizeof(vg_vbe_contr_info_t), &mem_map);
  buf = mem_map.phys;

  r.u.w.ax = 0x4F00; 
  r.u.w.es = PB2BASE(buf); 
  r.u.w.di = PB2OFF(buf);  
  r.u.b.intno = 0x10;

   
  if (sys_int86(&r) != OK) {
    lm_free(&mem_map);
    return 1;
  }

  *info_p = *(vg_vbe_contr_info_t *)mem_map.virt;


  lm_free(&mem_map);
  return 0;

}

/**
 * @brief Get the Hor Resolution object
 * 
 * @return int 
 */

int getHorResolution(){
  return hres;
}

/**
 * @brief Get the Ver Resolution object
 * 
 * @return int 
 */

int getVerResolution(){
  return vres;
}

/**
 * @brief Get the Graphics Buffer object
 * 
 * @return uint16_t* 
 */

uint16_t* getGraphicsBuffer(){
  return (uint16_t*)double_buffer;
}
