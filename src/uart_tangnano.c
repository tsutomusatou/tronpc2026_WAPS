#include "common_utils.h"
#include "uart_tangnano.h"

/* Flag for user callback */
uint8_t g_uart_tangnano_event = RESET_VALUE;
bool uart_tangnano_tx_done = false;

fsp_err_t uart_tangnano_init(void)
{
     fsp_err_t err = FSP_SUCCESS;

     /* Initialize UART channel with baud rate 115200 */
     err = R_SCI_B_UART_Open (&g_uart_tangnano_ctrl, &g_uart_tangnano_cfg);
     if (FSP_SUCCESS != err)
     {
         APP_ERR_PRINT ("\r\n** [UART][TGN] R_SCI_UART_Open API failed  **\r\n");
         return err;
     }

     return err;
}

void uart_tangnano_callback(uart_callback_args_t *p_args)
{
    /* Logged the event in global variable */
    g_uart_tangnano_event = (uint8_t)p_args->event;

    if (UART_EVENT_TX_COMPLETE == g_uart_tangnano_event)
    {
        uart_tangnano_tx_done = true;
        SEGGER_RTT_printf(0, "[UART][TGN] TX complete: %d\n", g_uart_tangnano_event);
    }
}
