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
