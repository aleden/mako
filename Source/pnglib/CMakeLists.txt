file(GLOB PNGLIB_H_SRCS *.h)
file(GLOB PNGLIB_C_SRCS *.c)

include_directories(${ZLIB_INCLUDE_DIR})

add_library(pnglib
            ${PNGLIB_C_SRCS}
            ${PNGLIB_H_SRCS})
            
target_link_libraries(pnglib zlib)