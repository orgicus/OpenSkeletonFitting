include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_library(OpenNI REQUIRED)
# sorta hacky
include_directories(${OpenNI}/../include/ni)

#only on mac
IF(APPLE)
	include_directories(/usr/local/include/)
	link_directories(/usr/local/lib)
ENDIF(APPLE)


set(SOURCEFILES
	main.cpp
)

set(HEADERFILES
)

add_executable(OpenSFApp ${SOURCEFILES} ${HEADERFILES})
target_link_libraries(OpenSFApp ${PROJECTLIBS})

#add external libraries
target_link_libraries(OpenSFApp flann openNI)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(CMinpack REQUIRED)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-lcminpack")

target_link_libraries(OpenSFApp
	opencv_calib3d
	opencv_core
	opencv_highgui
	opencv_imgproc
	opencv_video
	boost_thread-mt
	${Boost_FILESYSTEM_LIBRARY}
  	${Boost_SYSTEM_LIBRARY}
  	${CMinpack_LIBRARIES}
  	)