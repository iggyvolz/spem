cmake_minimum_required(VERSION 3.25)
project(spem)

set(CMAKE_CXX_STANDARD 23)

set(PHP_ROOT CACHE PATH "Path to PHP installation directory")

include_directories(
        ${PHP_ROOT}/include/php
        ${PHP_ROOT}/include/php/main
        ${PHP_ROOT}/include/php/TSRM
        ${PHP_ROOT}/include/php/Zend
        ${PHP_ROOT}/include/php/ext
        ${PHP_ROOT}/include/php/ext/date/lib
        ${PHP_ROOT}/include
        ${PHP_ROOT}/include/main
        ${PHP_ROOT}/include/TSRM
        ${PHP_ROOT}/include/Zend
        ${PHP_ROOT}/include/ext
        ${PHP_ROOT}/include/ext/date/lib
)
if (WIN32)
    link_directories(${PHP_ROOT}/lib)
endif ()
add_library(spem SHARED main.cpp)
if(WIN32)
    target_compile_definitions(spem PRIVATE ZEND_WIN32 PHP_WIN32)
    target_link_libraries(spem PRIVATE php8)
endif()
