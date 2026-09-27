/***********************************************************************************************************************
 * File Name    : pdm_ep.c
 * Description  : Contains data structures and functions use to record audio.
 **********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#include "pdm_ep.h"
#include "model.h"
#include "preprocessing.h"
#include "sample_audio.h"

/* Private function declaration */
static pdm_buf_size_t pdm_calc_buffer_from_seconds(uint8_t seconds);
static fsp_err_t pdm_error_check(void);
static void check_audio_data(void);
//static void sound_detection_enable(void);
//static void sound_detection_disable(void);
//static void record_audio(void);
//static void audio_preprocessing(void);
//static void inference(void);

/* Public global variables */
int16_t g_pcm16_buffer[ONE_SEC_FRAME_SAMPLE * 2] = {0}; // ping-pong buffer
int16_t g_record_buffer[RECORD_BUFFER_SAMPLE * 2] BSP_PLACE_IN_SECTION (BSP_UNINIT_SECTION_PREFIX ".sdram") = {0};
//int32_t g_pcm32_buffer [ONE_SEC_FRAME_SAMPLE * 2] = {0};
//int32_t g_record_buffer[RECORD_BUFFER_SAMPLE] BSP_PLACE_IN_SECTION (BSP_UNINIT_SECTION_PREFIX ".sdram") = {0};


int16_t downsampled[16000] BSP_PLACE_IN_SECTION (BSP_UNINIT_SECTION_PREFIX ".sdram") = {0};

pdm_buf_size_t g_buf_size = {RESET_VALUE, RESET_VALUE};
uint8_t g_pcm_bits = RESET_VALUE;
uint8_t recorded_seconds = RESET_VALUE;
static int8_t g_mfcc_buffer[MODEL_INPUT_SIZE];

/* Private global variables */
static volatile bool g_stop_receive_data = false;
static volatile bool g_sound_detect = false;
static volatile pdm_error_t g_pdm_error = PDM_ERROR_NONE;
static uint8_t total_record_seconds = RESET_VALUE;
static uint8_t g_pcm16_frame_idx = RESET_VALUE;

#define AUDIO_DATA_MAX 32767
#define AUDIO_DATA_MIN -32768

/* Sound detection Event Flag */
ID flg_pdm_snddet;

#undef AUDIO_DEBUG
#undef USE_SAMPLE_AUDIO

#ifdef AUDIO_DEBUG
void check_audio_data()
{
    int length = RECORD_BUFFER_SAMPLE;

    int data_bits = g_pcm_bits;   // 16 bits
    float audio_scale = 32768.0f; // 32768.0f (2^15)

    APP_PRINT("\n=== %d-bit Audio Data Check ===\n", data_bits);

    // 最初の10サンプルを表示
    APP_PRINT("First 10 samples:\n");
    int display_count = (length < 10) ? length : 10;

    for(int i = 0; i < display_count; i++) {
        // 修正：符号拡張を行う
        int16_t raw = g_record_buffer[i];
        float normalized = (float)raw / audio_scale;

        // これで、画面には「-24」や「-1」といった正しいマイナスの値が表示されるようになります
        APP_PRINT("  [%d]: %8d (0x%08X) -> %+.6f\n",
               i, raw, (unsigned int)g_record_buffer[i], normalized);
    }

    // 最大値・最小値を確認
    int16_t max_val = INT16_MIN;
    int16_t min_val = INT16_MAX;

    int check_samples = (length < 1000) ? length : 1000;

    for(int i = 0; i < check_samples; i++) {
        if(g_record_buffer[i] > max_val) max_val = g_record_buffer[i];
        if(g_record_buffer[i] < min_val) min_val = g_record_buffer[i];
    }

    APP_PRINT("\nRange check (first %d samples):\n", check_samples);
    APP_PRINT("  Max value: %d (0x%08X)\n", max_val, (unsigned int)max_val);
    APP_PRINT("  Min value: %d (0x%08X)\n", min_val, (unsigned int)min_val);
    APP_PRINT("  Expected range: -%d to +%d (%d-bit)\n", (int)audio_scale, (int)(audio_scale-1.0f), data_bits);

    // 正規化後の範囲確認
    float max_norm = (float)max_val / audio_scale;
    float min_norm = (float)min_val / audio_scale;
    APP_PRINT("\nNormalized range:\n");
    APP_PRINT("  Max: (x 1000)%d\n", (int)(max_norm * 1000.0f));
    APP_PRINT("  Min: (x 1000)%d\n", (int)(min_norm * 1000.0f));
    APP_PRINT("  Expected: approximately -1.0 to +1.0\n");

    // データの妥当性チェック
    if(max_val > (int32_t)(audio_scale-1.0f) || min_val < -(audio_scale)) {
        APP_PRINT("\n WARNING: Data out of %d-bit range!\n", data_bits);
    } else {
        APP_PRINT("\n Data is within valid %d-bit range\n", data_bits);
    }

    // 無音チェック
    int zero_count = 0;
    for(int i = 0; i < check_samples; i++) {
        if(g_record_buffer[i] == 0) zero_count++;
    }
    if(zero_count > check_samples * 9 / 10) {
        APP_PRINT("\n WARNING: Too many zero samples (%d/%d). Check microphone!\n",
               zero_count, check_samples);
    }
}
#endif

