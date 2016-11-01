file(GLOB ZLIB_H_SRCS *.h)
file(GLOB ZLIB_C_SRCS *.c)

add_library(zlib
            ${ZLIB_C_SRCS}
            ${ZLIB_H_SRCS})