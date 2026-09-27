#include "task_message.h"

ID MBX_KWS_TO_DFP;
ID MPL_KWS_TO_DFP;
ID MBX_KWS_TO_RPI;
ID MPL_KWS_TO_RPI;
ID MBX_KWS_TO_TGN;
ID MPL_KWS_TO_TGN;

void init_mailboxes(void)
{
    T_CMBX cmbx = { .mbxatr = TA_TFIFO };

    /* KWS -> DFP */
    MBX_KWS_TO_DFP = tk_cre_mbx(&cmbx);

    T_CMPL cmpl = {
        .mplatr = TA_TFIFO | TA_RNG3,
        .mplsz  = sizeof(T_DFP_CMD_MSG) * 5, /* 最大5個分の領域を確保 */
    };

    MPL_KWS_TO_DFP = tk_cre_mpl(&cmpl);

    /* KWS -> RPI */
    MBX_KWS_TO_RPI = tk_cre_mbx(&cmbx);

    T_CMPL cmpl_rpi = {
        .mplatr = TA_TFIFO | TA_RNG3,
        .mplsz  = sizeof(T_RPI_CMD_MSG) * 5, /* 最大5個分の領域を確保 */
    };

    MPL_KWS_TO_RPI = tk_cre_mpl(&cmpl_rpi);

    /* KWS -> TGN */
    MBX_KWS_TO_TGN = tk_cre_mbx(&cmbx);

    T_CMPL cmpl_tgn = {
        .mplatr = TA_TFIFO | TA_RNG3,
        .mplsz  = sizeof(T_TGN_CMD_MSG) * 5, /* 最大5個分の領域を確保 */
    };

    MPL_KWS_TO_TGN = tk_cre_mpl(&cmpl_tgn);
}
