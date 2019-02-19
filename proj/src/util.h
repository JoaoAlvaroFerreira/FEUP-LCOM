// game states
#include "bitmap.h"
#include "objetos.h"

#define BEGINNING 0
#define INITIAL_MENU 1
#define DIFFICULTY_MENU 2
#define HIGHSCORE_TABLE 3
#define IN_GAME 4
#define ENTER_NAME 5

#define Y_CLIENTE 400
#define Y_BAG 780

#define SPACE_BAR_BREAK 0xb9
#define W_KEY               0x11
#define D_KEY               0x20
#define A_KEY               0x1E
#define S_KEY               0x1F

// creations
Objeto *create_objeto(Bitmap *bmp, int x, int y, Alignment alignment,bool pickable);
Objeto_texto *create_objeto_texto(char *string, int x, int y,Alignment alignment, bool pickable);
Comprimido *create_pill(Bitmap *bmp, int x, int y, Alignment alignment,char cure1[10], char cure2[10]);
Cliente *create_cliente(int x, int y, Alignment alignment);
//bag_of_pills *create_bag_of_pills(Bitmap *bmp, int x, int y, Alignment alignment, bool pickable);

//draws
void drawBitmaps(Objeto **object, unsigned int numero_objetos);
void drawClientes(Cliente **clientes, unsigned int numero_clientes);
void drawComprimidos(Comprimido **comprimidos, unsigned int numero_comprimidos);
void draw_text_objects(Objeto_texto **objetos_texto, unsigned int numero_objetos_texto);
void draw_text_singular(Objeto_texto *objeto);
//void draw_bags(bag_of_pills **objetos, unsigned int numero_objetos);
void drawAll(Objeto **objetos, unsigned int numero_objetos, Objeto_texto **objetos_texto, unsigned int numero_objetos_texto);
void drawAllInGame(Objeto **in_game, unsigned int numero_in_game,
                   Cliente **clientes, unsigned int numero_clientes,
                   Comprimido **comprimidos, unsigned int numero_comprimidos,
                Objeto_texto **objetos_texto, unsigned int numero_objetos_texto);

//moves
void move_rato();
void move_objeto_geral(Objeto *object);
void move_objeto_coordenadas(Objeto *obj, int x, int y);
int move_cliente_coordenadas(Cliente *obj, int x, int y);
int move_comprimido(Comprimido *comprimido);
int move_clientes_in(Cliente **clientes, unsigned int numero_clientes);
int move_clientes_out(Cliente **clientes, unsigned int numero_clientes);
//int move_bag(bag_of_pills *object);

//verifications and game_utilities
bool check_if_all_cured(Cliente *cliente);
bool pressed(Objeto *object);
void dragPillProcess(Comprimido **comprimidos, int comprimidos_size,
                     Cliente **clientes, unsigned int numero_clientes);
void givePill(Comprimido *comprimido, Cliente *cliente);
void countdown(Cliente **clientes, unsigned int numero_clientes);

//"programing" utilities
Objeto **delete_objeto_from_array(Objeto *obj, Objeto **objetos,unsigned int *numero_objetos);
Objeto_texto **delete_objeto_texto_from_array(Objeto_texto *obj, Objeto_texto **objetos,unsigned int *numero_objetos_texto);
//bag_of_pills **delete_bag_from_array(bag_of_pills *obj, bag_of_pills **objetos, unsigned int *numero_objetos_texto);
Cliente **delete_cliente_from_array(Cliente *obj, Cliente **objetos, unsigned int *numero_objetos);
void deleteObjeto(Objeto *obj);
void deleteObjeto_texto(Objeto_texto *obj);
// if indice == -1 put obj in last position of the array objetos
// indice is the indice where to add in the array
Objeto **add_objeto_to_array(Objeto *obj, int indice, Objeto **objetos, unsigned int *numero_objetos);
Objeto_texto **add_objeto_texto_to_array(Objeto_texto *obj, int indice, Objeto_texto **objetos, unsigned int *numero_objetos_texto);
Comprimido **add_comprimido_array(Comprimido *obj, int indice, Comprimido **objetos, unsigned int *numero_objetos);
//bag_of_pills **add_bag_to_array(bag_of_pills *obj, int indice, bag_of_pills **objetos, unsigned int *numero_objetos);

Objeto_texto *updateText(Objeto_texto *obj, char *newText);
Comprimido ** resetPillPositions(Comprimido ** comprimidos);
void alterName();
