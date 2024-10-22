cmake_minimum_required(VERSION 3.10)

project(IntegralTests)

enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(TEST_SOURCES
    test_integral.c
    ../src/integral.c
)


#--------------------------------------------------------------------
#                            MyTests
#--------------------------------------------------------------------
add_executable(MyTests ${TEST_SOURCES})

target_include_directories(MyTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(MyTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    OUTPUT_NAME test
    DEBUG_POSTFIX _debug
)
#--------------------------------------------------------------------


#--------------------------------------------------------------------
#                            Criterion
#--------------------------------------------------------------------
# find_package(Criterion REQUIRED)
target_link_libraries(MyTests PRIVATE criterion m)
#--------------------------------------------------------------------


#--------------------------------------------------------------------
# Функция для установки опций компилятора для каждой конфигурации
#--------------------------------------------------------------------
function(add_sanitizer_test target sanitizer_name san_flags)
    add_executable(${target}_${sanitizer_name} ${TEST_SOURCES})

    target_compile_options(${target}_${sanitizer_name} PRIVATE ${san_flags} ${MY_DEBUG_FLAGS})
    target_link_options(${target}_${sanitizer_name} PRIVATE ${san_flags} ${MY_DEBUG_FLAGS})

    target_link_libraries(${target}_${sanitizer_name} criterion m)

    target_include_directories(${target}_${sanitizer_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )

    set_target_properties(${target}_${sanitizer_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        OUTPUT_NAME ${target}_${sanitizer_name}
        DEBUG_POSTFIX _debug
    )

    add_test(NAME ${target}_${sanitizer_name} COMMAND ${target}_${sanitizer_name})
endfunction()
#--------------------------------------------------------------------


#--------------------------------------------------------------------
#             Указываем настройки компиляции для Debug
#--------------------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/debug_config.cmake)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(MyTests PRIVATE ${MY_DEBUG_FLAGS})
    target_link_options   (MyTests PRIVATE ${MY_DEBUG_FLAGS})
    add_sanitizer_test(MyTests asan ${ASAN_FLAG})
    add_sanitizer_test(MyTests tsan ${TSAN_FLAG})
    add_sanitizer_test(MyTests msan ${MSAN_FLAG})
endif()
#--------------------------------------------------------------------
