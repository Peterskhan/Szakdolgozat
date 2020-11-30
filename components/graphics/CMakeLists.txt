idf_component_register(
	SRCS     "graphics.cpp" "fonts/font6x6/font6x6.cpp" "fonts/font8x8/font8x8.cpp"
	INCLUDE_DIRS "." "fonts/font6x6" "fonts/font8x8"
    REQUIRES driver_interface
)