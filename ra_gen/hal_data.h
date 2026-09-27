/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_sci_b_uart.h"
#include "r_uart_api.h"
#include "r_dmac.h"
#include "r_transfer_api.h"
#include "r_pdm_api.h"
#include "r_pdm.h"
FSP_HEADER
/** UART on SCI Instance. */
extern const uart_instance_t g_uart_tangnano;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_b_uart_instance_ctrl_t g_uart_tangnano_ctrl;
extern const uart_cfg_t g_uart_tangnano_cfg;
extern const sci_b_uart_extended_cfg_t g_uart_tangnano_cfg_extend;

#ifndef uart_tangnano_callback
void uart_tangnano_callback(uart_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart_raspi;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_b_uart_instance_ctrl_t g_uart_raspi_ctrl;
extern const uart_cfg_t g_uart_raspi_cfg;
extern const sci_b_uart_extended_cfg_t g_uart_raspi_cfg_extend;

#ifndef uart_raspi_callback
void uart_raspi_callback(uart_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_uart_dfplayer;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_b_uart_instance_ctrl_t g_uart_dfplayer_ctrl;
extern const uart_cfg_t g_uart_dfplayer_cfg;
extern const sci_b_uart_extended_cfg_t g_uart_dfplayer_cfg_extend;

#ifndef uart_dfplayer_callback
void uart_dfplayer_callback(uart_callback_args_t *p_args);
#endif
/* Transfer on DMAC Instance. */
extern const transfer_instance_t g_transfer1;

/** Access the DMAC instance using these structures when calling API functions directly (::p_api is not used). */
extern dmac_instance_ctrl_t g_transfer1_ctrl;
extern const transfer_cfg_t g_transfer1_cfg;

#ifndef pdm_rxi_dmac_isr
void pdm_rxi_dmac_isr(transfer_callback_args_t *p_args);
#endif
/* Sinc Decimation ratio has been rounded to the nearest integer.
 * Target Sampling Frequency: 32000 Hz
 * Actual Sampling Frequency: 32258 Hz */
#define PDM2_CALCULATED_SINCRNG_VALUE (9)
#define PDM2_CALCULATED_SINCDEC_VALUE (62)
#define PDM2_FILTER_SETTLING_TIME_US  (841)

/** PDM Instance. */
extern const pdm_instance_t g_pdm0;

/** Access the PDM instance using these structures when calling API functions directly (::p_api is not used). */
extern pdm_instance_ctrl_t g_pdm0_ctrl;
extern const pdm_cfg_t g_pdm0_cfg;

#ifndef pdm_callback
void pdm_callback(pdm_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
