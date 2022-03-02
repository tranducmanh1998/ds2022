# Distributed file system

- Server: at least 2 servers, each have different list of file (should not duplicate)

- Client:

    - list of file
    - upload file to server
    - connect to servers to get file

## Non-functional requirement

1. Easy to OverBufferflow attack (don't care security problem)
2. No warning or usage, no clearly exception handle
3. Simple as its much be simple
4. The user is the program writer (trust everything from user)

## Architecture

1. Communicate through RPC of Sun

- list file: Bytes(len) listFile(void);
- read file: Bytes readFile(int fd);
- write file: void writeFile(int fd, Bytes(len));
- open file: int openFile(stringName);
- close file: void closeFile(int fd);

2. Client decide which file to which server

- get list of file then print
- check new file in list of not
- random select server to upload
- upload file
- download file

3. Server support:

- list file
- read file
- write file

**Warning**: architecture will vary and do not trust it, trust code.

## Setup

Requirement:

1. CMake
2. make
3. gcc, g++
4. Linux
5. libtirpc-dev
6. rpcbind

Install:

```
mkdir build
cd build
cmake ../
```

Build:

```
cd build
make -j
```

Note: for ubuntu

```
make -f Makefile.file_transfer
rm *.o
mv file_transfer_client build/2.rpc_client
mv file_transfer_server build/1.rpc_server
```

Generate clangd setting for vim lsp:

```
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../
cp compile_commands.json ../
```

## Run program

Generate rpcbind

```
sudo rpcbind
```

Run servers

```
cd build
mkdir data
cd data
../1.rpc_server
```

Update server ip to client side

```
cd build
echo "<host>" >> hosts.txt
```

Run client

```
cd build
./2.rpc_client hosts.txt <OPTION>

OPTION:
1. list
2. download
3. upload
```

## Bug

1. Could not recognize file name with space
