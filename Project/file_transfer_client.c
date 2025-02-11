/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "file_transfer.h"
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define CHUNK 1000

char serverFile[255];
char clientFile[255];
int flag = 0;    // 0 - false, 1 - true
char* hosts[10]; // more than this is impossible

typedef struct Host {
    char host[255];
    struct Host* next;
} Host;

void list(char* string)
{
    char file_list_cpy[strlen(string)];
    strcpy(file_list_cpy, string);

    char* token = strtok(file_list_cpy, " ");
    if (token != NULL)
        printf("%s\n", token);
    while ((token = strtok(NULL, " ")) != NULL)
        printf("%s\n", token);
}

bool_t hasFile(char* string, char* fileName)
{
    char file_list_cpy[strlen(string) + 1];
    strcpy(file_list_cpy, string);

    char* token = strtok(file_list_cpy, " ");
    if (token != NULL && strcmp(token, fileName) == 0)
        return TRUE;
    while ((token = strtok(NULL, " ")) != NULL)
        if (strcmp(token, fileName) == 0)
            return TRUE;
    return FALSE;
}

void upload(CLIENT* clnt)
{
    FileName serverFileCpy;
    Content serverContent;
    int* serverFd;
    int chunkSize;
    bool_t* isAppend;
    bool_t* isClosed;

    /* open client side file */
    int cfd = open(clientFile, O_RDWR);
    printf("Connected to client file %s in fd(%d)\n", clientFile, cfd);

    /* create file */
    memcpy(serverFileCpy, serverFile, 255);
    serverFd = setfilename_0(serverFileCpy, clnt);
    printf("Connected to server file %s in fd(%d)\n", serverFile, *serverFd);
    if (serverFd == (int*)NULL) {
        clnt_perror(clnt, "call failed: could not open server file");
        exit(1);
    }
    serverContent.fd = *serverFd;

    /* append data to file */
    while ((chunkSize = read(cfd, serverContent.array, CHUNK)) > 0) {
        serverContent.size = chunkSize;
        isAppend = appendcontent_0(&serverContent, clnt);
        if (isAppend == (bool_t*)NULL) {
            clnt_perror(clnt, "call failed: could not append data");
        }
    }

    /* close server side file */
    isClosed = closefile_0(serverFd, clnt);
    if (isClosed == (bool_t*)NULL) {
        clnt_perror(clnt, "call failed: could not close server file");
    }

    /* close client side file */
    close(cfd);
}

void download(CLIENT* clnt)
{
    FileName serverFileCpy;
    Content* serverContent;
    int* serverFd;
    bool_t* isClosed;

    /* create client side file */
    memcpy(serverFileCpy, serverFile, 255);
    int cfd = open(clientFile, O_RDWR | O_CREAT, 0666);

    /* open file */
    serverFd = setfilename_0(serverFileCpy, clnt);
    if (serverFd == (int*)NULL) {
        clnt_perror(clnt, "call failed: could not open server file");
        exit(1);
    }

    /* read data from file */
    while (TRUE) {
        /* read data from server */
        serverContent = readcontent_0(serverFd, clnt);
        if (serverContent == (Content*)NULL) {
            clnt_perror(clnt, "call failed: could not append data");
        }

        /* push data to file */
        if (serverContent->size > 0) {
            write(cfd, serverContent->array, serverContent->size);
        } else {
            break;
        }
    }

    /* close server side file */
    isClosed = closefile_0(serverFd, clnt);
    if (isClosed == (bool_t*)NULL) {
        clnt_perror(clnt, "call failed: could not close server file");
    }

    /* close client side file */
    close(cfd);
}

Content* getList(CLIENT* clnt)
{
    Content* fileList;
    char* voidArg;
    fileList = listfile_0((void*)&voidArg, clnt);
    if (fileList == (Content*)NULL) {
        clnt_perror(clnt, "call failed");
    }
    return fileList;
}

