################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/user/ir/ir_base.cpp \
../Core/user/ir/ir_code.cpp \
../Core/user/ir/ir_receiver.cpp \
../Core/user/ir/ir_sender.cpp \
../Core/user/ir/ir_time.cpp 

OBJS += \
./Core/user/ir/ir_base.o \
./Core/user/ir/ir_code.o \
./Core/user/ir/ir_receiver.o \
./Core/user/ir/ir_sender.o \
./Core/user/ir/ir_time.o 

CPP_DEPS += \
./Core/user/ir/ir_base.d \
./Core/user/ir/ir_code.d \
./Core/user/ir/ir_receiver.d \
./Core/user/ir/ir_sender.d \
./Core/user/ir/ir_time.d 


# Each subdirectory must supply rules for building sources it contributes
Core/user/ir/%.o Core/user/ir/%.su Core/user/ir/%.cyclo: ../Core/user/ir/%.cpp Core/user/ir/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-user-2f-ir

clean-Core-2f-user-2f-ir:
	-$(RM) ./Core/user/ir/ir_base.cyclo ./Core/user/ir/ir_base.d ./Core/user/ir/ir_base.o ./Core/user/ir/ir_base.su ./Core/user/ir/ir_code.cyclo ./Core/user/ir/ir_code.d ./Core/user/ir/ir_code.o ./Core/user/ir/ir_code.su ./Core/user/ir/ir_receiver.cyclo ./Core/user/ir/ir_receiver.d ./Core/user/ir/ir_receiver.o ./Core/user/ir/ir_receiver.su ./Core/user/ir/ir_sender.cyclo ./Core/user/ir/ir_sender.d ./Core/user/ir/ir_sender.o ./Core/user/ir/ir_sender.su ./Core/user/ir/ir_time.cyclo ./Core/user/ir/ir_time.d ./Core/user/ir/ir_time.o ./Core/user/ir/ir_time.su

.PHONY: clean-Core-2f-user-2f-ir

