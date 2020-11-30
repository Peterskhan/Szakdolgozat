idf_component_register(
	SRCS "i2c/i2c_master.cpp" "sd_spi/sd_spi.cpp"
    INCLUDE_DIRS "." "uart" "i2c" "spi" "sd_spi"
    REQUIRES fatfs
)