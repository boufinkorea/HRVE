cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(SET CMP0015 NEW)
project(HiREV_Eye)
find_package(OpenCV REQUIRED)

MESSAGE(${CMAKE_SOURCE_DIR}/lib)

include_directories(include ${CMAKE_SOURCE_DIR}/ITT_Camera ${CMAKE_SOURCE_DIR}/include/bobcat_gev ${CMAKE_SOURCE_DIR}/include/kai ${CMAKE_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS} )
link_directories(lib/bobcat_gev/ lib/kai/ /opt/imperx/bobcat_gev/lib)
add_definitions( -D_UNIX_ -D_LINUX_ -DLINUX -DPV_GUI_NOT_AVAILABLE -DFILE_STORAGE_PATH="FileStorage" -D_GLIBCXX_USE_CXX11_ABI=0)

add_executable (HiREV_Eye src/HiREV_Eye.cpp ITT_Camera/ITTCamera.cpp)

target_link_libraries(HiREV_Eye PvBase PvDevice PvBuffer PvGenICam PvStream PvTransmitter PvVirtualDevice PvAppUtils kai2 ${OpenCV_LIBS} )
set_target_properties(HiREV_Eye PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")

#target_link_libraries(PayloadCam libEbTransportLayerLib libEbUtilsLib libPtConvertersLib libPtUtilsLib libPvAppUtils libPvBase libPvBuffer libPvDevice libPvGUI libPvGenICam libPvPersistence libPvSerial libPvStream libPvTransmitter libPvVirtualDevice libSimpleImagingLib libipxtruesense)
