#include "common_utils.h"
#include "uart_raspi.h"

/* Flag for user callback */
uint8_t g_uart_raspi_event = RESET_VALUE;
bool uart_raspi_tx_done = false;
bool uart_raspi_rx_done = false;

fsp_err_t uart_raspi_init(void)
{
     fsp_err_t err = FSP_SUCCESS;

     /* Initialize UART channel with baud rate 115200 */
     err = R_SCI_B_UART_Open (&g_uart_raspi_ctrl, &g_uart_raspi_cfg);
     if (FSP_SUCCESS != err)
     {
         APP_ERR_PRINT ("\r\n** [UART][RPI] R_SCI_UART_Open API failed  **\r\n");
         return err;
     }

     return err;
}

void uart_raspi_callback(uart_callback_args_t *p_args)
{
    /* Logged the event in global variable */
    g_uart_raspi_event = (uint8_t)p_args->event;

    if (UART_EVENT_TX_COMPLETE == g_uart_raspi_event)
    {
        uart_raspi_tx_done = true;
        SEGGER_RTT_printf(0, "[UART][RPI] TX complete: %d\n", g_uart_raspi_event);
    }
#if 0
    else if(UART_EVENT_RX_COMPLETE == g_uart_raspi_event)
    {
        uart_raspi_rx_done = true;
        break;
    }
#endif
}
