################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
OrbitOLED/OrbitOLEDInterface.obj: ../OrbitOLED/OrbitOLEDInterface.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/harry/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path="/home/harry/Documents/University 2019/ENCE361/Project/Milestone2/thu_pm_group-23/milestone2" --include_path="/home/harry/ti/TivaWare" --include_path="/home/harry/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="OrbitOLED/OrbitOLEDInterface.d_raw" --obj_directory="OrbitOLED" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

