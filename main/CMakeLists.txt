# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES driver_interface bme280 ssd1306 http cpp_json thingspeak wifi dataflow app_components)

set(COMPONENT_SRCS "main.cpp")
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
