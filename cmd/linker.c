#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/types.h>

#include <sys/syscall.h>
#include <linux/memfd.h>

#define DEFULT_PATH_LEN 128

const char *stdlibCode = 
    "format ELF64 executable\n"
    "  ; ...ваша stdlib...\n";

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "expectred one argument (file.smuc)");
        return EXIT_FAILURE;
    }

    const char *srcCodeFP = argv[1];

    int memfd = syscall(SYS_memfd_create, "smuclib", 0);

    if (memfd == -1) {
        fprintf(stderr, "memfd_create syscall failed");
        return EXIT_FAILURE;
    }

    write(memfd, stdlibCode, strlen(stdlibCode));
    write(memfd, srcCodeFP, strlen(srcCodeFP));

    lseek(memfd, 0, SEEK_SET);

    pid_t pid = fork();
    if (pid == 0) {
        char memf_path[DEFULT_PATH_LEN];
        snprintf(memf_path, sizeof(memf_path), "/proc/self/fd/%d", memfd);
        execlp("fasm", "fasm", memf_path, NULL);
        fprintf(stderr, "execlp function call failed");
        _exit(1);
    } else {
        int child_state;
        waitpid(pid, &child_state, 0);
        if (WIFEXITED(child_state) && WEXITSTATUS(child_state) == EXIT_SUCCESS) {
            fprintf(stdout, "Compiled successfully!\n");
        } else {
            fprintf(stdout, "Compilation has occurred!\n");
        }
        close(memfd);
    }

    return EXIT_SUCCESS;
}
