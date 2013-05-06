FIND_PACKAGE(Threads)
SET(SRC socket.c mchat.c msg.c user.c ../lib/wfun.c ../lib/debug.c
    ../lib/hash.c ../lib/list.c)
ADD_EXECUTABLE(mchat ${SRC})
INCLUDE_DIRECTORIES(../lib)
TARGET_LINK_LIBRARIES(mchat libmchat ${CMAKE_THREAD_LIBS_INIT})
