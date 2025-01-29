#define _GNU_SOURCE

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>

#include <sys/syscall.h>
#include <linux/memfd.h>

#define SRC_CODE_LEN 1 << 16
#define DEFULT_PATH_LEN 1 << 14
#define DEFULT_USER_PATH_LEN 1 << 10

#include "includes/smuclib.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "[ ERR ] expected only one argument with extension .smuc");
        return EXIT_FAILURE;
    }

    const char *srcCodeFP = argv[1];
    const char *dot = strrchr(srcCodeFP, '.');
    if (!dot || strcmp(dot, ".smuc") != 0) {
        fprintf(stderr, "[ ERR ] input file must have .smuc extension\n");
        return EXIT_FAILURE;
    }

    int srcCodeFD = open(srcCodeFP, O_RDONLY);
    if (-1 == srcCodeFD) {
        fprintf(stderr, "[ INTERNAL ERR ] open src code file failed");
        return EXIT_FAILURE;
    }

    char srcCodeBuffer[SRC_CODE_LEN];
    int srcCodeBytesCnt = read(srcCodeFD, srcCodeBuffer, SRC_CODE_LEN);
    if (-1 == srcCodeBytesCnt) {
        fprintf(stderr, "[ INTERNAL ERR ] read of src code file failed");
        return EXIT_FAILURE;
    }

    int memfd = syscall(SYS_memfd_create, "f.smuc", 0);
    if (-1 == memfd) {
        fprintf(stderr, "[ INTERNAL ERR ] memfd_create syscall failed");
        return EXIT_FAILURE;
    }

    if (-1 == write(memfd, stdlibCode, strlen(stdlibCode))) {
        fprintf(stderr, "[ INTERNAL ERR ] write(stdlibCode) syscall failed");
        return EXIT_FAILURE;
    }
    if (-1 == write(memfd, srcCodeBuffer, srcCodeBytesCnt)) {
        fprintf(stderr, "[ INTERNAL ERR ] write(srcCodeBuffer) syscall failed");
        return EXIT_FAILURE;
    }

    if (-1 == lseek(memfd, 0, SEEK_SET)) {
        fprintf(stderr, "[ INTERNAL ERR ] lseek syscall failed");
        return EXIT_FAILURE;   
    }

    char userPath[DEFULT_USER_PATH_LEN];

    if (!getcwd(userPath, DEFULT_USER_PATH_LEN)) {
        perror("[ INTERNAL ERR ] getcwd failed");
        return EXIT_FAILURE;
    }

    char outName[DEFULT_PATH_LEN];
    memset(outName, 0, sizeof(outName));

    size_t baseLen = (size_t)(dot - srcCodeFP);
    if (baseLen >= sizeof(outName)) {
        fprintf(stderr, "[ ERR ] input file name too long\n");
        return EXIT_FAILURE;
    }
    strncpy(outName, srcCodeFP, baseLen);
    outName[baseLen] = '\0';

    char resPath[DEFULT_PATH_LEN];
    snprintf(resPath, DEFULT_PATH_LEN, "%s/%s", userPath, outName);

    pid_t childPID = fork();
    if (0 == childPID) {
        char memf_path[DEFULT_PATH_LEN];
        snprintf(memf_path, sizeof(memf_path), "/proc/self/fd/%d", memfd);
        execlp("fasm", "fasm", memf_path, resPath, NULL);
        fprintf(stderr, "[ INTERNAL ERR ] execlp function call failed");
        _exit(1);
    } else {
        int child_state;
        waitpid(childPID, &child_state, 0);
        if (WIFEXITED(child_state) && WEXITSTATUS(child_state) == EXIT_SUCCESS) {
            fprintf(stdout, "[ OK ] Compiled successfully!\n");
        } else {
            fprintf(stdout, "[ ERR ] Compilation has occurred!\n");
        }
        close(memfd);
    }

    return EXIT_SUCCESS;
}
