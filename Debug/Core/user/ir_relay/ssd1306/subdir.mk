################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/user/ir_relay/ssd1306/fonts.c \
../Core/user/ir_relay/ssd1306/ssd1306.c 

C_DEPS += \
./Core/user/ir_relay/ssd1306/fonts.d \
./Core/user/ir_relay/ssd1306/ssd1306.d 

OBJS += \
./Core/user/ir_relay/ssd1306/fonts.o \
./Core/user/ir_relay/ssd1306/ssd1306.o 


# Each subdirectory must supply rules for building sources it contributes
Core/user/ir_relay/ssd1306/%.o Core/user/ir_relay/ssd1306/%.su Core/user/ir_relay/ssd1306/%.cyclo: ../Core/user/ir_relay/ssd1306/%.c Core/user/ir_relay/ssd1306/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir/controller" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-user-2f-ir_relay-2f-ssd1306

clean-Core-2f-user-2f-ir_relay-2f-ssd1306:
	-$(RM) ./Core/user/ir_relay/ssd1306/fonts.cyclo ./Core/user/ir_relay/ssd1306/fonts.d ./Core/user/ir_relay/ssd1306/fonts.o ./Core/user/ir_relay/ssd1306/fonts.su ./Core/user/ir_relay/ssd1306/ssd1306.cyclo ./Core/user/ir_relay/ssd1306/ssd1306.d ./Core/user/ir_relay/ssd1306/ssd1306.o ./Core/user/ir_relay/ssd1306/ssd1306.su

.PHONY: clean-Core-2f-user-2f-ir_relay-2f-ssd1306

