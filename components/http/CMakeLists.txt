idf_component_register(
	SRCS "http_client.cpp" "http_request.cpp" "http_response.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_http_client
)