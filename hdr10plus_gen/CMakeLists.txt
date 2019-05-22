# CMakeList.txt : hdr10plus_gen の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

include_directories(../x265/source/dynamicHDR10)

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (hdr10plus_gen "hdr10plus_gen.cpp")

target_link_libraries(hdr10plus_gen dynamicHDR10)
# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
