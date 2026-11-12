cmake_minimum_required(VERSION 3.20)
project(GerberCommentRegression LANGUAGES CXX)
if(NOT WIN32)
    message(FATAL_ERROR "The existing Gerber library requires the Windows SDK")
endif()
include(CTest)
# Standalone regression target: do not alter the historical Visual Studio projects.
file(GLOB library_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/../*.cpp")
add_executable(gerber_comment_tests comment_eof_tests.cpp ${library_sources})
target_include_directories(gerber_comment_tests PRIVATE "${CMAKE_CURRENT_LIST_DIR}/..")
target_compile_features(gerber_comment_tests PRIVATE cxx_std_11)
# Legacy headers require Windows definitions and std::string before inclusion.
# Keep this test-only compatibility prelude out of the production source files.
target_precompile_headers(gerber_comment_tests PRIVATE <windows.h> <string>)
foreach(case empty text following multiline long_valid missing_empty missing_text missing_long missing_after_valid)
    add_test(NAME comment_${case} COMMAND gerber_comment_tests ${case})
    set_tests_properties(comment_${case} PROPERTIES TIMEOUT 10)
endforeach()
