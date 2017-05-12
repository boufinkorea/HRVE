################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ITT_Camera/ITTCamera.cpp 

OBJS += \
./ITT_Camera/ITTCamera.o 

CPP_DEPS += \
./ITT_Camera/ITTCamera.d 


# Each subdirectory must supply rules for building sources it contributes
ITT_Camera/%.o: ../ITT_Camera/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_UNIX_ -DLINUX -DPV_GUI_NOT_AVAILABLE -DFILE_STORAGE_PATH="FileStorage" -D_LINUX_ -I/usr/local/include -I/home/jinhkim/workspace/HiREV_Eye/ITT_Camera -I/home/jinhkim/workspace/HiREV_Eye/include/kai -I/home/jinhkim/workspace/HiREV_Eye/include -I/opt/imperx/bobcat_gev/include -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


