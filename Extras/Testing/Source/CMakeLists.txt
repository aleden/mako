include_directories(${MAKO_INCLUDE_DIR})

file(GLOB TESTING_CPP_SRCS *.cpp)
file(GLOB TESTING_H_SRCS *.h)

add_executable(Testing
               ${TESTING_CPP_SRCS}
               ${TESTING_H_SRCS})
               
target_link_libraries(Testing Mako)