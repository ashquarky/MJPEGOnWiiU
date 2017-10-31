cmake_minimum_required(VERSION 3.2)
project(AVIMediaPlayer)

add_rpx(mediap  src/main.c
                src/screen.c
                src/app.c
                src/monitor.c
                src/decoder/avi-mjpeg.c
                ext/src/dmae/dmae.c
                src/tmp_hacks.c)
include_directories(${CMAKE_SOURCE_DIR}/ext/src)
target_link_libraries(mediap
    whb
    coreinit
    nsysnet
    proc_ui
    gx2)

include_directories(${CMAKE_SOURCE_DIR}/ext/include)
target_link_libraries(mediap
    ${CMAKE_SOURCE_DIR}/ext/lib/libturbojpeg.a)

include_directories(${CMAKE_SOURCE_DIR}/src)
set_property(TARGET mediap PROPERTY C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
