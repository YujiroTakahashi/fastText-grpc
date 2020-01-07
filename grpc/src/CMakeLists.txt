add_library(croco-ftext-shared SHARED 
	${CMAKE_CURRENT_SOURCE_DIR}/client.cc
	${CMAKE_CURRENT_SOURCE_DIR}/service.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ftext.pb.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ftext.grpc.pb.cc)
target_link_libraries(croco-ftext-shared
	pthread
	${PROTOBUF_LIBRARY}
	${grpc-shared}
	${grpcpp-shared}
	${fasttext-shared})
set_target_properties(croco-ftext-shared PROPERTIES OUTPUT_NAME croco_fasttext) 

add_executable(ftext-server-bin ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)
target_link_libraries(ftext-server-bin 
	pthread
	${PROTOBUF_LIBRARY}
	${grpc-shared}
	${grpcpp-shared}
	${fasttext-shared}
	croco-ftext-shared
)
set_target_properties(ftext-server-bin PROPERTIES OUTPUT_NAME fasttext-grpc-server)


add_executable(ftext-client-bin ${CMAKE_CURRENT_SOURCE_DIR}/sample.cc)
target_link_libraries(ftext-client-bin 
	pthread
	${PROTOBUF_LIBRARY}
	${grpc-shared}
	${grpcpp-shared}
	${fasttext-shared}
	croco-ftext-shared
)
set_target_properties(ftext-client-bin PROPERTIES OUTPUT_NAME sample-client)


install (TARGETS croco-ftext-shared LIBRARY DESTINATION lib)
install (TARGETS ftext-server-bin RUNTIME DESTINATION bin)