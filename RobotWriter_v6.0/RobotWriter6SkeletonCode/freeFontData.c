#include <stdlib.h>
#include "font.h"

void freeFontData(struct FontData* font) {
    if (!font) return;

    for (int i = 0; i < 256; i++) {
        if (font->characters[i].strokes) {
            free(font->characters[i].strokes);
            font->characters[i].strokes = NULL;
        }
    }

    free(font);
}
