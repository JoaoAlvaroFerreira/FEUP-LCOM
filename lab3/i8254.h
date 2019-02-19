#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */
int extern counter;

#define TIMER_FREQ  1193182    /**< @brief clock frequency for timer in PC and AT */
#define DELAY_US    20000

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */
#define ERROR 			1   /**When functions return as expected*/


/* I/O port addresses */

#define TIMER_0			0x40 /**< @brief Timer 0 count register */
#define TIMER_1			0x41 /**< @brief Timer 1 count register */
#define TIMER_2			0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL		0x43 /**< @brief Control register */

#define SPEAKER_CTRL		0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0		0x00     /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1		BIT(6)   /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2		BIT(7)   /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD		(BIT(7)|BIT(6))  /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB		BIT(4)  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB		BIT(5)  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB		(TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */ 

/* Operating mode: bits 3, 2 and 1 */

#define MODE_COMPARISON 	(BIT(1) | BIT(2) | BIT(3))
#define TIMER_SQR_WAVE		(BIT(2)|BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN		BIT(2)          /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD			0x01   /**< @brief Count in BCD */
#define TIMER_BIN			0x00   /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_         BIT(5)
#define TIMER_RB_STATUS_        BIT(4)
#define TIMER_RB_SEL(n)         BIT((n)+1)

/* INIT MODE COMPARISON */

#define COMPARE_VALUE 			0xcf
#define LSB_ONLY_MODE 			0xdf
#define MSB_ONLY_MODE 			0xef
#define MSB_AFTER_LSB_MODE 		0xff

/* Counting Mode Definitions */
#define INTERRUPT_ON_TERMINAL_COUNT 	0x00
#define HARDWARE_RETRIGGERABLE_ONE_SHOT 0x01
#define RATE_GENERATOR					0x02
#define SQUARE_WAVE_MODE				0x03
#define SOFTWARE_TRIGGERED_STROBE		0x04
#define HARDWARE_TRIGGERED_STROBE		0x05
#define UNEXISTENT_MODE_1				0x06
#define UNEXISTENT_MODE_2				0x07

/* Setting Frequency Limits */
#define MINIMUM_FREQUENCY 				18
#define MAXIMUM_FREQUENCY 				65537

/**@}*/

#endif /* _LCOM_I8254_H */
