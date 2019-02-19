// IMPORTANT: you must include the following incline in all your C files
#include <lcom/lcf.h>
// Any header files included below this incline should have been created by you
#include <stdint.h>
#include <stdio.h>
#include "mouse.h"
#include "i8042.h"
#include "timer3.h"

extern int counter;
extern struct packet pp;
extern bool Y_sign, X_sign;
uint32_t minixState;

// Any header files included below this incline should have been created by you

int main(int argc, char *argv[])
{
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command incline arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt)
{
  int ipc_status;
  message msg;
  int bit = 12;
  int r;

  if (dataReportingEnable() == ERROR)
  {
    printf("ERROR: Enabling data reporting failed.\n");
    return 1;
  }

  if (mouse_subscribe_int(bit) == ERROR)
  {
    printf("ERROR: Subscribe failed.\n");
    return 1;
  }

  while (cnt > 0)
  {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    { // received notification

      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: // hardware interrupt notification
        if (msg.m_notify.interrupts & BIT(bit))
        { // subscribed interrupt

          mouse_ih();
          if (mouseByteReader() == OK)
          {
            cnt--;
          }
        }
        break;
      default:
        break; // no other notifications expected: do nothing
      }
    }
    else
    { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  if (mouse_unsubscribe_int() == ERROR)
  {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }

  if (dataReportingDisable() == ERROR)
  {
    printf("ERROR: Disabling data reporting failed.\n");
    return 1;
  }

  if (restoreStateDefault() != OK)
  {
    printf("ERROR: Issue returning Minix to default state. \n");
    return 1;
  }

  return 0;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt)
{

  int trueTime = period * MILISECONDS_IN_A_SECOND;

  while (cnt > 0)
  {

    if (mouseByteReaderRemote() == OK)
    {

      cnt--;
    }
    tickdelay(micros_to_ticks(trueTime));
  }

  if (dataReportingDisable() == ERROR)
  {
    printf("ERROR: Disabling data reporting failed.\n");
    return 1;
  }

  if (enableStreamMode() == ERROR)
  {
    printf("ERROR: Enabling streaming mode failed.\n");
    return 1;
  }

  if (restoreStateDefault() == ERROR)
  {
    printf("ERROR: Issue returning Minix to default state. \n");
    return 1;
  }

  return 0;
}

int(mouse_test_async)(uint8_t idle_time)
{
  int ipc_status;
  message msg;
  int mouse_bit = 4;
  uint8_t timer_bit = 0;
  int r;

  if (dataReportingEnable() == ERROR)
  {
    printf("ERROR: Enabling data reporting failed.\n");
    return 1;
  }

  if (mouse_subscribe_int(mouse_bit) == ERROR)
  {
    printf("ERROR: Subscribe failed");
  }
  if (timer_subscribe_int3(timer_bit) == ERROR)
  {
    printf("ERROR: subscribe timer failed");
    return 1;
  }

  while (counter < (idle_time * 60))
  {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status))
    { // received notification

      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: // hardware interrupt notification
        if (msg.m_notify.interrupts & BIT(mouse_bit))
        { // subscribed interrupt

          mouse_ih();
          if (mouseByteReader() == OK)
          {
            counter = 0;
          }
        }
        else if (msg.m_notify.interrupts & BIT(timer_bit))
        {
          timer_int_handler();
        }
        break;
      default:
        break; // no other notifications expected: do nothing
      }
    }
    else
    { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

  if (mouse_unsubscribe_int() == ERROR)
  {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }
  if (timer_unsubscribe_int() == ERROR)
  {
    printf("ERROR: Unsubscribe timer failed");
    return 1;
  }

  if (dataReportingDisable() == ERROR)
  {
    printf("ERROR: Disable data reporting failed.\n");
    return 1;
  }

  return 0;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance)
{
  int ipc_status;
  message msg;
  int bit = 12;
  int r;
  int state = 0;
  bool STOP = false;
  bool next = false;
  float incline = 0;
  int x_deslocado;
  int count = 0;

  if (dataReportingEnable() == ERROR)
  {
    printf("ERROR: Enabling data reporting failed.\n");
    return 1;
  }

  if (mouse_subscribe_int(bit) == ERROR)
  {
    printf("ERROR: Subscribe failed");
  }

  while (STOP != true)
  {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
    {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status))
    { // received notification

      switch (_ENDPOINT_P(msg.m_source))
      {
      case HARDWARE: // hardware interrupt notification
        if (msg.m_notify.interrupts & BIT(bit))
        { // subscribed interrupt

          mouse_ih();
      
          if (mouseByteReader() == OK)
          {
            count++;

            switch (state)
            {
            case INIT:
              if (pp.lb && !pp.rb && !pp.mb)
              {

                state = FIRST_LINE;
              }

              break;

            case FIRST_LINE:
              if (pp.lb && !pp.rb && !pp.mb)
              {

                if ((Y_sign && (abs(pp.delta_y) > tolerance)) || (X_sign && (abs(pp.delta_x) > tolerance)))
                {
                  state = INIT;
                  next = false;
                  incline = 0;
                  x_deslocado = 0;
                }
                else
                {

                  incline = slope(pp.delta_x, pp.delta_y);
                  x_deslocado = x_deslocado + pp.delta_x;
                  if ((incline >= SLOPE_CHECK) && (x_deslocado >= x_len))
                  {

                    next = true;
                  }
                }
              }
              else if (next && !pp.lb && !pp.rb && !pp.mb)
              {
                state = VERTICE;
                next = false;
                incline = 0;
                x_deslocado = 0;
              }
              else
              {
                state = INIT;
                next = false;
                incline = 0;
                x_deslocado = 0;
              }

              break;

            case VERTICE:
              if (!pp.lb && !pp.rb && !pp.mb && (abs(pp.delta_y) <= tolerance) && (abs(pp.delta_x) <= tolerance))
              {

                incline = 0;
                x_deslocado = 0;
              }
              else if (!pp.lb && pp.rb && !pp.mb)
              {
                state = SECOND_LINE;
              }

              break;

            case SECOND_LINE:
              if (!pp.lb && pp.rb && !pp.mb)
              {
                

                if ((!Y_sign && (abs(pp.delta_y) > tolerance)) || (X_sign && (abs(pp.delta_x) > tolerance)))
                {
                  state = INIT;
                  next = false;
                  incline = 0;
                  x_deslocado = 0;
           
                }
                else
                {
                  incline = slope(pp.delta_x, pp.delta_y);
                  x_deslocado = x_deslocado + pp.delta_x;
           
                  if ((incline >= SLOPE_CHECK) && (x_deslocado >= x_len))
                  {
                    next = true;
                  }
                }
              }
              else if (next && !pp.lb && !pp.rb && !pp.mb)
              {

                STOP = true;
                next = false;
                incline = 0;
                x_deslocado = 0;
          
              }
              else
              {
                state = INIT;
                next = false;
                incline = 0;
                x_deslocado = 0;
              }

              break;
            }
          }
        }
        break;
      default:
        break; // no other notifications expected: do nothing
      }
    }
    else
    { // received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }

 
  if (mouse_unsubscribe_int() == ERROR)
  {
    printf("ERROR: Unsubscribe failed");
    return 1;
  }

  if (dataReportingDisable() == ERROR)
  {
    printf("ERROR: Disabling data reporting failed.\n");
    return 1;
  }

  return 0;
}