void process(char* host, int mode)
{
    CLIENT* clnt;

#ifndef DEBUG
    clnt = clnt_create(host, FILE_TRANSFER, FILE_TRANSFER_VER, "udp");
    if (clnt == NULL) {
        printf("Could not connect to server (%s)\n", host);
        clnt_pcreateerror(host);
        exit(1);
    }
#endif /* DEBUG */

    switch (mode) {
    case 0: // list file in server
        list(getList(clnt)->array);
        flag = 1;
        break;
    case 1: // check file
        if (hasFile(getList(clnt)->array, serverFile))
            flag = 1;
        else
            flag = 0;
        break;
    case 2: // download file from server
        printf("Start download from %s to %s\n", serverFile, clientFile);
        download(clnt);
        flag = 1;
        break;
    case 3: // upload file to server
        printf("Start upload from %s to %s\n", clientFile, serverFile);
        upload(clnt);
        flag = 1;
        break;
    default:
        printf("Error mode!\n");
    }

#ifndef DEBUG
    clnt_destroy(clnt);
#endif /* DEBUG */
}

void loadFile()
{
    printf("Warning: pas d'espace dans le nom du fichier\n");
    printf("client file: ");
    scanf("%s", clientFile);
    printf("server file: ");
    scanf("%s", serverFile);
}

void loadHost(char* hostFile, Host* host)
{
    int fd = open(hostFile, O_RDONLY);

    /* verify host file */
    if (fd < 0)
        printf("Could not load host from file %s", hostFile);

    char hostString[1000];
    memset(hostString, 0, sizeof(hostString));
    int size = read(fd, hostString, 1000); // don't do like this, but I don't care

    /* load first host */
    char* token = strtok(hostString, "\n");
    memcpy(host->host, token, strlen(token) + 1);
    host->next = NULL;
    printf("load host: (%s)\n", host->host);

    /* load next host */
    while ((token = strtok(NULL, "\n")) != NULL) {
        if (strlen(token) > 1) {
            Host* next = (Host*)malloc(sizeof(Host));
            memcpy(next->host, token, strlen(token));
            printf("load host: (%s)\n", next->host);
            next->next = NULL;
            host->next = next;
            host = next;
        }
    }
}

int main(int argc, char* argv[])
{
    Host head;

    if (argc < 3) {
        printf("usage: %s <hosts_file> <OPTION>\n", argv[0]);
        printf(" OPTION:\n");
        printf("    list: list files\n");
        printf("    download: copy server file to client file\n");
        printf("    upload: copy client file to server file\n");
        exit(1);
    }
    loadHost(argv[1], &head);
    Host* ptr = &head;
    if (strcmp(argv[2], "list") == 0) {
        printf("list of file:\n");
        while (ptr != NULL) {
            flag = 0;
            process(ptr->host, 0);
            if (flag != 1)
                printf("Ambiguous error while process host %s\n", ptr->host);
            ptr = ptr->next;
        }
    } else if (strcmp(argv[2], "download") == 0) {
        loadFile();
        while (ptr != NULL) {
            process(ptr->host, 1); // find file in server
            if (flag == 1) {       // file exited in server
                process(ptr->host, 2);
                if (flag != 1)
                    printf("Ambiguous error while process host %s\n", ptr->host);
                break; // file is unique in all hosts
            }
            ptr = ptr->next;
        }
    } else if (strcmp(argv[2], "upload") == 0) {
        int countFile = 0;
        int numHost = 0;
        loadFile();

        /* check exited file */
        while (ptr != NULL) {
            process(ptr->host, 1); // find file in server
            countFile += flag;
            numHost++;
            ptr = ptr->next;
        }

        /* upload file */
        ptr = &head;
        if (countFile > 0) {
            printf("File %s exited in distributed FS\n", serverFile);
        } else {
            srand(time(NULL));
            int num = rand() % numHost;
            for (int i = 0; i <= num; i++)
                if (ptr->next != NULL)
                    ptr = ptr->next;
            printf("Upload to server: %s\n", ptr->host);
            flag = 0;
            process(ptr->host, 3);
            if (flag != 1)
                printf("Ambiguous error while process host %s\n", ptr->host);
        }
    } else {
        printf("Could not recognize option: %s\n", argv[2]);
    }
    return 0;
}
