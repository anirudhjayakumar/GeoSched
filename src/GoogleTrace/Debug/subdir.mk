################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GoogleTrace.cpp \
../JobEvent.cpp \
../TaskEvent.cpp \
../common.cpp \
../main.cpp 

OBJS += \
./GoogleTrace.o \
./JobEvent.o \
./TaskEvent.o \
./common.o \
./main.o 

CPP_DEPS += \
./GoogleTrace.d \
./JobEvent.d \
./TaskEvent.d \
./common.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../../includes -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


