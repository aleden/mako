include_directories(${MAKO_INCLUDE_DIR})

file(GLOB TCP_SERVER_CPP_SRCS *.cpp)
file(GLOB TCP_SERVER_H_SRCS *.h)

add_executable(TCPServer
               ${TCP_SERVER_CPP_SRCS}
               ${TCP_SERVER_H_SRCS})
               
target_link_libraries(TCPServer Mako)