#include "bitmap.h"
#include "stdbool.h"

typedef struct {
    Bitmap* bitmap;
    int x;
    int y;
} Rato;

typedef struct {
    Bitmap* bitmap;
    int x;
    int y;
    bool inside;
    bool pickable;
    Alignment alignment;
} Objeto;

typedef struct {
    Bitmap **bitmaps;
    char* string;
    unsigned int number_bitmaps;
    int x;
    int y;
    bool inside;
    bool pickable;
    Alignment alignment;
} Objeto_texto;

typedef struct {
    Bitmap* bitmap;
    int x;
    int y;
    int timeLeft;
    bool in_position;
    int speed;
    bool inside;
    int number_symptoms;
    int pills_recieved;
    Objeto_texto* symptom1;
    Objeto_texto* symptom2;
    Objeto_texto* symptom3;
    Alignment alignment;
} Cliente;

typedef struct {
    Bitmap* bitmap;
    int x;
    int y;
    bool inside;
    bool pickable;
    char cure1[10];
    char cure2[10];
    Alignment alignment;
} Comprimido;

  
typedef struct{
    Objeto_texto* name;
    Objeto_texto* score;
} highscore;


