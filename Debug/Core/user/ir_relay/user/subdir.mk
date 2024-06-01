################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/user/ir_relay/user/usermain.cpp 

C_SRCS += \
../Core/user/ir_relay/user/font5x7.c 

C_DEPS += \
./Core/user/ir_relay/user/font5x7.d 

OBJS += \
./Core/user/ir_relay/user/font5x7.o \
./Core/user/ir_relay/user/usermain.o 

CPP_DEPS += \
./Core/user/ir_relay/user/usermain.d 


# Each subdirectory must supply rules for building sources it contributes
Core/user/ir_relay/user/%.o Core/user/ir_relay/user/%.su Core/user/ir_relay/user/%.cyclo: ../Core/user/ir_relay/user/%.c Core/user/ir_relay/user/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir/controller" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/user/ir_relay/user/%.o Core/user/ir_relay/user/%.su Core/user/ir_relay/user/%.cyclo: ../Core/user/ir_relay/user/%.cpp Core/user/ir_relay/user/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir/controller" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-user-2f-ir_relay-2f-user

clean-Core-2f-user-2f-ir_relay-2f-user:
	-$(RM) ./Core/user/ir_relay/user/font5x7.cyclo ./Core/user/ir_relay/user/font5x7.d ./Core/user/ir_relay/user/font5x7.o ./Core/user/ir_relay/user/font5x7.su ./Core/user/ir_relay/user/usermain.cyclo ./Core/user/ir_relay/user/usermain.d ./Core/user/ir_relay/user/usermain.o ./Core/user/ir_relay/user/usermain.su

.PHONY: clean-Core-2f-user-2f-ir_relay-2f-user

