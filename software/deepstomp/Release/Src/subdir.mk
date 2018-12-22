################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/deepstomp.c \
../Src/deepstomp_delay.c \
../Src/deepstomp_distortion.c \
../Src/deepstomp_expansor.c \
../Src/deepstomp_modextension.c \
../Src/deepstomp_multieffect.c \
../Src/dstlib_dcremover.c \
../Src/dstlib_leveldetector.c \
../Src/main.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/deepstomp.o \
./Src/deepstomp_delay.o \
./Src/deepstomp_distortion.o \
./Src/deepstomp_expansor.o \
./Src/deepstomp_modextension.o \
./Src/deepstomp_multieffect.o \
./Src/dstlib_dcremover.o \
./Src/dstlib_leveldetector.o \
./Src/main.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/deepstomp.d \
./Src/deepstomp_delay.d \
./Src/deepstomp_distortion.d \
./Src/deepstomp_expansor.d \
./Src/deepstomp_modextension.d \
./Src/deepstomp_multieffect.d \
./Src/dstlib_dcremover.d \
./Src/dstlib_leveldetector.d \
./Src/main.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' -DARM_MATH_CM3 '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/PROJECTFILES/STM32WB/deepstomp/Inc" -I"C:/PROJECTFILES/STM32WB/deepstomp/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/PROJECTFILES/STM32WB/deepstomp/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/PROJECTFILES/STM32WB/deepstomp/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/PROJECTFILES/STM32WB/deepstomp/Drivers/CMSIS/Include"  -Og -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


