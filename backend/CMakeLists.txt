cmake_minimum_required(VERSION 3.10)
project("MSBackend")

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DQP_API_VERSION=9999 -O3 -no-pie -pedantic -fno-exceptions -fno-rtti")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")

include_directories(${RAPIDJSON_INCLUDE_DIR})

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GFLAGS_INCLUDE_DIR}
    ${BENCHMARK_INCLUDE_DIR}
    ${RAPIDJSON_INCLUDE_DIR}
)

set(QP_DIR $ENV{QPCPP})
set(QP_PORT ${QP_DIR}/ports/posix)

file(GLOB_RECURSE QP_HEADERS ${QP_DIR}/*.hpp)
set(PREV " ")
foreach(header ${QP_HEADERS})
    if(NOT ${header} MATCHES "/exampl" AND NOT ${header} MATCHES "posix-qv" AND NOT ${header} MATCHES "posix-qutest")
        if(${header} MATCHES "/include" OR ${header} MATCHES ${QP_PORT} OR ${header} MATCHES "/src")
            string(REGEX MATCHALL "/.+/" header ${header})
            if(NOT ${PREV} MATCHES ${header})
                list(APPEND QP_INCLUDES ${header})
                message(${header})
                set(PREV ${header})
            endif()
        endif()
    endif()
endforeach()

file(GLOB_RECURSE QP_SOURCES ${QP_DIR}/*.cpp)
foreach(source ${QP_SOURCES})
    if(NOT ${source} MATCHES "/exampl" AND NOT ${source} MATCHES "qs" AND NOT ${source} MATCHES "posix-qv" AND NOT ${source} MATCHES "posix-qutest" )
        if(${source} MATCHES "/qf/")
            list(APPEND QP_LIBS ${source})
            message(${source})
        elseif(${source} MATCHES ${QP_PORT})
            list(APPEND QP_LIBS ${source})
            message(${source})
        elseif(${source} MATCHES "include")
            list(APPEND QP_LIBS ${source})
            message(${source})
        endif()
    endif()
endforeach()

include_directories(${QP_INCLUDES})
add_library(QP ${QP_LIBS})

include_directories(include)
add_subdirectory(sources)

add_executable(${PROJECT_NAME} backend.cpp)
target_link_libraries(${PROJECT_NAME} QP pthread sources)
add_dependencies(${PROJECT_NAME} rapidjson)