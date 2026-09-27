#include <tk/tkernel.h>

/* Mailbox ID */
extern ID MBX_KWS_TO_DFP;
extern ID MPL_KWS_TO_DFP;

extern ID MBX_KWS_TO_RPI;
extern ID MPL_KWS_TO_RPI;

extern ID MBX_KWS_TO_TGN;
extern ID MPL_KWS_TO_TGN;

/* Message structure */
typedef struct {
    T_MSG msg;
    int track_no;
} T_DFP_CMD_MSG;

typedef struct {
    T_MSG msg;
    int mode;
} T_RPI_CMD_MSG;

typedef struct {
    T_MSG msg;
    int mode;
} T_TGN_CMD_MSG;
