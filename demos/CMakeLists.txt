cmake_minimum_required(VERSION 3.8)  # 不要修改
project(CodeCraft)                   # 不要修改

set(CMAKE_CXX_STANDARD      17)      # 不要修改
set(CMAKE_C_STANDARD        11)      # 不要修改
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/) # 不要修改
option(BUILD_SHARED_LIBS    ""  OFF) # 不要修改

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)   # 自己添加的
aux_source_directory(./      cur_src) # 不要修改

# 如果需要，可以使用aux_source_directory增加目录

add_subdirectory(./src) # 自己添加的

add_executable(code_craft ${cur_src}) # ！！！不要修改 code_craft 名称，直接影响结果；可以根据语法在 ${cur_src} 后面追加
target_include_directories(code_craft PUBLIC ${INCLUDE_DIR}) # 自己添加的
# 以下可以根据需要增加需要链接的库
#if (NOT WIN32)
#    target_link_libraries(code_craft  pthread  rt  m)
#endif (NOT WIN32)

# if (NOT WIN32)
target_link_libraries(code_craft  PUBLIC NDSL)
# endif (NOT WIN32)
