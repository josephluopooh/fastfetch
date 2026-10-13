#include "common/path.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef _WIN32
#include <sys/stat.h>
#include <unistd.h>

static void verify(bool value, int line) {
    if (!value) {
        fprintf(stderr, "Path regression failed at line %d\n", line);
        exit(1);
    }
}

#define VERIFY(value) verify((value), __LINE__)
#endif

int main(void) {
    #ifndef _WIN32
    {
        char directory[] = "/tmp/fastfetch-path-XXXXXX";
        VERIFY(mkdtemp(directory) != nullptr);
        char* previous = getcwd(nullptr, 0);
        VERIFY(previous != nullptr && chdir(directory) == 0);
        FILE* executable = fopen("ff-path-program", "w");
        VERIFY(executable != nullptr);
        fclose(executable);
        VERIFY(chmod("ff-path-program", 0700) == 0);
        const char* paths[] = { "", ":/nonexistent", "/nonexistent:", "/nonexistent::/missing" };
        FF_STRBUF_AUTO_DESTROY result = ffStrbufCreate();
        for (size_t i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i) {
            VERIFY(setenv("PATH", paths[i], 1) == 0);
            VERIFY(ffFindExecutableInPath("ff-path-program", &result) == nullptr);
            VERIFY(ffStrbufEqualS(&result, "./ff-path-program"));
        }
        unlink("ff-path-program");
        VERIFY(chdir(previous) == 0);
        free(previous);
        rmdir(directory);
    }
    #endif
    return 0;
}
