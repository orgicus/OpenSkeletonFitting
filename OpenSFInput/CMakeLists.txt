include_directories(${CMAKE_CURRENT_BINARY_DIR})
# include_directories(${PROJECT_SOURCE_DIR}/Externals/include)
# include_directories(${PROJECT_SOURCE_DIR}/Externals/include/openni)

find_library(OpenNI REQUIRED)
# sorta hacky
include_directories(${OpenNI}/../include/ni)

set(SOURCEFILES
	Input.cpp
	InputKinect.cpp
	InputNumbered.cpp
	InputPlayerONI.cpp
)

set(HEADERFILES
	Input.h
	InputFactory.h
	InputKinect.h
	InputNumbered.h
	InputPlayerONI.h
)

add_library(OpenSFInput ${SOURCEFILES} ${HEADERFILES})