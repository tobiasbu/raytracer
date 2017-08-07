cmake_minimum_required(VERSION 2.8)

project (Raytracer)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" "INSTALL")
#set(CMAKE_SUPPRESS_REGENERATION true)

if(MSVC)
	#SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GL")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
	SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup /LTCG")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup /LTCG")
	set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/RELEASE /LTCG /OPT:REF /OPT:ICF /INCREMENTAL:NO /GL")
endif()



# COPY DIRECTORY
macro(copy_dir srcDir destDir)
    file(GLOB_RECURSE templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        message(STATUS "Copying file ${srcDir}/${templateFile}") #...
        #message(STATUS "    ... into ${destDir}/${templateFile}")
        configure_file(${srcDir}/${templateFile} ${destDir}/${templateFile} COPYONLY)
    endforeach(templateFile)
endmacro(copy_dir)

# Set paths for source and include files
set(RAYTRACER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(RAYTRACER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Set path for commons libraries path
set(RAYTRACER_COMMONS "${CMAKE_CURRENT_SOURCE_DIR}/commons")

if (NOT WIN32)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
endif (NOT WIN32)

# Add user-defined library path
if (WIN32)
	set(RAYTRACER_COMMONS "${CMAKE_CURRENT_SOURCE_DIR}/commons/windows")
	set(CMAKE_LIBRARY_PATH "${RAYTRACER_COMMONS}")
endif (WIN32)

# Required packages
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

#set(GLEW_INCLUDE_DIR "${CMAKE_LIBRARY_PATH}/glew/include")
#set(GLEW_LIBRARY "${CMAKE_LIBRARY_PATH}/glew/lib")

# Doing this only for glew
set(CMAKE_LIBRARY_PATH "${RAYTRACER_COMMONS}/glew/lib")
set(CMAKE_PREFIX_PATH "${RAYTRACER_COMMONS}/glew/include")

find_package(GLEW REQUIRED)
if(NOT GLEW_FOUND)
    message("ERROR: GLEW not found")
endif(NOT GLEW_FOUND)

set(FREEGLUT_INCLUDE_DIR "${RAYTRACER_COMMONS}/freeglut/include")
set(FREEGLUT_LIBRARIES_DIR "${RAYTRACER_COMMONS}/freeglut/lib")
set(FREEGLUT_LIBRARIES "freeglut")
set(FREEGLUT_LIBRARIES_DEBUG "freeglutd")


# Create filters for every directory in project
file(
    GLOB_RECURSE SOURCE_LIST 
    LIST_DIRECTORIES false
    "${RAYTRACER_SRC}/*.cpp*"
)

file(
    GLOB_RECURSE INCLUDE_LIST 
    LIST_DIRECTORIES false
    "${RAYTRACER_INCLUDE}/*.h*"
	"${RAYTRACER_INCLUDE}/*.hpp*"
)


#add_library(Raytracer ${SOURCE_LIST} ${INCLUDE_LIST})

# Filters for sources
foreach(_source IN ITEMS ${SOURCE_LIST})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${RAYTRACER_SRC}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("Source Files\\${_group_path}" FILES "${_source}")
endforeach()

# Filter for includes
foreach(_inc IN ITEMS ${INCLUDE_LIST})
    get_filename_component(_inc_path "${_inc}" PATH)
    file(RELATIVE_PATH _inc_path_rel "${RAYTRACER_INCLUDE}" "${_inc_path}")
    string(REPLACE "/" "\\" _group_path "${_inc_path_rel}")
    source_group("Include Files\\${_group_path}" FILES "${_inc}")
endforeach()


# Final step, adding all libs and sources together into executable program
include_directories(${RAYTRACER_INCLUDE} ${GLEW_INCLUDE_DIR} ${FREEGLUT_INCLUDE_DIR})
link_directories(${FREEGLUT_LIBRARIES_DIR} ${CMAKE_LIBRARY_PATH}) #${GLEW_INCLUDE_DIRS}
#ink_libraries(${GLEW_LIBRARIES})
add_executable(Raytracer ${SOURCE_LIST} ${INCLUDE_LIST})

target_link_libraries(Raytracer optimized glew32 debug glew32d general ${OPENGL_LIBRARY})

target_link_libraries(Raytracer optimized ${FREEGLUT_LIBRARIES})

target_link_libraries(Raytracer debug ${FREEGLUT_LIBRARIES_DEBUG})



set_target_properties( Raytracer
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET Raytracer POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        "${PROJECT_SOURCE_DIR}/img"
        "$<TARGET_FILE_DIR:Raytracer>/img")
		
add_custom_command(TARGET Raytracer POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        "${PROJECT_SOURCE_DIR}/scene"
        "$<TARGET_FILE_DIR:Raytracer>/scene")

add_custom_command(TARGET Raytracer POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        "${PROJECT_SOURCE_DIR}/obj"
        "$<TARGET_FILE_DIR:Raytracer>/obj")
		
add_custom_command(TARGET Raytracer PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:Raytracer>/output")

if (WIN32)

# Adds a post-build event to Raytracer
add_custom_command(TARGET Raytracer POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_directory  
        "${RAYTRACER_COMMONS}/bin/$<CONFIGURATION>"
        $<TARGET_FILE_DIR:Raytracer>)    
endif (WIN32)





