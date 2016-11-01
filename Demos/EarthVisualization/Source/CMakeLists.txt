include_directories(${MAKO_INCLUDE_DIR})

file(GLOB EARTHVIS_CPP_SRCS *.cpp)
file(GLOB EARTHVIS_H_SRCS *.h)

add_executable(EarthVisualization
               ${EARTHVIS_CPP_SRCS}
               ${EARTHVIS_H_SRCS})

target_link_libraries(EarthVisualization Mako)