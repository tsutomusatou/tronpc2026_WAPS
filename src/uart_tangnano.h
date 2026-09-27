#ifndef UART_TANGNANO_H_
#define UART_TANGNANO_H_

#include "common_utils.h"

/* Flag for user callback */
extern uint8_t g_uart_tangnano_event;
extern bool uart_tangnano_tx_done;

/* Function declaration */
fsp_err_t uart_tangnano_init(void);

#endif /* UART_DFPLAYER_H_ */
