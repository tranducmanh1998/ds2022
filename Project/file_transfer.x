/*
 * File transfer program
 */

/*
 * File transfer type declare
 */

typedef char Chunk[1000];
typedef char FileName[255];

struct Content {
    int fd;
    int size;
    Chunk array;
};

/*
 * File transfer program
 */
program FILE_TRANSFER {
    version FILE_TRANSFER_VER {
        /*
         * create if not existed file then open file name.
         * @return {int} file descriptor
         */
        int setFileName(FileName) = 0;

        /*
         * append data to file
         * @return {bool} action status
         */
        bool appendContent(Content) = 1;

        /*
         * read content from file
         * @return {Content} chunk of file with -1 size mean EOF
         */
        Content readContent(int) = 2;

        /*
         * close file opended
         * @param {int} fd for closing
         * @return {bool} action status
         */
        bool closeFile(int) = 3;

        /*
         * list of file is string separated by newline
         * @return {FileList} list of files presented as a string
         */
        Content listFile(void) = 4;
    } = 0;
} = 0;
