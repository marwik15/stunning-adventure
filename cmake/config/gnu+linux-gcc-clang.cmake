if (UNIX) #create aplication
    set(CMAKE_CXX_FLAGS "-no-pie -s -pthread")
endif (UNIX)