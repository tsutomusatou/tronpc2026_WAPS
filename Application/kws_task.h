#ifndef KWS_TASK_H
#define KWS_TASK_H

#include <tk/tkernel.h>

#include "common_utils.h"
#include "dfplayer_task.h"
#include "raspi_task.h"
#include "tangnano_task.h"
#include "pdm_ep.h"

extern ID    kws_tskid;            // Task ID number
extern T_CTSK kws_ctsk;

extern void kws_task(INT stacd, void *exinf);

extern ID init_flgid;;

#endif /* KWS_TASK_H */
