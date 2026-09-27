/***********************************************************************************************************************
 * File Name    : uart_dfplayer.c
 * Description  : Contains UART functions definition.
 **********************************************************************************************************************/
#include "common_utils.h"
#include "uart_dfplayer.h"

/* Flag for user callback */
uint8_t g_uart_event = RESET_VALUE;
bool uart_tx_done = false;

fsp_err_t uart_dfplayer_init(void)
{
     fsp_err_t err = FSP_SUCCESS;

     /* Initialize UART channel with baud rate 115200 */
     err = R_SCI_B_UART_Open (&g_uart_dfplayer_ctrl, &g_uart_dfplayer_cfg);
     if (FSP_SUCCESS != err)
     {
         APP_ERR_PRINT ("\r\n** [UART][DFP] R_SCI_UART_Open API failed  **\r\n");
         return err;
     }

     return err;
}

void uart_dfplayer_callback(uart_callback_args_t *p_args)
{
    /* Logged the event in global variable */
    g_uart_event = (uint8_t)p_args->event;

    if (UART_EVENT_TX_COMPLETE == g_uart_event)
    {
        uart_tx_done = true;
        SEGGER_RTT_printf(0, "[UART][DFP] UART TX complete: %d\n", g_uart_event);
    }
}
