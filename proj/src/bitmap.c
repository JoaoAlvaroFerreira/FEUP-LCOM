#include "Bitmap.h"

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "graphics.h"
#include "i8042.h"

/**
 * @brief Loads bitmaps to the project when given their filename (with path)
 * @author Henrique Ferrolho
 * @param filename 
 * @return Bitmap* 
 */

Bitmap *loadBitmap(const char *filename) {
  // allocating necessary size
  Bitmap *bmp = (Bitmap *) malloc(sizeof(Bitmap));

  // open filename in read binary mode
  FILE *filePtr;
  filePtr = fopen(filename, "rb");
  if (filePtr == NULL)
    return NULL;

  // read the bitmap file header
  BitmapFileHeader bitmapFileHeader;
  fread(&bitmapFileHeader, 2, 1, filePtr);

  // verify that this is a bmp file by check bitmap id
  if (bitmapFileHeader.type != 0x4D42) {
    fclose(filePtr);
    return NULL;
  }

  int rd;
  do {
    if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
      break;
    if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
      break;
    if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
      break;
  } while (0);

  if (rd != 1) {
    fprintf(stderr, "Error reading file\n");
    exit(-1);
  }

  // read the bitmap info header
  BitmapInfoHeader bitmapInfoHeader;
  fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

  // move file pointer to the begining of bitmap data
  fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

  // allocate enough memory for the bitmap image data
  unsigned char *bitmapImage = (unsigned char *) malloc(
    bitmapInfoHeader.imageSize);

  // verify memory allocation
  if (!bitmapImage) {

    free(bitmapImage);
    fclose(filePtr);
    return NULL;
  }

  // read in the bitmap image data
  fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

  // make sure bitmap image data was read
  if (bitmapImage == NULL) {
    fclose(filePtr);
    return NULL;
  }

  // close file and return bitmap image data
  fclose(filePtr);

  bmp->bitmapData = bitmapImage;
  bmp->bitmapInfoHeader = bitmapInfoHeader;

  return bmp;
}

/**
 * @brief Draws bitmaps on the screen, given that Minix is in an appropriate graphical mode
 * 
 * @author Henrique Ferrolho
 * @param bmp 
 * @param x 
 * @param y 
 * @param alignment 
 */
void drawBitmap(Bitmap *bmp, int x, int y, Alignment alignment) {
  if (bmp == NULL)
    return;

  int width = bmp->bitmapInfoHeader.width;
  int drawWidth = width;
  int height = bmp->bitmapInfoHeader.height;

  if (alignment == ALIGN_CENTER)
    x -= width / 2;
  else if (alignment == ALIGN_RIGHT)
    x -= width;

  if (x + width < 0 || x > getHorResolution() || y + height < 0 || y > getVerResolution())
    return;

  int xCorrection = 0;
  if (x < 0) {
    xCorrection = -x;
    drawWidth -= xCorrection;
    x = 0;

    if (drawWidth > getHorResolution())
      drawWidth = getHorResolution();
  }
  else if (x + drawWidth >= getHorResolution()) {
    drawWidth = getHorResolution() - x;
  }

  uint16_t *bufferStartPos;
  uint16_t *imgStartPos;

  int i, j;

  for (i = 0; i < height; i++) {
    int pos = y + height - 1 - i;

    if (pos < 0 || pos >= getVerResolution())
      continue;

    bufferStartPos = getGraphicsBuffer();

    bufferStartPos += x + pos * getHorResolution();
    imgStartPos = (uint16_t*)bmp->bitmapData + xCorrection + i * width;

    for (j = 0; j < drawWidth; j++) {
      
      if (imgStartPos[j] != 0xf81f){
        bufferStartPos[j] = imgStartPos[j];
      }

    }
  }
  return;
}

/**
 * @brief Deletes Bitmaps from the project, freeing the memory they had used up until that point
 * 
 * @param bmp 
 */
  void deleteBitmap(Bitmap * bmp) {
    if (bmp == NULL)
      return;

    free(bmp->bitmapData);
    free(bmp);
  }
