include_directories(${MAKO_INCLUDE_DIR})

file(GLOB TCP_CLIENT_CPP_SRCS *.cpp)
file(GLOB TCP_CLIENT_H_SRCS *.h)

add_executable(TCPClient
               ${TCP_CLIENT_CPP_SRCS}
               ${TCP_CLIENT_H_SRCS})
               
target_link_libraries(TCPClient Mako)