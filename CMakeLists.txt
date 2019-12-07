cmake_minimum_required(VERSION 3.0)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(LOLIconFastSwitch)
include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(A_NAME "${PROJECT_NAME}")
set(A_SELF eboot.bin)

set(K_NAME "${PROJECT_NAME}_kernel")
set(K_SELF "${K_NAME}.skprx")

set(U_NAME "${PROJECT_NAME}_user")
set(U_SELF "${U_NAME}.suprx")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -std=c99")
set(VITA_MAKE_FSELF_FLAGS "${VITA_MAKE_FSELF_FLAGS} -a 0x2800000000000001")
link_directories(
  "${CMAKE_BINARY_DIR}/${K_NAME}_stubs"
  "${CMAKE_BINARY_DIR}/${U_NAME}_stubs"
)

# app

set(VITA_APP_NAME "LOLIcon Fast Switch")
set(VITA_TITLEID "AKRK00001")
set(VITA_VERSION "01.00")

add_executable("${A_NAME}"
  main.c
)

target_link_libraries("${A_NAME}"
  SceLibKernel_stub
  SceVshBridge_stub
  taihen_stub
  LOLIconFastSwitchUser_stub_weak
)

add_dependencies("${A_NAME}" "${U_NAME}_stubs")
vita_create_self("${A_SELF}" "${A_NAME}" UNSAFE)

# kernel plugin

add_executable("${K_NAME}"
  main_kernel.c
)

target_compile_options("${K_NAME}" PRIVATE -std=gnu99)

target_link_libraries("${K_NAME}"
  SceCpuForDriver_stub
  SceModulemgrForDriver_stub
  "-nostdlib"
)

vita_create_self("${K_SELF}" "${K_NAME}" CONFIG exports_kernel.yml UNSAFE)
vita_create_stubs("${K_NAME}_stubs" "${K_SELF}" exports_kernel.yml KERNEL)

# user plugin

add_executable("${U_NAME}"
  main_user.c
)

target_link_libraries("${U_NAME}"
  LOLIconFastSwitchKernel_stub
  "-nostdlib"
)

add_dependencies("${U_NAME}" "${K_NAME}_stubs")
vita_create_self("${U_SELF}" "${U_NAME}" CONFIG exports_user.yml UNSAFE)
vita_create_stubs("${U_NAME}_stubs" "${U_SELF}" exports_user.yml)

# vpk

vita_create_vpk("${A_NAME}.vpk" "${VITA_TITLEID}" "${A_SELF}"
  VERSION "${VITA_VERSION}"
  NAME "${VITA_APP_NAME}"
  FILE "${CMAKE_BINARY_DIR}/${K_SELF}" "${K_SELF}"
  FILE "${CMAKE_BINARY_DIR}/${U_SELF}" "${U_SELF}"
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE sce_sys/livearea/contents/bg0.png sce_sys/livearea/contents/bg0.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
)
