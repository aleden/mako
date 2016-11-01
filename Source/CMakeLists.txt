add_subdirectory(zlib)
add_subdirectory(pnglib)
add_subdirectory(jpeglib)

file(GLOB MAKO_H_SRCS *.h)
file(GLOB MAKO_CPP_SRCS *.cpp)

include_directories($ENV{DXSDK_DIR}include
                    $ENV{CG_INC_PATH}
                    ${PHYSX_SDKS_DIR}/Foundation/include
                    ${PHYSX_SDKS_DIR}/NxCharacter/include
                    ${PHYSX_SDKS_DIR}/Physics/include
                    ${PHYSX_SDKS_DIR}/PhysXLoader/include
                    ${PHYSX_SDKS_DIR}/Cooking/include
                    ${FREETYPE2_INCLUDE_DIR}
                    ${JPEGLIB_INCLUDE_DIR}
                    ${PNGLIB_INCLUDE_DIR}
                    ${ZLIB_INCLUDE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../Extras/3dsMaxMakoMeshExporter)

add_library(Mako SHARED
            ${MAKO_CPP_SRCS}
            ${MAKO_H_SRCS})

set(FREETYPE2_LIB optimized ${FREETYPE2_LIB_DIR}/freetype2312MT.lib debug ${FREETYPE2_LIB_DIR}/freetype2312MT_D.lib)

target_link_libraries(Mako
                      zlib
                      pnglib
                      jpeglib
                      $ENV{CG_LIB_PATH}/cg.lib
                      $ENV{CG_LIB_PATH}/cgD3D9.lib
                      $ENV{DXSDK_DIR}lib\\x86\\dxerr.lib
                      $ENV{DXSDK_DIR}lib\\x86\\dxguid.lib
                      $ENV{DXSDK_DIR}lib\\x86\\d3dx9.lib
                      $ENV{DXSDK_DIR}lib\\x86\\d3d9.lib
                      ${FREETYPE2_LIB}
                      ${PHYSX_LIBS_DIR}/NxCharacter.lib
                      ${PHYSX_LIBS_DIR}/NxCooking.lib
                      ${PHYSX_LIBS_DIR}/PhysXLoader.lib)