#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_





//uint32_t extern cnt;
/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Timer. Needs to be completed.
 */

#define KB_IRQ 		1
#define MOUSE_IRQ   12
#define DELAY_US    20000
#define BIT(n) (0x01<<(n))

//CHECKS
#define ERROR		1
#define BREAKCODE_CHECK 0x80
#define DOUBLE_BYTE_SCAN1 0xe0
#define DOUBLE_BYTE_SCAN2 0xe1
#define MOUSE_READING_CYCLES 3
#define ATTEMPTS 5
#define MILISECONDS_IN_A_SECOND 1000

//STATE MACHINE
#define INIT    0
#define FIRST_LINE 1
#define VERTICE 2
#define SECOND_LINE 3
#define SLOPE_CHECK 1


//IMPORTANT REGISTERS
#define STAT_REG 			0x64
#define KBC_CMD_REG			0x64
#define IN_BUF				0x60
#define OUT_BUF				0x60
#define OBF 				BIT(0)
#define IBF 				BIT(1)
#define AUX         BIT(5)
#define TO_ERR 				BIT(6)
#define PAR_ERR 			BIT(7)


//IMPORTANT KEYS
#define ESC_KEY_MAKE		0x01
#define ESC_KEY_BREAK		0x81
#define ENTER_KEY_MAKE      0x1c
#define ENTER_KEY_BREAK      0x9c
#define W_KEY               0x11
#define D_KEY               0x20
#define A_KEY               0x1E
#define UP_ARROW            

//KEYBOARD COMMANDS
#define READ_CMD_BYTE			0x20
#define WRITE_CMD_BYTE			0x60
#define CHECK_KBC_SELF			0xAA  //Em principio não usamos
#define CHECK_KBC_INTERFACE 	0xAB
#define DISABLE_KBD_INTERFACE	0xAD
#define ENABLE_KBD_INTERFACE	0xAE

//MOUSE WRITE
#define WRITE_BYTE_MOUSE       0xD4
#define ACK 0xFA
#define NACK 0xFE
#define M_ERROR 0xFC
//MOUSE COMMANDS
#define MOUSE_RESET   0xFE
#define MOUSE_RESEND   0xFE
#define MOUSE_SET_DEFAULT 0xF6
#define MOUSE_DISABLE 0xF5
#define MOUSE_ENABLE 0xF4
#define MOUSE_SET_SAMPLE_RATE 0xF3
#define MOUSE_SET_REMOTE_MODE 0xF0
#define MOUSE_READ_DATA 0xEB
#define MOUSE_SET_STREAM_MODE 0xEA
#define MOUSE_STATUS_REQUEST 0xE9
#define MOUSE_SET_RESOLUTION 0xE8
#define MOUSE_SET_SCALING_ACC_MODE 0xE7
#define MOUSE_SET_SCALING_LIN_MODE 0xE6

//VBE stuFF
#define GRAPHIC_SECONDS_DELAY 1000000
#define TRANSPARENCY 0xF81F

#endif /* _LCOM_I8254_H */
