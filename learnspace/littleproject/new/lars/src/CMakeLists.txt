aux_source_directory(. SRC_LIST)

include_directories(../include)

add_executable(main ${SRC_LIST})
TARGET_LINK_LIBRARIES(main pthread)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)