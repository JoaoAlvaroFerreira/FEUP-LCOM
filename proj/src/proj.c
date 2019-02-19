// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
// Any header files included below this line should have been created by you
#include "graphics.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboard_standalone.h"
#include "mouse.h"
#include "timer3.h"
#include "util.h"
#include "highscores.h"


uint32_t scanCode;
uint16_t modeGlobal;
int extern Counter;
extern struct packet pp;
Rato rato;
extern uint16_t hres; /* XResolution */
extern uint16_t vres; /* YResolution */
// 7b4d7b cor transparente
unsigned int number_deseases = 3;
char **deseases;
Bitmap *ghost;
unsigned int points = 0;
unsigned int tempo = 0;
char *name = "AAA";

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");
  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");
  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}

/**
 * @brief The main loop of the proj, where every interrupt is handled and where every game stat is processed.
 * 
 */
int(proj_main_loop)(int argc, char *argv[]) {
  printf("argc %x", argc);
  printf("argv %c", argv);
  vg_init_11a();
  // Create mouse
  Bitmap *Rato = loadBitmap("/home/lcom/labs/proj/src/images/cursorBasic.bmp");
  rato.bitmap = Rato;
  rato.x = 250;
  rato.y = 250;
  bool notWin = true;
  bool showBoard = false;
  // create objects
  Bitmap *ghost = loadBitmap("/home/lcom/labs/proj/src/images/ghost.bmp");
  deseases = (char **)malloc(sizeof(char) * number_deseases);
  for (unsigned int k = 0; k < number_deseases; k++)
    deseases[k] = (char *)malloc(sizeof(char) * 10);
  strcpy(deseases[0], "LUNGS");
  strcpy(deseases[1], "STOMACH");
  strcpy(deseases[2], "SKIN");

  // pills
  Bitmap *pill1_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill1.bmp");
  Comprimido *pill1 =
      create_pill(pill1_image, 330, 840, ALIGN_LEFT, "LUNGS", "LUNGS");
  Bitmap *pill2_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill2.bmp");
  Comprimido *pill2 =
      create_pill(pill2_image, 400, 840, ALIGN_LEFT, "LUNGS", "STOMACH");
  Bitmap *pill3_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill3.bmp");
  Comprimido *pill3 =
      create_pill(pill3_image, 470, 840, ALIGN_LEFT, "LUNGS", "SKIN");
  Bitmap *pill4_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill4.bmp");
  Comprimido *pill4 =
      create_pill(pill4_image, 540, 840, ALIGN_LEFT, "STOMACH", "LUNGS");
  Bitmap *pill5_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill5.bmp");
  Comprimido *pill5 =
      create_pill(pill5_image, 610, 840, ALIGN_LEFT, "STOMACH", "STOMACH");
  Bitmap *pill6_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill6.bmp");
  Comprimido *pill6 =
      create_pill(pill6_image, 680, 840, ALIGN_LEFT, "STOMACH", "SKIN");
  Bitmap *pill7_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill7.bmp");
  Comprimido *pill7 =
      create_pill(pill7_image, 750, 840, ALIGN_LEFT, "SKIN", "LUNGS");
  Bitmap *pill8_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill8.bmp");
  Comprimido *pill8 =
      create_pill(pill8_image, 820, 840, ALIGN_LEFT, "SKIN", "STOMACH");
  Bitmap *pill9_image =
      loadBitmap("/home/lcom/labs/proj/src/images/pills/pill9.bmp");
  Comprimido *pill9 =
      create_pill(pill9_image, 890, 840, ALIGN_LEFT, "SKIN", "SKIN");
  // clientes
  Cliente *velha1 = create_cliente( -550, Y_CLIENTE, ALIGN_LEFT);
  Cliente *velha2 = create_cliente( -300, Y_CLIENTE, ALIGN_LEFT);
  Cliente *velha3 = create_cliente( -800, Y_CLIENTE, ALIGN_LEFT);
  /* Bitmap *pills[] = {pill1, pill2, pill3};
  unsigned int pillCount = 0; */
  Objeto *pill = create_objeto(ghost, 100, 100, ALIGN_LEFT, false);
  Objeto_texto *get_in =
      create_objeto_texto("GET IN!!!", 500, 435, ALIGN_LEFT, false);
  Bitmap *outside =
      loadBitmap("/home/lcom/labs/proj/src/images/Pharmacy_outside.bmp");
  Bitmap *initial_menu_image =
      loadBitmap("/home/lcom/labs/proj/src/images/menu/title.bmp");
  Bitmap *top_score =
      loadBitmap("/home/lcom/labs/proj/src/images/menu/topscores.bmp");
  Objeto *background = create_objeto(outside, 0, 0, ALIGN_LEFT, false);
  Bitmap *Saida = loadBitmap("/home/lcom/labs/proj/src/images/exit.bmp");
  Objeto *saida = create_objeto(Saida, 0, 0, ALIGN_LEFT, true);
  Bitmap *Door = loadBitmap("/home/lcom/labs/proj/src/images/door.bmp");
  Objeto *door = create_objeto(Door, 223, 570, ALIGN_LEFT, true);
  Bitmap *Blue_arrow = loadBitmap("/home/lcom/labs/proj/src/images/arrow.bmp");
  Bitmap *Red_arrow =
      loadBitmap("/home/lcom/labs/proj/src/images/red_arrow.bmp");
  Objeto *arrow = create_objeto(Blue_arrow, 400, 435, ALIGN_LEFT, false);
  Bitmap *Play =
      loadBitmap("/home/lcom/labs/proj/src/images/menu/startgame.bmp");
  Objeto *play_button = create_objeto(ghost, 480, 300, ALIGN_LEFT, false);
  Bitmap *Highscore =
      loadBitmap("/home/lcom/labs/proj/src/images/menu/topscorebutton.bmp");
  Objeto *highscore_button = create_objeto(ghost, 480, 600, ALIGN_LEFT, false);
  Bitmap *basepharmacy =
      loadBitmap("/home/lcom/labs/proj/src/images/pharmacy/basepharmacy.bmp");
  Objeto *pharmacy_bg = create_objeto(basepharmacy, 0, 0, ALIGN_LEFT, false);
  Bitmap *table =
      loadBitmap("/home/lcom/labs/proj/src/images/pharmacy/table.bmp");
  Objeto *pharmacy_table = create_objeto(table, 0, 0, ALIGN_LEFT, false);
  Bitmap *pillbox =
      loadBitmap("/home/lcom/labs/proj/src/images/pharmacy/desk.bmp");
  Objeto *pill_box = create_objeto(pillbox, 0, 0, ALIGN_LEFT, false);
  Bitmap *scoresignbmp =
      loadBitmap("/home/lcom/labs/proj/src/images/pharmacy/scoresign.bmp");
  Objeto *scoresign = create_objeto(scoresignbmp, 0, 0, ALIGN_LEFT, false);

  Bitmap *pillBoard =
      loadBitmap("/home/lcom/labs/proj/src/images/pharmacy/sign.bmp");

  unsigned int numero_highscores;
  highscore **high;
  high = readFromFile(&numero_highscores);

  // vector with objects to draw
  unsigned int numero_objetos = 7;
  Objeto **objetos;
  objetos = (Objeto **)malloc(sizeof(Objeto) * numero_objetos);
  objetos[0] = background;
  objetos[1] = pill;
  objetos[2] = saida;
  objetos[3] = door;
  objetos[4] = arrow;
  objetos[5] = play_button;
  objetos[6] = highscore_button;
  Objeto **in_game;
  unsigned int numero_in_game = 4;
  in_game = (Objeto **)malloc(sizeof(Objeto) * numero_in_game);
  in_game[0] = pharmacy_bg;
  in_game[1] = pharmacy_table;
  in_game[2] = pill_box;
  in_game[3] = scoresign;

  // vector with text_objects to draw
  unsigned int numero_objetos_texto = 1;
  Objeto_texto **objetos_texto;
  objetos_texto =
      (Objeto_texto **)malloc(sizeof(Objeto_texto) * numero_objetos_texto);
  objetos_texto[0] = get_in;

  unsigned int numero_clientes = 3;
  Cliente **clientes;
  clientes = (Cliente **)malloc(sizeof(Cliente) * numero_clientes);
  clientes[0] = velha1;
  clientes[1] = velha2;
  clientes[2] = velha3;
  unsigned int numero_comprimidos = 9;
  Comprimido **comprimidos;
  comprimidos = (Comprimido **)malloc(sizeof(Comprimido) * numero_comprimidos);
  comprimidos[0] = pill1;
  comprimidos[1] = pill2;
  comprimidos[2] = pill3;
  comprimidos[3] = pill4;
  comprimidos[4] = pill5;
  comprimidos[5] = pill6;
  comprimidos[6] = pill7;
  comprimidos[7] = pill8;
  comprimidos[8] = pill9;

  int r;
  int ipc_status;
  message msg;
  uint8_t timer_bit = 0;
  int keyboard_bit = 1;
  int mouse_bit = 12;
  int state = 0;
  unsigned int arrow_color = 1;
  char Points[10];
  char Get_name[40];

  if (dataReportingEnable() == ERROR) {
    printf("ERROR: Enabling data reporting failed.\n");
    return 1;
  }
  if (k_subscribe_int(keyboard_bit) == ERROR) {
    printf("ERROR: Subscribe failed");
  }
  if (mouse_subscribe_int(mouse_bit) == ERROR) {
    printf("ERROR: Subscribe failed");
  }
  if (timer_subscribe_int3(timer_bit) == ERROR) {
    printf("ERROR: subscribe timer failed");
    return 1;
  }
  while (notWin && scanCode != ENTER_KEY_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: // hardware interrupt notification
        // Keyboard
        // interrupt----------------------------------------------------------------------------------
        if (msg.m_notify.interrupts &
            BIT(keyboard_bit)) { // subscribed interrupt
          scanCodeReader();

          switch (state) {

          case INITIAL_MENU:
            if (scanCode == ESC_KEY_BREAK)
              notWin = false;
            break;
          case IN_GAME:

            if (scanCode == SPACE_BAR_BREAK)
              comprimidos = resetPillPositions(comprimidos);

            if (scanCode == A_KEY) {
              showBoard = !showBoard;
            }
            break;
          case ENTER_NAME:
            alterName();

            if (scanCode == SPACE_BAR_BREAK) {
              char pontos[10];
              sprintf(pontos, "%d", points);
              high = makeHighScore(name, pontos, high, &numero_highscores);
              writeToFile(high, numero_highscores);
              high = readFromFile(&numero_highscores);
              high = orderHighscore(high, numero_highscores);
              writeToFile(high, numero_highscores);
              high = readFromFile(&numero_highscores);

              scanCode = ESC_KEY_BREAK;
            }
            break;
          default:
            break; // no other notifications expected: do nothing
          }

          if (scanCode == ESC_KEY_BREAK) {
            background->bitmap = initial_menu_image;
            play_button->bitmap = Play;
            highscore_button->bitmap = Highscore;
            saida->pickable = false;
            door->pickable = false;
            play_button->pickable = true;
            highscore_button->pickable = true;
            points = 0;
            Cliente *velha1 = create_cliente(-550, Y_CLIENTE, ALIGN_LEFT);
            Cliente *velha2 = create_cliente(-300, Y_CLIENTE, ALIGN_LEFT);
            Cliente *velha3 = create_cliente(-800, Y_CLIENTE, ALIGN_LEFT);
            clientes[0] = velha1;
            clientes[1] = velha2;
            clientes[2] = velha3;
            state = INITIAL_MENU;
          }

        }
        //-------------------------------------------------------------------------------------------------------
        // mouse
        // interrupt--------------------------------------------------------------------------------------
        else if (msg.m_notify.interrupts &
                 BIT(mouse_bit)) { // subscribed interrupt
          mouse_ih();
          if (mouseByteReader() == OK) {
            switch (state) {
            case BEGINNING:
              if (pressed(door)) {
                background->bitmap = initial_menu_image;
                objetos =
                    delete_objeto_from_array(saida, objetos, &numero_objetos);
                objetos =
                    delete_objeto_from_array(arrow, objetos, &numero_objetos);
                saida->pickable = false;
                door->pickable = false;
                play_button->pickable = true;
                highscore_button->pickable = true;
                objetos_texto = delete_objeto_texto_from_array(
                    get_in, objetos_texto, &numero_objetos_texto);

                state = INITIAL_MENU;
              }

              if (pressed(saida)) {
                notWin = false;
              }
              break;
            case INITIAL_MENU:
              play_button->bitmap = Play;
              highscore_button->bitmap = Highscore;
              if (pressed(play_button)) {
                state = IN_GAME;
                counter = 0;
                tempo = 0;

                background->bitmap = basepharmacy;
              }
              if (pressed(highscore_button)) {
                background->bitmap = top_score;
                play_button->bitmap = ghost;
                highscore_button->bitmap = ghost;
                state = HIGHSCORE_TABLE;
              }
              break;
            case IN_GAME:
              dragPillProcess(comprimidos, numero_comprimidos, clientes,
                              numero_clientes);
              break;
            default:
              break; // no other notifications expected: do nothing
            }
            move_rato(); // this function must be written always after moving
                         // all other objects
          }
        }
        //-------------------------------------------------------------------------------------------------------
        // timer
        // interrupt---------------------------------------------------------------------------------------
        else if (msg.m_notify.interrupts & BIT(timer_bit)) {
          timer_int_handler();
          switch (state) {
          case BEGINNING:
            if (counter % 60 == 0) {
              if (arrow_color == 1) {
                arrow->bitmap = Blue_arrow;
                arrow_color = 2;
              } else if (arrow_color == 2) {
                arrow->bitmap = Red_arrow;
                arrow_color = 1;
              }
            }
            break;

          case IN_GAME:
            move_clientes_in(clientes, numero_clientes);
            if (counter % 60 == 0) {
              tempo++;
              countdown(clientes, numero_clientes);
            }
            move_clientes_out(clientes, numero_clientes);
            if (tempo == 30) {
              play_button->bitmap = ghost;
              highscore_button->bitmap = ghost;
              state = ENTER_NAME;
            }
            break;

          case HIGHSCORE_TABLE:
            drawAll(objetos, numero_objetos, objetos_texto,
                    numero_objetos_texto);
            drawHighscores(high, numero_highscores);
            doubleBuffering();

            break;

          case ENTER_NAME:
            sprintf(Points, "You got %d points!", points);

            break;
          default:
            break; // no other notifications expected: do nothing
          }

          if (state == IN_GAME) {
            drawAllInGame(in_game, numero_in_game, clientes, numero_clientes,
                          comprimidos, numero_comprimidos, objetos_texto,
                          numero_objetos_texto);

            // draw_text_objects(objetos_texto, numero_objetos_texto);

            if (showBoard)
              drawBitmap(pillBoard, 0, 0, ALIGN_LEFT);

            doubleBuffering();
          } else if (state != HIGHSCORE_TABLE) {
            drawAll(objetos, numero_objetos, objetos_texto,
                    numero_objetos_texto);
            if (state == ENTER_NAME) {
              Objeto_texto *score = score =
                  create_objeto_texto(Points, 410, 200, ALIGN_LEFT, false);
              draw_text_singular(score);

              sprintf(Get_name, "INSERT NAME %s THEN PRESS SPACE BAR", name);

              Objeto_texto *player = score =
                  create_objeto_texto(Get_name, 150, 600, ALIGN_LEFT, false);
              draw_text_singular(player);
            }

            doubleBuffering();
          }
        }
        //--------------------------------------------------------------------------------------------------------
        break;
      default:
        break; // no other notifications expected: do nothing
      }
    } else { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  if (mouse_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }
  if (k_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }
  if (timer_unsubscribe_int() == ERROR) {
    printf("ERROR: Unsubscribe timer failed");
    return 1;
  }
  if (dataReportingDisable() == ERROR) {
    printf("ERROR: Disabling data reporting failed.\n");
    return 1;
  }

  // free bitmaps
  deleteBitmap(Rato);
  // deleteBitmap(pill1);
  // deleteBitmap(pill2);
  // deleteBitmap(pill3);
  deleteBitmap(initial_menu_image);
  deleteBitmap(outside);
  vg_exit();
  return 1;
}
