#ifndef UART_RASPI_H_
#define UART_RASPI_H_

#include "common_utils.h"

/* Flag for user callback */
extern uint8_t g_uart_raspi_event;
extern bool uart_raspi_tx_done;
extern bool uart_raspi_rx_done;

/* Function declaration */
fsp_err_t uart_raspi_init(void);

#endif /* UART_DFPLAYER_H_ */
