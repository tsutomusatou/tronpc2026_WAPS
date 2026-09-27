/***********************************************************************************************************************
 * File Name    : uart_ep.h
 * Description  : Contains function declaration of uart_ep.c and Macros.
 **********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#ifndef UART_DFPLAYER_H_
#define UART_DFPLAYER_H_

#include "common_utils.h"

/* Flag for user callback */
extern uint8_t g_uart_event;
extern bool uart_tx_done;

/* Function declaration */
fsp_err_t uart_dfplayer_init(void);

#endif /* UART_DFPLAYER_H_ */
