################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/user/irTime.cpp \
../Core/user/retarget.cpp \
../Core/user/username.cpp 

OBJS += \
./Core/user/irTime.o \
./Core/user/retarget.o \
./Core/user/username.o 

CPP_DEPS += \
./Core/user/irTime.d \
./Core/user/retarget.d \
./Core/user/username.d 


# Each subdirectory must supply rules for building sources it contributes
Core/user/%.o Core/user/%.su Core/user/%.cyclo: ../Core/user/%.cpp Core/user/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-user

clean-Core-2f-user:
	-$(RM) ./Core/user/irTime.cyclo ./Core/user/irTime.d ./Core/user/irTime.o ./Core/user/irTime.su ./Core/user/retarget.cyclo ./Core/user/retarget.d ./Core/user/retarget.o ./Core/user/retarget.su ./Core/user/username.cyclo ./Core/user/username.d ./Core/user/username.o ./Core/user/username.su

.PHONY: clean-Core-2f-user

