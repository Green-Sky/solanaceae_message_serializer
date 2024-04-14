cmake_minimum_required(VERSION 3.9...3.24 FATAL_ERROR)

project(solanaceae)

add_library(solanaceae_message_serializer
	./solanaceae/message3/message_serializer.hpp
	./solanaceae/message3/message_serializer.cpp

	# TODO: extract?
	./solanaceae/message3/nj/message_components.hpp
	./solanaceae/message3/nj/message_components_serializer.hpp
	./solanaceae/message3/nj/message_components_serializer.cpp
)

target_include_directories(solanaceae_message_serializer PUBLIC .)
target_compile_features(solanaceae_message_serializer PUBLIC cxx_std_17)
target_link_libraries(solanaceae_message_serializer PUBLIC
	solanaceae_util
	solanaceae_message3
	nlohmann_json::nlohmann_json
)

########################################

