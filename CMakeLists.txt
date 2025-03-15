cmake_minimum_required(VERSION 3.10)
project(tgfs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(APPLE)
    message(FATAL_ERROR "Building on OSX is not supported.")
endif()

message(STATUS "glibc version: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -v OUTPUT_VARIABLE COMPILER_VERSION ERROR_VARIABLE COMPILER_VERSION)
string(REGEX REPLACE "[ ]*\n" "; " COMPILER_VERSION "${COMPILER_VERSION}")
message(STATUS "compiler version: ${COMPILER_VERSION}")

set(
    SOURCES
    main.cpp
    fs_entry.cpp
    db_manager.cpp
)

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FUSE REQUIRED fuse3)
    pkg_check_modules(SQLITE3 REQUIRED sqlite3)
    # Добавляем найденные пути из pkg-config
    include_directories(${FUSE_INCLUDE_DIRS})
    link_directories(${FUSE_LIBRARY_DIRS})
    add_compile_options(${FUSE_CFLAGS_OTHER})

    set(BOOST_ROOT "/usr/include/boost")

    find_package(Boost 1.74.0 REQUIRED COMPONENTS system filesystem)
    include_directories(${Boost_INCLUDE_DIRS})
    find_package(OpenSSL REQUIRED)
    link_directories(${OPENSSL_LIBRARIES})

    add_executable(${PROJECT_NAME} ${SOURCES})

    add_subdirectory(./lib/tgbot-cpp)
    add_subdirectory(./lib/SQLiteCpp)
    add_subdirectory(./lib/json)
    include_directories(${CMAKE_SOURCE_DIR}/lib/Fusepp)
    include_directories(${CMAKE_SOURCE_DIR}/lib/json/include)

    target_link_libraries(${PROJECT_NAME} 
            ${FUSE_LIBRARIES}
            ${Boost_LIBRARIES}
            TgBot
            SQLiteCpp
            crypto
            pthread
            sqlite3
    )


endif()
