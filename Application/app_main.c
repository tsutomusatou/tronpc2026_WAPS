#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "common_utils.h"
#include "kws_task.h"
#include "dfplayer_task.h"
#include "raspi_task.h"
#include "tangnano_task.h"

ID init_flgid;

void idle_task(INT stacd, void *exinf);

ID    idle_tskid;
T_CTSK idle_ctsk = {
    .itskpri    = 30,
    .stksz      = 512,
    .task       = idle_task,
    .tskatr     = TA_HLNG | TA_RNG3,
};

void idle_task(INT stacd, void *exinf)
{
    while (1)
    {
        tk_dly_tsk(10);   // Wait for 10ms to keep uT-kernel ready
    }
}


void create_sync_flag(void)
{
    T_CFLG cflg = {
        .flgatr = TA_TFIFO | TA_WMUL,
        .iflgptn = 0
    };
    init_flgid = tk_cre_flg(&cflg);
}

/* usermain関数 */
EXPORT INT usermain(void)
{
    APP_PRINT("=== Start usermain program ===\n");

    create_sync_flag();

    /* Idle task */
    idle_tskid = tk_cre_tsk(&idle_ctsk);
    tk_sta_tsk(idle_tskid, 0);
    APP_PRINT("Idle Task ID: %d\n", idle_tskid);

    /* KWS (Key Word Spotting) task */
	kws_tskid = tk_cre_tsk(&kws_ctsk);
	tk_sta_tsk(kws_tskid, 0);
	APP_PRINT("KWS Task ID: %d\n", kws_tskid);

    /* DFPlayer task */
    dfplayer_tskid = tk_cre_tsk(&dfplayer_ctsk);
    tk_sta_tsk(dfplayer_tskid, 0);
    APP_PRINT("DFPlayer Task ID: %d\n", dfplayer_tskid);

    /* Raspi task */
    raspi_tskid = tk_cre_tsk(&raspi_ctsk);
    tk_sta_tsk(raspi_tskid, 0);
    APP_PRINT("Raspi Task ID: %d\n", raspi_tskid);

    /* Tang Nano task */
    tangnano_tskid = tk_cre_tsk(&tangnano_ctsk);
    tk_sta_tsk(tangnano_tskid, 0);
    APP_PRINT("Tang Nano Task ID: %d\n", tangnano_tskid);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
