################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g071cbtx.s 

OBJS += \
./Core/Startup/startup_stm32g071cbtx.o 

S_DEPS += \
./Core/Startup/startup_stm32g071cbtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32g071cbtx.o: ../Core/Startup/startup_stm32g071cbtx.s
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -c -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32g071cbtx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

