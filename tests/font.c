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
        ffFontInitPango(&font, "BlackChancery 12");
        VERIFY(ffStrbufEqualS(&font.pretty, "BlackChancery (12pt)"));
        ffFontDestroy(&font);
        ffFontInitPango(&font, "Sans Semi-Bold 12");
        VERIFY(ffStrbufEqualS(&font.pretty, "Sans (12pt, SemiBold)"));
        ffFontDestroy(&font);
    }
    {
        FFfont font;
        ffFontInitXft(&font, "Sans-1.2.3");
        VERIFY(font.size.length == 0);
        VERIFY(ffStrbufEqualS(&font.name, "Sans-1.2.3"));
        ffFontDestroy(&font);
        ffFontInitXft(&font, "Sans:size=12oops");
        VERIFY(font.size.length == 0);
        ffFontDestroy(&font);
    }
    {
        FFfont font;
        ffFontInitXlfd(&font, "-misc-fixed-medium-r-normal--14-18446744073709551716-75-75-c-70-iso10646-1");
        VERIFY(ffStrbufEqualS(&font.size, "14px"));
        ffFontDestroy(&font);
    }
    {
        FFfont font;
        ffFontInitQt(&font, "Sans,12,-1,5,50,0,0,0,0,0,Bold  Italic ");
        VERIFY(ffStrbufEqualS(&font.pretty, "Sans (12pt, Bold, Italic)"));
        ffFontDestroy(&font);
    }
    {
        FFfont font;
        ffFontInitQt(&font, "Monospace,-1,14,5,50,0,0,0,0,0,Regular");
        VERIFY(ffStrbufEqualS(&font.pretty, "Monospace (14px, Regular)"));
        ffFontDestroy(&font);
    }
    {
        FFfont font;
        ffFontInitPango(&font, "Sans 12 \t  ");
        VERIFY(ffStrbufEqualS(&font.pretty, "Sans (12pt)"));
        ffFontDestroy(&font);
    }
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
