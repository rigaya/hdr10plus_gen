# CMakeList.txt : 最上位の CMake プロジェクト ファイル。グローバル構成を行います
# また、サブプロジェクトをここに含めます。
#
cmake_minimum_required (VERSION 2.8.18)

project ("hdr10plus_gen")

if(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

# サブプロジェクトを含めます。
set(ENABLE_HDR10_PLUS 1)
add_subdirectory ("hdr10plus_gen")
add_subdirectory ("x265/source/dynamicHDR10")
