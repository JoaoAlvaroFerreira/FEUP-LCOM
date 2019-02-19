#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"
#include <minix/sysutil.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

static int hook_id;
uint32_t mouseByte, minixState;
bool failure;
unsigned int packetLength = MOUSE_READING_CYCLES;
bool Y_sign, X_sign;
struct packet pp;

int mouse_subscribe_int(int bit_no)
{

  hook_id = bit_no;
  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0)
    return 1;

  return 0;
}

int mouse_unsubscribe_int()
{
  if (sys_irqdisable(&hook_id) != OK)
  {

    printf("ERROR: sys_irqdisable of kbd_unsubscribe_int failed.\n");
    return 1;
  }
  if (sys_irqrmpolicy(&hook_id) != OK)
    return 1;

  return 0;
}

int remoteEnable()
{
  uint32_t stat, data;

  for (unsigned int i = 0; i < ATTEMPTS; i++)
  {

    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
    sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE);

    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
      sys_outb(OUT_BUF, MOUSE_SET_REMOTE_MODE);

      tickdelay(micros_to_ticks(DELAY_US));
      sys_inb(OUT_BUF, &data); //Aknowledgement Byte

      if (data == ACK)
      {

        return 0;
      }

      tickdelay(micros_to_ticks(DELAY_US));
    }
    }
  }

  return 1;
}

int dataReportingEnable()
{
  uint32_t stat, data;

  for (unsigned int i = 0; i < ATTEMPTS; i++)
  {
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
    sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE);

    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
      sys_outb(OUT_BUF, MOUSE_ENABLE);

      tickdelay(micros_to_ticks(DELAY_US));
      sys_inb(OUT_BUF, &data); //Aknowledgement Byte

      if (data == ACK)
      {

        return 0;
      }

      tickdelay(micros_to_ticks(DELAY_US));
    }
    }
  }

  return 1;
}

int restoreStateDefault()
{

  minixState = minix_get_dflt_kbc_cmd_byte();
  uint32_t stat;

  for (unsigned int i = 0; i < ATTEMPTS; i++)
  {
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
    sys_outb(KBC_CMD_REG, WRITE_CMD_BYTE);

    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
      sys_outb(OUT_BUF, minixState);
      return 0;
    }
    }
  }
  return 1;
}

int dataReportingDisable()
{
  uint32_t stat, data;

  for (unsigned int i = 0; i < ATTEMPTS; i++)
  {
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
    sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE);

    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
      sys_outb(OUT_BUF, MOUSE_DISABLE);

      tickdelay(micros_to_ticks(DELAY_US));
      sys_inb(OUT_BUF, &data); //Aknowledgement Byte

      if (data == ACK)
      {

        return 0;
      }
      tickdelay(micros_to_ticks(DELAY_US));
    }
    }
  }

  return 1;
}

int remoteReadData()
{

  uint32_t stat, data;

  for (unsigned int i = 0; i < ATTEMPTS; i++)
  {
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
    sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE);

    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
      sys_outb(OUT_BUF, MOUSE_READ_DATA);

      tickdelay(micros_to_ticks(DELAY_US));
      sys_inb(OUT_BUF, &data); //Aknowledgement Byte

      if (data == ACK)
      {

        return 0;
      }
    }}
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

int enableStreamMode()
{

  uint32_t stat, data;

  for (unsigned int i = 0; i < ATTEMPTS; i++)
  {
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
    sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE);

    tickdelay(micros_to_ticks(DELAY_US));
    sys_inb(STAT_REG, &stat);

    if (!(stat & IBF))
    {
      sys_outb(OUT_BUF, MOUSE_SET_STREAM_MODE);

      tickdelay(micros_to_ticks(DELAY_US));
      sys_inb(OUT_BUF, &data); //Aknowledgement Byte

      if (data == ACK)
      {

        return 0;
      }
    }
    }

  
    tickdelay(micros_to_ticks(DELAY_US));
  }

  return 1;
}

int mouseByteReader()
{

  if (packetLength == MOUSE_READING_CYCLES)
  { //FIRST BYTE

    if (mouseByte & BIT(3))
    {

      pp.bytes[0] = mouseByte;

      pp.lb = mouseByte & BIT(0);
      pp.rb = mouseByte & BIT(1);
      pp.mb = mouseByte & BIT(2);
      X_sign = mouseByte & BIT(4);
      Y_sign = mouseByte & BIT(5);
      pp.x_ov = mouseByte & BIT(6);
      pp.y_ov = mouseByte & BIT(7);
    }
    else
    {

      packetLength = MOUSE_READING_CYCLES;
      return 1;
    }
  }
  else if (packetLength == 2)
  { //SECOND BYTE

    pp.bytes[1] = mouseByte;

    if (X_sign)
      pp.delta_x = 0xff00 | (int16_t)mouseByte;
    else
      pp.delta_x = (int16_t)mouseByte;
  }
  else if (packetLength == 1)
  { //THIRD BYTE

    pp.bytes[2] = mouseByte;

    if (Y_sign)
      pp.delta_y = 0xff00 | (int16_t)mouseByte;
    else
      pp.delta_y = (int16_t)mouseByte;
  }
  else
  {

    printf("ERROR: Invalid Byte Length");
    return 1;
  }

  packetLength--;
  if (packetLength == 0)
  {

    mouse_print_packet(&pp);
    packetLength = MOUSE_READING_CYCLES;
    return 0;
  }
  return 1;
}

int mouseByteReaderRemote()
{

  if (remoteReadData() == ERROR)
  {
    printf("ERROR: Issues passing the command to read data");
    return 1;
  }

  for (unsigned int i = MOUSE_READING_CYCLES; i > 0; i--)
  {
    sys_inb(OUT_BUF, &mouseByte);

    if (i == MOUSE_READING_CYCLES)
    { //FIRST BYTE

      if (mouseByte & BIT(3))
      {

        pp.bytes[0] = mouseByte;

        pp.lb = mouseByte & BIT(0);
        pp.rb = mouseByte & BIT(1);
        pp.mb = mouseByte & BIT(2);
        X_sign = mouseByte & BIT(4);
        Y_sign = mouseByte & BIT(5);
        pp.x_ov = mouseByte & BIT(6);
        pp.y_ov = mouseByte & BIT(7);
      }
      else
      {

        return 1;
      }
    }
    else if (i == 2)
    { //SECOND BYTE

      pp.bytes[1] = mouseByte;

      if (X_sign)
        pp.delta_x = 0xff00 | (int16_t)mouseByte;
      else
        pp.delta_x = (int16_t)mouseByte;
    }
    else if (i == 1)
    { //THIRD BYTE

      pp.bytes[2] = mouseByte;

      if (Y_sign)
        pp.delta_y = 0xff00 | (int16_t)mouseByte;
      else
        pp.delta_y = (int16_t)mouseByte;
    }
    else
    {

      printf("ERROR: Invalid Byte Length");
      return 1;
    }

    if (i == 1)
    {

      mouse_print_packet(&pp);
      i = MOUSE_READING_CYCLES;
      return 0;
    }
  }
  return 1;
}

void(mouse_ih)(void)
{
  uint32_t stat;
  sys_inb(STAT_REG, &stat); /* assuming it returns OK */
                            /* loop while 8042 output buffer is empty */
  if (stat & OBF)
  {
    if ((stat & (PAR_ERR | TO_ERR)) == 0)
    {
      sys_inb(OUT_BUF, &mouseByte); /* assuming it returns OK */
      return;
    }
    else
    {

      return;
    }
  }

  return;
}

float slope(int x, int y)
{
  return abs(y/x);
}
