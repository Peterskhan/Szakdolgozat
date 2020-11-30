idf_component_register(
	SRCS "bme280/df_bme280.cpp" "debounce/df_debounce.cpp" "debug/df_debug.cpp" "function/df_function.cpp"
	     "interfaces/df_i2c_master.cpp" "interfaces/df_sdspi.cpp" "thingspeak/df_thingspeak_read.cpp"
	     "thingspeak/df_thingspeak_write.cpp" "timers/df_watchdog.cpp" "wifi/df_wifi.cpp" "gpio/df_gpio.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES dataflow bme280 driver_interface thingspeak wifi
)