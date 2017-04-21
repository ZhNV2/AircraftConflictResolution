cmake_minimum_required(VERSION 3.7)
project(ACR)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(SOURCE_FILES main.cpp DomainData/Aircraft/aircraft.cpp DomainData/Aircraft/aircraft.h Geometry/Point/point.cpp Geometry/Point/point.h Geometry/Track/track.cpp Geometry/Track/track.h DomainData/DomainObject.h DomainData/DomainObjectsCollection/domain_objects_collection.cpp DomainData/DomainObjectsCollection/domain_objects_collection.h Runner/runner.cpp Runner/runner.h Geometry/Vector/vector.cpp Geometry/Vector/vector.h DomainData/ConflictManager/conflict_manager.cpp DomainData/ConflictManager/conflict_manager.h)
add_executable(ACR ${SOURCE_FILES})