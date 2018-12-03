cmake_minimum_required(VERSION 2.8)

project(adventofcode2018)


#set(SAVED_BUILD_TESTING ${BUILD_TESTING})
#message("build testing is ${BUILD_TESTING}")
set(BUILD_TESTING OFF)
#message("build testing is ${BUILD_TESTING}")
#set(BUILD_TESTING ${SAVED_BUILD_TESTING})




if (MSVC)
# /wd4005  macro-redefinition
# /wd4068  unknown pragma
# /wd4244  conversion from 'type1' to 'type2'
# /wd4267  conversion from 'size_t' to 'type2'
# /wd4800  force value to bool 'true' or 'false' (performance warning)
add_compile_options(/wd4005 /wd4068 /wd4244 /wd4267 /wd4800)
add_definitions(/DNOMINMAX /DWIN32_LEAN_AND_MEAN=1 /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS)
endif()

#add_subdirectory(${PROJECT_SOURCE_DIR}/lib/cctz)
#add_subdirectory(${PROJECT_SOURCE_DIR}/lib/googletest)
#add_subdirectory(${PROJECT_SOURCE_DIR}/lib/abseil)

add_compile_options("-ggdb")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/out")

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
#add_subdirectory(day4)
#add_subdirectory(day5)
#add_subdirectory(day13)
#add_subdirectory(day14)
