cmake_minimum_required(VERSION 3.2)
project(AVIMediaPlayer)

add_rpx(mediap  src/main.c
                src/screen.c
                src/app.c
                src/tmp_hacks.c)
target_link_libraries(mediap
    whb
    coreinit
    nsysnet
    proc_ui
    gx2)
set_property(TARGET mediap PROPERTY C_STANDARD 99)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
