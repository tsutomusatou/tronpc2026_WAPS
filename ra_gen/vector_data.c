/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = pdm_sdet_isr, /* PDM SDET (Sound detection interrupt) */
            [1] = pdm_err_isr, /* PDM ERR2 (Error detection interrupt channel 2) */
            [2] = dmac_int_isr, /* DMAC1 INT (DMAC1 transfer end) */
            [3] = rm_ethosu_isr, /* NPU IRQ (NPU IRQ) */
            [4] = sci_b_uart_rxi_isr, /* SCI0 RXI (Receive data full) */
            [5] = sci_b_uart_txi_isr, /* SCI0 TXI (Transmit data empty) */
            [6] = sci_b_uart_tei_isr, /* SCI0 TEI (Transmit end) */
            [7] = sci_b_uart_eri_isr, /* SCI0 ERI (Receive error) */
            [8] = sci_b_uart_rxi_isr, /* SCI1 RXI (Receive data full) */
            [9] = sci_b_uart_txi_isr, /* SCI1 TXI (Transmit data empty) */
            [10] = sci_b_uart_tei_isr, /* SCI1 TEI (Transmit end) */
            [11] = sci_b_uart_eri_isr, /* SCI1 ERI (Receive error) */
            [12] = sci_b_uart_rxi_isr, /* SCI3 RXI (Receive data full) */
            [13] = sci_b_uart_txi_isr, /* SCI3 TXI (Transmit data empty) */
            [14] = sci_b_uart_tei_isr, /* SCI3 TEI (Transmit end) */
            [15] = sci_b_uart_eri_isr, /* SCI3 ERI (Receive error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_PDM_SDET,GROUP0), /* PDM SDET (Sound detection interrupt) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_PDM_ERR2,GROUP1), /* PDM ERR2 (Error detection interrupt channel 2) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_DMAC1_INT,GROUP2), /* DMAC1 INT (DMAC1 transfer end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_NPU_IRQ,GROUP3), /* NPU IRQ (NPU IRQ) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SCI0_RXI,GROUP4), /* SCI0 RXI (Receive data full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TXI,GROUP5), /* SCI0 TXI (Transmit data empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SCI0_TEI,GROUP6), /* SCI0 TEI (Transmit end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SCI0_ERI,GROUP7), /* SCI0 ERI (Receive error) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI1_RXI,GROUP0), /* SCI1 RXI (Receive data full) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TXI,GROUP1), /* SCI1 TXI (Transmit data empty) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_SCI1_TEI,GROUP2), /* SCI1 TEI (Transmit end) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_SCI1_ERI,GROUP3), /* SCI1 ERI (Receive error) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_SCI3_RXI,GROUP4), /* SCI3 RXI (Receive data full) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TXI,GROUP5), /* SCI3 TXI (Transmit data empty) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SCI3_TEI,GROUP6), /* SCI3 TEI (Transmit end) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI3_ERI,GROUP7), /* SCI3 ERI (Receive error) */
        };
        #endif
        #endif
