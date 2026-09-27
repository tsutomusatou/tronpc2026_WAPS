#ifndef RASPI_TASK_H
#define RASPI_TASK_H

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include "common_utils.h"
#include "uart_raspi.h"

extern void raspi_task(INT stacd, void *exinf);

extern ID    raspi_tskid;            // Task ID number
extern T_CTSK raspi_ctsk;

#endif /* RASPI_TASK_H */
