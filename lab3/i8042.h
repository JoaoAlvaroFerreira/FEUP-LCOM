#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_





//uint32_t extern cnt;
/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Timer. Needs to be completed.
 */



#define KB_IRQ 		1
#define DELAY_US    20000
#define BIT(n) (0x01<<(n))

//CHECKS
#define ERROR		1
#define BREAKCODE_CHECK 0x80
#define DOUBLE_BYTE_SCAN1 0xe0
#define DOUBLE_BYTE_SCAN2 0xe1


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

//KEYBOARD COMMANDS
#define READ_CMD_BYTE			0x20
#define WRITE_CMD_BYTE			0x60
#define CHECK_KBC_SELF			0xAA  //Em principio não usamos
#define CHECK_KBC_INTERFACE 	0xAB
#define DISABLE_KBD_INTERFACE	0xAD
#define ENABLE_KBD_INTERFACE	0xAE


#endif /* _LCOM_I8254_H */
