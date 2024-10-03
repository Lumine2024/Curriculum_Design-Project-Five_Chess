cmake_minimum_required(VERSION 3.20)
project (FiveChess VERSION 0.2)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)
add_executable (FiveChess main.cpp Search_AI.cpp)
target_link_libraries (FiveChess winmm)
target_include_directories (FiveChess PUBLIC ${PROJECT_SOURCE_DIR})
include_directories("C:/Program Files (x86)/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.29.30133/include")
link_directories("C:/Program Files (x86)/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.29.30133/lib/x64")
add_custom_command(TARGET FiveChess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/FiveChess.mp3
    $<TARGET_FILE_DIR:FiveChess>
)
