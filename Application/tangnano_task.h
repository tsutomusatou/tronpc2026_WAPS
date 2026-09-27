#ifndef TANGNANO_TASK_H
#define TANGNANO_TASK_H

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "common_utils.h"
#include "uart_tangnano.h"

extern void tangnano_task(INT stacd, void *exinf);

extern ID    tangnano_tskid;            // Task ID number
extern T_CTSK tangnano_ctsk;

#endif /* TANGNANO_TASK_H */
