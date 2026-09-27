/***********************************************************************************************************************
 * File Name    : pdm_ep.h
 * Description  : Contains data structures and functions used in pdm_ep.c.
 **********************************************************************************************************************/
/***********************************************************************************************************************
* Copyright (c) 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/

#ifndef PDM_EP_2_H_
#define PDM_EP_2_H_

#include <stdint.h>
#include <stddef.h>
#include <tk/tkernel.h>
#include "common_utils.h"


/* Perform VOX wakeup MCU from LPM SW standby operation */
//#define ENABLE_VOX_WAKEUP           (0U)

/* Perform playback the recorded audio operation */
//#define ENABLE_PLAYBACK             (0U)

/* PDM microphones startup time, RA8P1_EK microphones startup time is 35ms */
#define PDM_MIC_STARTUP_TIME_US     (35000)
//#define PDM_SDE_UPPER_LIMIT         (1000)
//#define PDM_SDE_LOWER_LIMIT         (0xFFF80000)
#define PDM_SDE_UPPER_LIMIT         (3000)
#define PDM_SDE_LOWER_LIMIT         (0xFFE80000)

/* Output sample rate after PDM decimation (Hz) */
#define PDM_FS_HZ                   (32000U)

/* Number of interleaved channels: 1 = mono, 2 = stereo
 * This EP only supports a mono microphone */
#define PDM_CHANNELS                (1U)

/* Target duration (seconds) of audio captured in the buffer */
#define PDM_DURATION_MIN_IN_SEC     (1U)
#define PDM_DURATION_MAX_IN_SEC     (1U)

/* PCM output data bits per sample */
#define PCM_16BITS                  (16U)
#define PCM_20BITS                  (20U)

/* PDM alignment (samples per interrupt/frame per channel) */
#define PDM_FRAME_SAMPLES           ((1<<PDM_INTERRUPT_THRESHOLD_16) * PDM_CHANNELS )

/* Align n up to the next multiple of a (a must be > 0) */
#define PDM_ALIGN_UP(n,a)           (((uint32_t)((n) + (a) - 1U) / (uint32_t)(a)) * (uint32_t)(a) )

/*
 * Total number of interleaved samples.
 * samples_raw = duration * Fs_out * channels
 * samples     = align_up(samples_raw, frame_samples)
 */
#define RECORD_BUFFER_SAMPLE    (32000U)
#define ONE_SEC_FRAME_SAMPLE    (32000U)
//#define RECORD_BUFFER_SAMPLE    PDM_ALIGN_UP((uint32_t)((uint64_t)PDM_DURATION_MAX_IN_SEC * PDM_FS_HZ * PDM_CHANNELS), PDM_FRAME_SAMPLES)
//#define ONE_SEC_FRAME_SAMPLE    PDM_ALIGN_UP((uint32_t)((uint64_t)PDM_DURATION_MIN_IN_SEC * PDM_FS_HZ * PDM_CHANNELS), PDM_FRAME_SAMPLES)

/* PDM buffer size */
typedef struct
{
    uint32_t samples;  /* Interleaved sample count, aligned to frame size */
    uint32_t bytes;    /* Buffer size in bytes */
} pdm_buf_size_t;

/* Public functions declarations */
void pdm_ep_init(void);
void pdm_ep_entry(void);
void handle_error(fsp_err_t err, char * err_str);
void sound_detection_enable(void);
void sound_detection_disable(void);
void record_audio(void);
void audio_preprocessing(void);
int inference(void);


extern ID flg_pdm_snddet;
#define SOUND_DETECT (1<<0)

#endif /* PDM_EP_H_ */