/* Initialize PDMIF (Pulse Density Modulation Interface) */
void pdm_ep_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Initialize PDM module */
    err = R_PDM_Open(&g_pdm0_ctrl, &g_pdm0_cfg);
    handle_error(err, "**R_PDM_Open API failed**\r\n");

    /* Wait for filter settling and startup time */
    R_BSP_SoftwareDelay(PDM2_FILTER_SETTLING_TIME_US + PDM_MIC_STARTUP_TIME_US, BSP_DELAY_UNITS_MICROSECONDS);

    /* Set PCM bits output data */
    g_pcm_bits = PCM_16BITS;
    APP_PRINT("PCM mode = %d bits\r\n", g_pcm_bits);
}

void sound_detection_enable(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Configure the sound detection range */
    pdm_sound_detection_setting_t sound_detection_setting =
    {
        .sound_detection_lower_limit = PDM_SDE_LOWER_LIMIT,
        .sound_detection_upper_limit = PDM_SDE_UPPER_LIMIT
    };

    /* Enable sound detection */
    err = R_PDM_SoundDetectionEnable(&g_pdm0_ctrl, sound_detection_setting);
    handle_error(err, "**R_PDM_SoundDetectionEnable API failed**\r\n");
}

void sound_detection_disable(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Disable sound detection */
    err = R_PDM_SoundDetectionDisable(&g_pdm0_ctrl);
    handle_error(err, "**R_PDM_SoundDetectionDisable API failed**\r\n");
}

void record_audio(void)
{
    fsp_err_t err = FSP_SUCCESS;

    total_record_seconds = 1;

    /* Calculate size of record audio */
    g_buf_size = pdm_calc_buffer_from_seconds(total_record_seconds);

    APP_PRINT("\r\nStart recording in %d seconds ...\r\n", total_record_seconds);

    /* Start recording data */
    err = R_PDM_Start(&g_pdm0_ctrl, g_pcm16_buffer, sizeof(g_pcm16_buffer), ONE_SEC_FRAME_SAMPLE);
    handle_error(err, "**R_PDM_Start API failed**\r\n");


    /* Wait until the desired data is fully recorded */
    while (!g_stop_receive_data);

    /* Clear g_stop_receive_data flag */
    g_stop_receive_data = false;

    /* Handle all occurred error events */
    err = pdm_error_check();
    handle_error(err, "PDM errors are detected\r\n");

    APP_PRINT("\r\nRecord successfully\r\n");

    /* Stop recording */
    err = R_PDM_Stop(&g_pdm0_ctrl);
    handle_error(err, "**R_PDM_Stop API failed**\r\n");

#ifdef AUDIO_DEBUG
    check_audio_data();
#endif
}

void audio_preprocessing(void)
{
    for (int i = 0; i < (int)RECORD_BUFFER_SAMPLE * 2; i += 4)
    {
         int16_t raw_sample = (int16_t)g_record_buffer[i];

         /* Make data value larger. << 1 means x2 */
         int16_t amplified = raw_sample << 1;

         /* Guard value within 16 bit range */
         if (amplified > AUDIO_DATA_MAX)  amplified = AUDIO_DATA_MAX;
         if (amplified < AUDIO_DATA_MIN) amplified = AUDIO_DATA_MIN;

         /* Put data in buffer for inference */
         downsampled[i/4] = (int16_t)amplified;
    }

#ifdef USE_SAMPLE_AUDIO // Overwritten by sample audio data
    for (int i = 0; i < 16000; i++)
    {
        downsampled[i] = sample_audio[i];
    }
#endif

    /* Pre-process to create input tensor for inference.  */
    preprocess(downsampled, 16000, g_mfcc_buffer);

#if 0 /* debug */
    int8_t *mfcc = g_mfcc_buffer;
    for (int i = 0; i < 50; i++) {
        APP_PRINT("%d, ", mfcc[i]);
        if ((i+1) % 10 == 0)
            APP_PRINT("\n");
    }
#endif
}

