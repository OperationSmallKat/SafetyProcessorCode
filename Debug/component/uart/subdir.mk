################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/miniusart_adapter.c 

OBJS += \
./component/uart/miniusart_adapter.o 

C_DEPS += \
./component/uart/miniusart_adapter.d 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project/board" -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project/source" -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project" -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project/drivers" -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project/device" -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project/CMSIS" -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project/component/uart" -I"/home/kbisland/Documents/MCUXpresso_11.2.1_4149/workspace/LPC845_Project/utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


