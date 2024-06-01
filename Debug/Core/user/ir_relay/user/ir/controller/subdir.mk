################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/user/ir_relay/user/ir/controller/controller_casio.cpp \
../Core/user/ir_relay/user/ir/controller/controller_daikin.cpp \
../Core/user/ir_relay/user/ir/controller/controller_fan.cpp \
../Core/user/ir_relay/user/ir/controller/controller_hdrplayer.cpp \
../Core/user/ir_relay/user/ir/controller/controller_kns.cpp \
../Core/user/ir_relay/user/ir/controller/controller_mod.cpp \
../Core/user/ir_relay/user/ir/controller/controller_pioneer_dvd.cpp \
../Core/user/ir_relay/user/ir/controller/controller_toshiba_tv.cpp \
../Core/user/ir_relay/user/ir/controller/controller_toshiba_vcr.cpp 

OBJS += \
./Core/user/ir_relay/user/ir/controller/controller_casio.o \
./Core/user/ir_relay/user/ir/controller/controller_daikin.o \
./Core/user/ir_relay/user/ir/controller/controller_fan.o \
./Core/user/ir_relay/user/ir/controller/controller_hdrplayer.o \
./Core/user/ir_relay/user/ir/controller/controller_kns.o \
./Core/user/ir_relay/user/ir/controller/controller_mod.o \
./Core/user/ir_relay/user/ir/controller/controller_pioneer_dvd.o \
./Core/user/ir_relay/user/ir/controller/controller_toshiba_tv.o \
./Core/user/ir_relay/user/ir/controller/controller_toshiba_vcr.o 

CPP_DEPS += \
./Core/user/ir_relay/user/ir/controller/controller_casio.d \
./Core/user/ir_relay/user/ir/controller/controller_daikin.d \
./Core/user/ir_relay/user/ir/controller/controller_fan.d \
./Core/user/ir_relay/user/ir/controller/controller_hdrplayer.d \
./Core/user/ir_relay/user/ir/controller/controller_kns.d \
./Core/user/ir_relay/user/ir/controller/controller_mod.d \
./Core/user/ir_relay/user/ir/controller/controller_pioneer_dvd.d \
./Core/user/ir_relay/user/ir/controller/controller_toshiba_tv.d \
./Core/user/ir_relay/user/ir/controller/controller_toshiba_vcr.d 


# Each subdirectory must supply rules for building sources it contributes
Core/user/ir_relay/user/ir/controller/%.o Core/user/ir_relay/user/ir/controller/%.su Core/user/ir_relay/user/ir/controller/%.cyclo: ../Core/user/ir_relay/user/ir/controller/%.cpp Core/user/ir_relay/user/ir/controller/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F756xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir/controller" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user/ir" -I"/Users/nicolelou/STM32CubeIDE/1126104/Core/user/ir_relay/user" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-user-2f-ir_relay-2f-user-2f-ir-2f-controller

clean-Core-2f-user-2f-ir_relay-2f-user-2f-ir-2f-controller:
	-$(RM) ./Core/user/ir_relay/user/ir/controller/controller_casio.cyclo ./Core/user/ir_relay/user/ir/controller/controller_casio.d ./Core/user/ir_relay/user/ir/controller/controller_casio.o ./Core/user/ir_relay/user/ir/controller/controller_casio.su ./Core/user/ir_relay/user/ir/controller/controller_daikin.cyclo ./Core/user/ir_relay/user/ir/controller/controller_daikin.d ./Core/user/ir_relay/user/ir/controller/controller_daikin.o ./Core/user/ir_relay/user/ir/controller/controller_daikin.su ./Core/user/ir_relay/user/ir/controller/controller_fan.cyclo ./Core/user/ir_relay/user/ir/controller/controller_fan.d ./Core/user/ir_relay/user/ir/controller/controller_fan.o ./Core/user/ir_relay/user/ir/controller/controller_fan.su ./Core/user/ir_relay/user/ir/controller/controller_hdrplayer.cyclo ./Core/user/ir_relay/user/ir/controller/controller_hdrplayer.d ./Core/user/ir_relay/user/ir/controller/controller_hdrplayer.o ./Core/user/ir_relay/user/ir/controller/controller_hdrplayer.su ./Core/user/ir_relay/user/ir/controller/controller_kns.cyclo ./Core/user/ir_relay/user/ir/controller/controller_kns.d ./Core/user/ir_relay/user/ir/controller/controller_kns.o ./Core/user/ir_relay/user/ir/controller/controller_kns.su ./Core/user/ir_relay/user/ir/controller/controller_mod.cyclo ./Core/user/ir_relay/user/ir/controller/controller_mod.d ./Core/user/ir_relay/user/ir/controller/controller_mod.o ./Core/user/ir_relay/user/ir/controller/controller_mod.su ./Core/user/ir_relay/user/ir/controller/controller_pioneer_dvd.cyclo ./Core/user/ir_relay/user/ir/controller/controller_pioneer_dvd.d ./Core/user/ir_relay/user/ir/controller/controller_pioneer_dvd.o ./Core/user/ir_relay/user/ir/controller/controller_pioneer_dvd.su ./Core/user/ir_relay/user/ir/controller/controller_toshiba_tv.cyclo ./Core/user/ir_relay/user/ir/controller/controller_toshiba_tv.d ./Core/user/ir_relay/user/ir/controller/controller_toshiba_tv.o ./Core/user/ir_relay/user/ir/controller/controller_toshiba_tv.su ./Core/user/ir_relay/user/ir/controller/controller_toshiba_vcr.cyclo ./Core/user/ir_relay/user/ir/controller/controller_toshiba_vcr.d ./Core/user/ir_relay/user/ir/controller/controller_toshiba_vcr.o ./Core/user/ir_relay/user/ir/controller/controller_toshiba_vcr.su

.PHONY: clean-Core-2f-user-2f-ir_relay-2f-user-2f-ir-2f-controller

