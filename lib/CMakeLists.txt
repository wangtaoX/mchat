SET(CMAKE_C_FLAGS "-g -Wall")
SET(src hash.c list.c wfun.c debug.c string.c)
ADD_LIBRARY(libmchat ${src})
