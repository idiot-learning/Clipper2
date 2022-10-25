add_library(ChaoFileUtils INTERFACE
# ${CMAKE_CURRENT_SOURCE_DIR}/ChaoFileLoad.h 
# ${CMAKE_CURRENT_SOURCE_DIR}/ChaoFileSave.h 

)
messaGE("CMAKE_CURRENT_SOURCE_DIR IS : ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(ChaoFileUtils INTERFACE
#$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(ChaoFileUtils INTERFACE Clipper2 ${Boost_LIBRARIES})