int inference(void)
{
    /* Copy input tensor to NPU */
    memcpy(GetModelInputPtr_serving_default_keras_tensor_0(), g_mfcc_buffer, MODEL_INPUT_SIZE);

    /* Inference */
    APP_PRINT("Inference starts...\n");
    RunModel(false);

    /* Get result */
    int8_t *output_scores = (int8_t*)GetModelOutputPtr_StatefulPartitionedCall_1_0_70032();

    /* The number of classes */
    int num_classes = 5;
    /* Label of each class */
    const char* labels[] =  { "silence", "unknown", "konnichiwa", "ohayo", "oyasumi" };


    int max_index = 0;
    int8_t max_score = -128; /* Initialize with minimum value of INT8 */

    /* Look for a class with the highest possibility */
    for (int i = 0; i < num_classes; i++) {
        if (output_scores[i] > max_score) {
            max_score = output_scores[i];
            max_index = i;
        }
    }

    APP_PRINT("Max Candidate: %s (Score: %d)\r\n", labels[max_index], max_score);

    /* INT8 quantized score 0 - 128 after softmax means high score */
    /* max_score > 0 : 50% possibility --> Threshold to detect a class */
//    if ((max_index > 1 && max_score == 127) || max_index < 2) {
    if (((max_index == 2 || max_index == 4) && max_score == 127) \
            || (max_index == 3 && max_score > 100) \
            || (max_index < 2))
    {
        APP_PRINT(">> Detected: %s\r\n", labels[max_index]);
    } else {
        APP_PRINT(">> Not Sure: Score is low.\r\n");
        max_index = -1; /* Not play audio */
    }

    return max_index;
}

#if 0
/***********************************************************************************************************************
 *  Function Name: pdm_ep_entry
 *  Description  : This function is used to start PDM example operation.
 *  Arguments    : None.
 *  Return Value : None.
 **********************************************************************************************************************/
void pdm_ep_entry(void)
{
    while (true)
    {
        sound_detection_enable();

        APP_PRINT("\r\n\r\nWaiting for sound detection to begin recording...\r\n");

        /* Make sure detect the sound before recording the audio */
        while (!g_sound_detect) {;}

        /* Reset g_sound_detect flag */
        g_sound_detect = false;

        sound_detection_disable();

        record_audio();

        audio_preprocessing();

        inference();
    }
}
#endif

/***********************************************************************************************************************
 *  Function Name: pdm_calc_buffer_from_seconds
 *  Description  : This function is used to calculate the recorded buffer size.
 *  Arguments    : None.
 *  Return Value : Recorded buffer size (in samples and bytes).
 **********************************************************************************************************************/
static pdm_buf_size_t pdm_calc_buffer_from_seconds(uint8_t seconds)
{
    uint32_t raw = seconds * PDM_FS_HZ * PDM_CHANNELS;

    /* Protect against overflow prior to cast */
    raw = raw > UINT32_MAX ? UINT32_MAX : raw;

    /* Round to nearest sample and align up to the frame boundary */
    uint32_t samples     = PDM_ALIGN_UP(raw, PDM_FRAME_SAMPLES);

    /* Calculate and round up the used bytes */
    uint32_t bytes       = samples * ((g_pcm_bits + (8-1)) / 8U);

    pdm_buf_size_t out   = {samples, bytes};

//    APP_PRINT("\r\nSamples %d, Bytes %d\r\n", samples, bytes);

    return out;
}
/***********************************************************************************************************************
* End of function pdm_calc_buffer_from_seconds.
***********************************************************************************************************************/

/***********************************************************************************************************************
 *  Function Name: pdm_callback
 *  Description  : This function is used to handle PDM event.
 *  Arguments    : p_args      Pointer to PDM callback argument.
 *  Return Value : None.
 **********************************************************************************************************************/
