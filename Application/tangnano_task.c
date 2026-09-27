#include "tangnano_task.h"
#include "kws_task.h"
#include "task_message.h"

ID    tangnano_tskid;
T_CTSK tangnano_ctsk = {             // Task creation information
    .itskpri    = 12,
    .stksz      = 1024,
    .task       = tangnano_task,
    .tskatr     = TA_HLNG | TA_RNG3,
};

fsp_err_t  tangnano_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Nothing to do */

     return err;
}

fsp_err_t  tangnano_send_msg(int mode)
{
    fsp_err_t err = FSP_SUCCESS;
    char msg[16];

//    sprintf(msg, "%d\n", mode);
    sprintf(msg, "%d", mode);

    g_uart_tangnano_event = RESET_VALUE;

    APP_PRINT("[TGN] Sending msg to Tang Nano: %s\n", msg);

    uart_tangnano_tx_done = false;
    err = R_SCI_B_UART_Write(&g_uart_tangnano_ctrl, (uint8_t *)msg, strlen(msg));
    while (!uart_tangnano_tx_done)
    {
        APP_PRINT("\r\n[TGN] Wait for completion of UART TX...\r\n");
        tk_dly_tsk(1);  // 軽く待機
    }
    return err;
}

void tangnano_task(INT stacd, void *exinf)
{
    T_TGN_CMD_MSG *msg = NULL;
    ER err;
    int mode = 1;

    APP_PRINT("[TGN] Task started.\n");

    tangnano_init();

    while(1) {
        APP_PRINT("\r\n[TGN] Sleeping...\r\n");
        tk_slp_tsk(TMO_FEVR);
        APP_PRINT("[TGN] Waked up.\n");

        // Receive message
        err = tk_rcv_mbx(MBX_KWS_TO_TGN, (T_MSG**)&msg, TMO_FEVR);

        APP_PRINT("[TGN] Received msg->mode: %d\n", msg->mode);
        mode = msg->mode;

        tangnano_send_msg(mode);

        APP_PRINT("[TGN] Sending to Tang Nano finished.\n");

//        APP_PRINT("[TGN] Sending to Tang Nano finished. Wakup kws task.\n");

        tk_rel_mpl(MPL_KWS_TO_TGN, (void*)msg);
//
//        err = tk_wup_tsk(kws_tskid);
//        if (err < E_OK)
//        {
//            APP_ERR_PRINT("[TGN] tk_wup_tsk failed: %d\n", err);
//        }
    }
}
