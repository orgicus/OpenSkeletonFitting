include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SOURCEFILES
	FeaturePoint.cpp
	Features.cpp
	FeatureTracking.cpp
	GeodesicDistMap.cpp
)

set(HEADERFILES
	FeaturePoint.h
	Features.h
	FeaturesFactory.h
	FeatureTracking.h
	GeodesicDistMap.h
)

add_library(OpenSFFeatures ${SOURCEFILES} ${HEADERFILES})