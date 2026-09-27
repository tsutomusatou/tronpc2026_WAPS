#ifndef DFPLAYER_TASK_H
#define DFPLAYER_TASK_H

#include <tk/tkernel.h>

#include "common_utils.h"
#include "uart_dfplayer.h"

extern ID    dfplayer_tskid;            // Task ID number
extern T_CTSK dfplayer_ctsk;

extern ID mbx_dfp_cmd;
extern T_CMBX cmbx_dfp_cmd;

extern void dfplayer_task(INT stacd, void *exinf);

extern ID init_flgid;;

#endif /* DFPLAYER_TASK_H */
