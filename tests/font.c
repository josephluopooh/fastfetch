#include "common/font.h"

#include <stdio.h>
#include <stdlib.h>

static void verify(bool value, int line) {
    if (!value) {
        fprintf(stderr, "Font regression failed at line %d\n", line);
        exit(1);
    }
}

#define VERIFY(value) verify((value), __LINE__)

int main(void) {
    {
        FFfont font;
        ffFontInitPango(&font, "Sans 12px");
        VERIFY(ffStrbufEqualS(&font.size, "12px"));
        VERIFY(ffStrbufEqualS(&font.pretty, "Sans (12px)"));
        ffFontDestroy(&font);
        ffFontInitPango(&font, "3270Terminal");
        VERIFY(ffStrbufEqualS(&font.name, "3270Terminal"));
        VERIFY(font.size.length == 0);
        ffFontDestroy(&font);
    }
    return 0;
}
