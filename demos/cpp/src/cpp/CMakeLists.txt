file(GLOB HEllOLIB_SRC_POOL ${PROJECT_SOURCE_DIR}/src/cpp/*.cpp)

# create a  library from the collected source files
add_library(NDSL SHARED ${HEllOLIB_SRC_POOL})
target_include_directories(NDSL PUBLIC ${INCLUDE_DIR})