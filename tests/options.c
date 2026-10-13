#include "common/option.h"
#include "options/display.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32
#include <sys/wait.h>
#include <unistd.h>
#endif
#define VERIFY(value) do { if (!(value)) { fprintf(stderr, "Options regression failed at %d\n", __LINE__); exit(1); } } while (0)
#ifndef _WIN32
static bool rejectsNumber(bool signedValue, const char* value) {
    pid_t child = fork();
    VERIFY(child >= 0);
    if (child == 0) {
        if (signedValue) { (void) ffOptionParseInt32("--test", value); }
        else { (void) ffOptionParseUInt32("--test", value); }
        _exit(0);
    }
    int status;
    VERIFY(waitpid(child, &status, 0) == child);
    return WIFEXITED(status) && WEXITSTATUS(status) != 0;
}
#endif
int main(void) {
#ifndef _WIN32
    VERIFY(rejectsNumber(false, ""));
    VERIFY(rejectsNumber(true, ""));
#endif
    return 0;
}
