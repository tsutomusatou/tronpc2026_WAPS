#include "dfplayer_task.h"
#include "kws_task.h"
#include "task_message.h"

ID    dfplayer_tskid;
T_CTSK dfplayer_ctsk = {             // Task creation information
    .itskpri    = 8,
    .stksz      = 1024,
    .task       = dfplayer_task,
    .tskatr     = TA_HLNG | TA_RNG3,
};

fsp_err_t  dfplayer_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Wait for DFPlayer is ready (3 sec) */
    tk_dly_tsk(3000);

    g_uart_event = RESET_VALUE;

     /* Control playback mode
      * 1:repeat one song
      * 2:repeat all
      * 3:play one song and pause
      * 4:Play randomly
      * 5:Repeat all in the folder
      * ?:query the current playback mode
      */
     uint8_t playmode_cmd[] = "AT+PLAYMODE=3\r\n";

     uart_tx_done = false;
     /* Set play mode */
     err = R_SCI_B_UART_Write(&g_uart_dfplayer_ctrl, playmode_cmd, strlen((char *)playmode_cmd));
     while (!uart_tx_done)
     {
         APP_PRINT("\r\n[DFP] Wait for completion of UART TX...\r\n");
         tk_dly_tsk(1);  // 軽く待機
     }
     tk_dly_tsk(2000); /* Wait a while until play mode is set. */

     return err;
}

fsp_err_t  dfplayer_playback(uint16_t track_number)
{
    fsp_err_t err = FSP_SUCCESS;
    char play_cmd[32];

    g_uart_event = RESET_VALUE;

    sprintf(play_cmd, "AT+PLAYNUM=%u\r\n", track_number);

    uart_tx_done = false;
    /* Play specified audio */
    err = R_SCI_B_UART_Write(&g_uart_dfplayer_ctrl, (uint8_t *)play_cmd, strlen(play_cmd));
    while (!uart_tx_done)
    {
        APP_PRINT("\r\n[DFP] Wait for completion of UART TX...\r\n");
        tk_dly_tsk(1);  // 軽く待機
    }
    tk_dly_tsk(1000);  /* Wait a while until audio playback finishes. */

    return err;
}

void dfplayer_task(INT stacd, void *exinf)
{
    T_DFP_CMD_MSG *msg = NULL;
    ER err;
    uint16_t track;

    APP_PRINT("[DFP] Task started.\n");

    dfplayer_init();

    while(1) {
        APP_PRINT("\r\n[DFP] Sleeping...\r\n");
        tk_slp_tsk(TMO_FEVR);
        APP_PRINT("[DFP] Waked up.\n");

        // Receive control message
        err = tk_rcv_mbx(MBX_KWS_TO_DFP, (T_MSG**)&msg, TMO_FEVR);

        APP_PRINT("[DFP] Received msg->track_no: %d\n", msg->track_no);
        track = (uint16_t)(msg->track_no + 1);
        APP_PRINT("[DFP] Play track No.%d\n", track);

        dfplayer_playback(track);
        APP_PRINT("[DFP] Audio playback finished. Wakup kws task.\n");

        tk_rel_mpl(MPL_KWS_TO_DFP, (void*)msg);

        err = tk_wup_tsk(kws_tskid);
        if (err < E_OK)
        {
            APP_ERR_PRINT("[DFP] tk_wup_tsk failed: %d\n", err);
        }
    }
}
