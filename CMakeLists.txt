cmake_minimum_required(VERSION 3.8)
project(Ntolen)

set(CMAKE_CXX_STANDARD 17)
set(SDL3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3/build)
# set(SDL3_image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL3_image/build)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

set(LSOURCE_FILES)

file(GLOB LSOURCE_FILES src/*.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(MSC_DEFS ${MSC_DEFS} DEBUG=1 Debug=1 MSC_DEBUG_TRACE_INSTRUCTIONS=0)
endif()

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

LIST(APPEND INCLUDE_DIRS
        src/
        lib/mosc/api
        lib/mosc/src/builtin
        lib/mosc/src/common
        lib/mosc/src/compiler
        lib/mosc/src/helpers
        lib/mosc/src/memory
        lib/mosc/src/meta
        lib/mosc/src/runtime
        include/SDL3
        )



set(SOURCE_FILES)
set(INCLUDE_PATH)

file(GLOB SOURCE_FILES_C lib/mosc/src/**/*.c)
file(GLOB SOURCE_FILES_H lib/mosc/src/**/*.h)
file(GLOB SOURCE_FILES_INC lib/mosc/src/**/*.inc)

LIST(APPEND MSC_SOURCE_FILES ${SOURCE_FILES_C} ${SOURCE_FILES_H} ${SOURCE_FILES_INC})

file(GLOB SOURCE_FILES src/*.cpp src/*.h src/*.hpp src/**/*.cpp src/**/*.h src/**/*.hpp src/**/*.msc src/**/**/*.cpp src/**/**/*.h src/**/**/*.hpp src/**/**/*.msc)
file(GLOB INCLUDE_PATH src/*.h)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
# find_package(SDL3_image REQUIRED CONFIG REQUIRED COMPONENTS SDL3_image)

add_executable(ntolen ${SOURCE_FILES} ${MSC_SOURCE_FILES})
TARGET_INCLUDE_DIRECTORIES(ntolen PUBLIC ${INCLUDE_PATH} ${INCLUDE_DIRS})
TARGET_COMPILE_DEFINITIONS(ntolen PRIVATE ${MSC_DEFS})

target_link_libraries(ntolen
SDL3::SDL3
# SDL3_image::SDL3_image
# SDL3_ttf::SDL3_ttf
        # ${CMAKE_CURRENT_SOURCE_DIR}/lib/libfmod.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL3_image.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libSDL3_ttf.dylib
)
