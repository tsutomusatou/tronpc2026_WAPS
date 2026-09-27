#include "raspi_task.h"
#include "kws_task.h"
#include "task_message.h"

ID    raspi_tskid;
T_CTSK raspi_ctsk = {             // Task creation information
    .itskpri    = 12,
    .stksz      = 1024,
    .task       = raspi_task,
    .tskatr     = TA_HLNG | TA_RNG3,
};

fsp_err_t  raspi_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Nothing to do */

     return err;
}

#if 0
fsp_err_t raspi_recv_msg(char *msg, uint32_t msg_size)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t index = 0;

    if ((NULL == msg) || (msg_size < 2))
    {
        return FSP_ERR_INVALID_ARGUMENT;
    }

    msg[0] = '\0';

    while (index < (msg_size - 1))
    {
        uart_raspi_rx_done  = false;

        /* Receive 1 byte */
        err = R_SCI_B_UART_Read(&g_uart_raspi_ctrl,
                                &uart_raspi_rx_byte,
                                1);

        if (FSP_SUCCESS != err)
        {
            APP_PRINT("[RPI] UART RX start error: %d\r\n", err);
            return err;
        }

        /* Wait for completion of RX */
        while (!uart_raspi_rx_done && !uart_raspi_rx_error)
        {
            tk_dly_tsk(1);
        }

        /* Store received 1 byte */
        msg[index] = (char) uart_raspi_rx_byte;

        /* Complete to receive if it's '\n' */
        if (uart_raspi_rx_byte == '\n')
        {
            msg[index] = '\0';
            break;
        }

        index++;
    }

    APP_PRINT("[RPI] Received msg from Raspi: %s\r\n", msg);

    return FSP_SUCCESS;
}
#endif

fsp_err_t  raspi_send_msg(int mode)
{
    fsp_err_t err = FSP_SUCCESS;
    char msg[16];

    sprintf(msg, "%d\n", mode);

    g_uart_raspi_event = RESET_VALUE;

    APP_PRINT("[RPI] Sending msg to Raspi: %s\n", msg);

    uart_raspi_tx_done = false;
    err = R_SCI_B_UART_Write(&g_uart_raspi_ctrl, (uint8_t *)msg, strlen(msg));
    while (!uart_raspi_tx_done)
    {
        APP_PRINT("\r\n[RPI] Wait for completion of UART TX...\r\n");
        tk_dly_tsk(1);  // 軽く待機
    }
    return err;
}

void raspi_task(INT stacd, void *exinf)
{
    T_RPI_CMD_MSG *msg = NULL;
    ER err;
    int mode = 1;

    APP_PRINT("[RPI] Task started.\n");

    raspi_init();

    while(1) {
        APP_PRINT("\r\n[RPI] Sleeping...\r\n");
        tk_slp_tsk(TMO_FEVR);
        APP_PRINT("[RPI] Waked up.\n");

        // Receive message
        err = tk_rcv_mbx(MBX_KWS_TO_RPI, (T_MSG**)&msg, TMO_FEVR);

        APP_PRINT("[RPI] Received msg->mode: %d\n", msg->mode);
        mode = msg->mode;

        raspi_send_msg(mode);

        APP_PRINT("[RPI] Sending to Raspi finished. Wakup kws task.\n");

        tk_rel_mpl(MPL_KWS_TO_RPI, (void*)msg);

        err = tk_wup_tsk(kws_tskid);
        if (err < E_OK)
        {
            APP_ERR_PRINT("[RPI] tk_wup_tsk failed: %d\n", err);
        }

//        if (mode > 2)
//            mode = 1;
//
//        APP_PRINT("[Raspi] Mode No.%d\n", mode);
//        tk_dly_tsk(1000);
//        mode++;
    }
}
