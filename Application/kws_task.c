#include "kws_task.h"
#include "task_message.h"

ID    kws_tskid;            // Task ID number
T_CTSK kws_ctsk = {             // Task creation information
    .itskpri    = 5,
    .stksz      = 1024,
    .task       = kws_task,
    .tskatr     = TA_HLNG | TA_RNG3,
};

void kws_task(INT stacd, void *exinf)
{
    ER err;
    UINT flg;
    int inference_result;
    static T_DFP_CMD_MSG *dfp_msg;
    static T_RPI_CMD_MSG *rpi_msg;
    static T_TGN_CMD_MSG *tgn_msg;


    LOCAL T_CFLG    cflg_pdm_snddet = {
            .flgatr         = TA_TFIFO | TA_WMUL,   // イベントフラグの属性
            .iflgptn        = 0,                    // イベントフラグの初期値
    };

    flg_pdm_snddet = tk_cre_flg(&cflg_pdm_snddet);          // イベントフラグの生成

    while(1) {
        sound_detection_enable();

//        APP_PRINT("\r\n[KWS] Waiting for sound detection...\r\n");
        APP_PRINT("\r\n<< READY TO SPEAK >>\r\n");

        /* Make sure detect the sound before recording the audio */
        tk_wai_flg(flg_pdm_snddet, SOUND_DETECT, (TWF_ANDW | TWF_CLR), &flg, TMO_FEVR);   // Wait for notification from ISR

        sound_detection_disable();

        record_audio();

        audio_preprocessing();

        inference_result = inference();
        APP_PRINT("[KWS] Inference result = %d\n", inference_result);

        if (inference_result >= 0) { /* Wake up dfplayer task to play audio */
            tk_get_mpl(MPL_KWS_TO_DFP, sizeof(T_DFP_CMD_MSG), (void**)&dfp_msg, TMO_FEVR);

            dfp_msg->track_no = inference_result;

            tk_snd_mbx(MBX_KWS_TO_DFP, (T_MSG*)dfp_msg);

            APP_PRINT("[KWS] Wake up dfplayer task.\n");
            err = tk_wup_tsk(dfplayer_tskid);
            if (err < E_OK)
            {
                APP_PRINT("[KWS] tk_wup_tsk failed: %d\n", err);
            }

            tk_dly_tsk(5000);

            tk_slp_tsk(TMO_FEVR); /* Sleep until waked up by dfplayer task */
            APP_PRINT("[KWS] Waked up.\n");

            if (inference_result > 1) {
                int mode = inference_result + 2; ; // mode. 4: "konnichiwa", 5: "ohayo", 6: "oyasumi"
                /* Send a message to Tang Nano */
                tk_get_mpl(MPL_KWS_TO_TGN, sizeof(T_TGN_CMD_MSG), (void**)&tgn_msg, TMO_FEVR);
                tgn_msg->mode = mode;

                tk_snd_mbx(MBX_KWS_TO_TGN, (T_MSG*)tgn_msg);

                APP_PRINT("[KWS] Wake up tangnano task.\n");
                err = tk_wup_tsk(tangnano_tskid);
                if (err < E_OK)
                {
                    APP_PRINT("tk_wup_tsk failed: %d\n", err);
                }

                /* Send a message to Raspi */
                tk_get_mpl(MPL_KWS_TO_RPI, sizeof(T_RPI_CMD_MSG), (void**)&rpi_msg, TMO_FEVR);
                rpi_msg->mode = mode;

                tk_snd_mbx(MBX_KWS_TO_RPI, (T_MSG*)rpi_msg);

                APP_PRINT("[KWS] Wake up raspi task.\n");
                err = tk_wup_tsk(raspi_tskid);
                if (err < E_OK)
                {
                    APP_PRINT("tk_wup_tsk failed: %d\n", err);
                }

                tk_slp_tsk(TMO_FEVR); /* Sleep until waked up by Raspi task */
                APP_PRINT("[KWS] Waked up.\n");
            }
        }
    }
}
