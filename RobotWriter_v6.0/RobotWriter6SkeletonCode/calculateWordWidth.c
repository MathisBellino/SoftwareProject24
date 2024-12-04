/**
 * calculateWordWidth.c
 * Part of CNC Plotter Project for MMME3085
 * 
 * 
 * Calculates the total width a word will occupy when drawn
 * Accounts for character spacing and scaling based on desired height
 *
 * @param word Null-terminated string to calculate width for
 * @param font Pointer to font data containing stroke information
 * @param height Desired text height in millimeters (4-10mm)
 * @return Total width in millimeters, or 0.0f if parameters invalid
 */


#include <stdio.h>
#include "font.h"

#define BASE_HEIGHT 18
#define MIN_HEIGHT 4
#define MAX_HEIGHT 10

float calculateWordWidth(const char* word, struct FontData* font, float height) {
    /* Parameter validation */
    if (!word || !font) {
        fprintf(stderr, "Error: Invalid parameters in calculateWordWidth\n");
        return 0.0f;
    }
    
    if (height < MIN_HEIGHT || height > MAX_HEIGHT) {
        fprintf(stderr, "Error: Height %.2f outside valid range %d-%d\n", 
                height, MIN_HEIGHT, MAX_HEIGHT);
        return 0.0f;
    }

    /* Calculate scaling factor */
    float scaleFactor = height / (float)BASE_HEIGHT;
    
    /* Process each character */
    float totalWidth = 0.0f;
    int i = 0;
    
    while (word[i] != '\0') {
        /* Get character data */
        int ascii = (int)word[i];
        struct CharacterData* charData = &font->characters[ascii];
        
        if (!charData->strokes || charData->numStrokes <= 0) {
            fprintf(stderr, "Warning: No stroke data for character '%c'\n", word[i]);
            i++;
            continue;
        }

        /* Find maximum x-coordinate */
        float maxX = charData->strokes[0].x;
        for (int j = 1; j < charData->numStrokes; j++) {
            if (charData->strokes[j].x > maxX) {
                maxX = charData->strokes[j].x;
            }
        }
        
        totalWidth += maxX * scaleFactor;
        i++;
    }
    
    return totalWidth;
}