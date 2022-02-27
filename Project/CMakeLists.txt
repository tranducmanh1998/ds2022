project(Project)
cmake_minimum_required(VERSION 3.10)

include_directories("/usr/include/tirpc")
set(CMAKE_C_FLAGS "-ltirpc")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

set(MAIN_SRC
    "file_transfer_xdr.c"
    )

set(CLIENT_SRC
    "file_transfer_client.c"
    "file_transfer_clnt.c"
    )

set(SERVER_SRC
    "file_transfer_server.c"
    "file_transfer_svc.c"
    )

add_executable(2.rpc_client ${MAIN_SRC} ${CLIENT_SRC})
add_executable(1.rpc_server ${MAIN_SRC} ${SERVER_SRC})

