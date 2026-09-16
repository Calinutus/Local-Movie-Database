cmake_minimum_required(VERSION 3.16)
project(a10_Calinutus_1)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH "D:/Qt/6.11.0/mingw_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

include_directories(Run)

add_executable(a10_Calinutus_1
        Run/admin_gui.cpp
        Run/admin_gui.h
        Run/domain.cpp
        Run/domain.h
        Run/main.cpp
        Run/movie_validator.cpp
        Run/movie_validator.h
        Run/movie_validator_exception.h
        Run/repository.cpp
        Run/repository.h
        Run/repository_exception.h
        Run/service.cpp
        Run/service.h
        Run/startwindow.cpp
        Run/startwindow.h
        Run/tests.cpp
        Run/tests.h
        Run/user_gui.cpp
        Run/user_gui.h
        Run/watchlist.cpp
        Run/watchlist.h
        Run/undo.h
        Run/watchlistmodel.h
        Run/watchlistwindow.h
)

target_link_libraries(a10_Calinutus_1 Qt6::Widgets)
