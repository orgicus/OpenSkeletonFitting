include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Externals/include)
include_directories(${PROJECT_SOURCE_DIR}/Externals/include/openni)
link_directories(${PROJECT_SOURCE_DIR}/Externals/lib)

IF(APPLE)
	link_directories(/usr/local/lib)
	find_library(OpenGL_LIBRARY OpenGL)
	include_directories(${OPENGL_INCLUDE_DIR})
ENDIF(APPLE)

set(SOURCEFILES
	main.cpp
	Visualization2d.cpp
	Visualization3d.cpp
)

set(HEADERFILES
	Visualization2d.h
	Visualization3d.h
)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(OpenCV COMPONENTS calib3d core highgui imgproc video REQUIRED)

add_executable(OpenSFVis ${SOURCEFILES} ${HEADERFILES})
target_link_libraries(OpenSFVis ${PROJECTLIBS})

#add external libraries
target_link_libraries(OpenSFVis flann GLFW openNI)

IF(APPLE)
	target_link_libraries(OpenSFVis ${OpenGL_LIBRARY})
ENDIF(APPLE)

find_package(CMinpack REQUIRED)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-lcminpack")


target_link_libraries(OpenSFVis
	opencv_calib3d
	opencv_core
	opencv_highgui
	opencv_imgproc
	opencv_video
	boost_thread-mt
	${Boost_FILESYSTEM_LIBRARY}
  	${Boost_SYSTEM_LIBRARY})