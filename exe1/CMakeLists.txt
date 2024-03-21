add_executable(exe1
        main.c
)

target_link_libraries(exe1 pico_stdlib hardware_adc freertos hardware_adc hardware_pwm)
pico_add_extra_outputs(exe1)