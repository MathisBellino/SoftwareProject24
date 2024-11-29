#include <stdio.h>
#include "font.h"

void processWord(const char* word, struct FontData* font, float* currentX, float* currentY, float height) {
    if (!word || !font || !currentX || !currentY) return;

    // Calculate if word fits on current line
    float wordWidth = calculateWordWidth(word, font, height);
    float maxWidth = 100.0f;

    // If word won't fit on current line
    if (*currentX + wordWidth > maxWidth) {
        printf("\n[Line Break] Word '%s' exceeds line width. Moving to next line.", word);
        handleLineBreak(currentX, currentY);
    }

    // Draw each character
    for (int i = 0; word[i] != '\0'; i++) {
        char c = word[i];
        // Only process if character has valid strokes
        if (font->characters[(int)c].numStrokes > 0) {
            printf("\n  -> Drawing character '%c' at position (%.2f, %.2f)", c, *currentX, *currentY);
            generateGCodeForCharacter(c, font, currentX, currentY, height);
        }
    }
}