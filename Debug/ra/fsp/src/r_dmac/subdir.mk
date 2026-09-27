################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/r_dmac/r_dmac.c 

C_DEPS += \
./ra/fsp/src/r_dmac/r_dmac.d 

OBJS += \
./ra/fsp/src/r_dmac/r_dmac.o 

SREC += \
mtk3_ra8p1_ek_wasp_r1.srec 

MAP += \
mtk3_ra8p1_ek_wasp_r1.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/r_dmac/%.o: ../ra/fsp/src/r_dmac/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RAFSP_EK_RA8P1_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -DUSE_VIRTUAL_COM=0 -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra_gen" -I"." -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra_cfg/fsp_cfg/bsp" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra_cfg/fsp_cfg" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/src" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/fsp/inc" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/fsp/inc/api" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/fsp/inc/instances" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/mtk3_bsp2" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/mtk3_bsp2/config" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/mtk3_bsp2/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/mtk3_bsp2/mtkernel/kernel/knlinc" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/arm/CMSIS-DSP/PrivateInclude" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/arm/CMSIS-DSP/Include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/fsp/src/rm_ethosu" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/ethos-u-core-driver/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/src/ruhmi_model" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/arm/CMSIS-NN/Include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/tflite-micro" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/ruy" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/gemmlowp" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/ethos-u-core-software/lib/layer_by_layer_profiler/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/ethos-u-core-software/lib/ethosu_monitor/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/ethos-u-core-software/lib/ethosu_profiler/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/ethos-u-core-software/lib/crc/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/ethos-u-core-software/lib/arm_profiler/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/arm/CMSIS-View/EventRecorder/Include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/arm/CMSIS-View/EventRecorder/Config" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/mtk3_ra8p1_ek_wasp_r1/ra/npu/flatbuffers/include" -std=c99 -Os -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

