file(GLOB JPEGLIB_H_SRCS *.h)
file(GLOB JPEGLIB_C_SRCS *.c)

add_library(jpeglib
            ${JPEGLIB_C_SRCS}
            ${JPEGLIB_H_SRCS})