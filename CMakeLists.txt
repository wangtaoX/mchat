CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#project name
PROJECT(mchat)

SET(CMAKE_C_FLAGS "-g -Wall")
SET(SUB_DIR src lib)
SET(CURSES_NEED_NCURSES TRUE)
FIND_PACKAGE(Curses REQUIRED)
MESSAGE(${CURSES_LIBRARY})

foreach(f ${SUB_DIR})
  ADD_SUBDIRECTORY(${f})
endforeach(f)