void pdm_callback(pdm_callback_args_t *p_args)
{
//    uint32_t dst_offset = ONE_SEC_FRAME_SAMPLE * recorded_seconds;        /* Record buffer offset */
    uint32_t dst_offset = 0;        /* Record buffer offset */
    uint32_t src_offset = 0; //g_pcm16_frame_idx * ONE_SEC_FRAME_SAMPLE;       /* Ping-pong buffer offset */
    int16_t *p_write = &g_record_buffer[dst_offset];
    int16_t *p_read = &g_pcm16_buffer[src_offset];

    switch (p_args->event)
    {
        case PDM_EVENT_DATA:
        {
            /* Check recording completion */
            if (recorded_seconds >= total_record_seconds)
            {
                recorded_seconds = RESET_VALUE;
                g_stop_receive_data = true;
                break;
            }

            /*  Left-align data to MSB */
            for (uint32_t i = 0; i < ONE_SEC_FRAME_SAMPLE; i++)
            {
                *p_write++ = *p_read++;
            }

            /* Update state */
            recorded_seconds++;
            g_pcm16_frame_idx ^= 1U;

            break;
        }

        case PDM_EVENT_SOUND_DETECTION:
        {
            tk_set_flg(flg_pdm_snddet, SOUND_DETECT);              // Wake up uros_uart_task
            g_sound_detect = true;
            break;
        }

        case PDM_EVENT_ERROR:
        {
            g_pdm_error = p_args->error;
            break;
        }

        default:
            break;
     }
}
/***********************************************************************************************************************
* End of function pdm_callback.
***********************************************************************************************************************/

/***********************************************************************************************************************
 *  Function Name: pdm_error_check
 *  Description  : This function is used to check the PDM errors.
 *  Arguments    : None.
 *  Return Value : FSP_SUCCESS    Upon successful operation.
 *                 Any other error code apart from FSP_SUCCESS.
 **********************************************************************************************************************/
static fsp_err_t pdm_error_check(void)
{
    fsp_err_t err = FSP_SUCCESS;

    if (PDM_ERROR_NONE != g_pdm_error)
    {
        if (g_pdm_error & PDM_ERROR_BUFFER_OVERWRITE)
        {
            APP_ERR_PRINT("Buffer overwrite. Decrease PDM clock output\r\n");
        }
        if (g_pdm_error & PDM_ERROR_SHORT_CIRCUIT)
        {
            APP_ERR_PRINT("Short circuit detected on PDM_DATAn pin. Please check microphone connection\r\n");
        }
        if (g_pdm_error & PDM_ERROR_OVERVOLTAGE_LOWER)
        {
            APP_ERR_PRINT("Low voltage. Verify microphone output and threshold settings\r\n");
        }
        if (g_pdm_error & PDM_ERROR_OVERVOLTAGE_UPPER)
        {
            APP_ERR_PRINT("High voltage. Verify microphone output and threshold settings\r\n");
        }

        return FSP_ERR_ASSERTION;
    }

    return err;
}
/***********************************************************************************************************************
* End of function pdm_error_check.
***********************************************************************************************************************/

/***********************************************************************************************************************
 *  Function Name: handle_error
 *  Description  : This function is used to close all opened modules, print and trap error.
 *  Arguments    : err            error code.
 *                 err_str        error string.
 *  Return Value : None.
 **********************************************************************************************************************/
void handle_error(fsp_err_t err, char * err_str)
{
    if (FSP_SUCCESS != err)
    {
        /* Print the error */
        APP_PRINT(err_str);

        /* Close opened PDM module*/
        if (MODULE_CLOSE != g_pdm0_ctrl.open)
        {
            if (FSP_SUCCESS != R_PDM_Close(&g_pdm0_ctrl))
            {
                APP_ERR_PRINT("** R_PDM_Close API failed **\r\n");
            }
        }

        /* Close opened DMAC module*/
//        if (MODULE_CLOSE != g_transfer0_ctrl.open)
//        {
//            if (FSP_SUCCESS != R_DMAC_Close(&g_transfer0_ctrl))
//            {
//                APP_ERR_PRINT("** R_DMAC_Close API failed **\r\n");
//            }
//        }

        /* Close opened AGT module*/
//        if (MODULE_CLOSE != g_timer0_ctrl.open)
//        {
//            if(FSP_SUCCESS != R_AGT_Close(&g_timer0_ctrl))
//            {
//                APP_ERR_PRINT("** R_AGT_Close API failed **\r\n");
//            }
//        }

        /* Close opened DAC_B module*/
//        if (MODULE_CLOSE != g_dac_b0_ctrl.channel_opened)
//        {
//            if (FSP_SUCCESS != R_DAC_B_Close(&g_dac_b0_ctrl))
//            {
//                APP_ERR_PRINT("** R_DAC_B_Close API failed **\r\n");
//            }
//        }

        /* Close opened LPM SW standby module*/
//        if (MODULE_CLOSE != g_sw_standby_ctrl.lpm_open)
//        {
//            if (FSP_SUCCESS != R_LPM_Close(&g_sw_standby_ctrl))
//            {
//                APP_ERR_PRINT("** R_LPM_Close API failed **\r\n");
//            }
//        }

        /* Trap the error */
        APP_ERR_TRAP(err);
    }
}
/***********************************************************************************************************************
* End of function handle_error.
***********************************************************************************************************************/
