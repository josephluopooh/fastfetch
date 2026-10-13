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
    {
        const char* invalid[] = { "{\"bar\":{\"width\":256}}", "{\"bar\":{\"width\":-1}}", "{\"bar\":{\"width\":\"10\"}}" };
        for (size_t i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i) {
            FF_STRBUF_AUTO_DESTROY json = ffStrbufCreateF("{\"display\":%s}", invalid[i]);
            yyjson_doc* doc = yyjson_read(json.chars, json.length, 0);
            VERIFY(doc != nullptr);
            FFOptionsDisplay options = {};
            yyjson_val* key = nullptr;
            VERIFY(ffOptionsParseDisplayJsonConfig(&options, yyjson_doc_get_root(doc), &key) != nullptr);
            yyjson_doc_free(doc);
        }
    }
#ifndef _WIN32
    VERIFY(rejectsNumber(false, ""));
    VERIFY(rejectsNumber(true, ""));
#endif
    return 0;
}
