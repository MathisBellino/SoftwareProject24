/**
 * storeFontData.c
 * Part of CNC Plotter Project for MMME3085
 *
 * Purpose: Loads and stores font data from file using dynamic allocation
 *          Follows lecture patterns for handling nested structure arrays
 *
 * Author: [Mathis Bellino]
 * Student ID: [20342807]
 * Date: December 2024
 * 
 * @param filename Path to the font data file to load
 * @return Pointer to populated FontData structure, or NULL on error
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>  // For Sleep
#include "font.h"
struct FontData* storeFontData(const char* filename) {
    FILE* fPtr = fopen(filename, "r");
    if (!fPtr) {
        perror("[ERROR] Failed to open font file");
        return NULL;
    }

    struct FontData* font = (struct FontData*)calloc(1, sizeof(struct FontData));
    if (!font) {
        perror("[ERROR] Memory allocation failed for FontData");
        fclose(fPtr);
        return NULL;
    }

    printf("Loading font data...\n");
    
    int marker, ascii, numStrokes;
    while (fscanf(fPtr, "%d %d %d", &marker, &ascii, &numStrokes) == 3) {
        // Skip invalid entries without showing errors
        if (marker != 999 || ascii < 0 || ascii >= 128 || numStrokes <= 0) {
            continue;
        }

        // Special handling for ASCII 127 - skip silently
        if (ascii == 127) {
            float x, y, pen;
            for (int i = 0; i < numStrokes; i++) {
                fscanf(fPtr, "%f %f %f", &x, &y, &pen);
            }
            continue;
        }

        struct MovementData* strokeData = (struct MovementData*)calloc(
            numStrokes, sizeof(struct MovementData)
        );
        
        if (!strokeData) {
            printf("[ERROR] Failed to allocate memory for strokes of ASCII %d\n", ascii);
            continue;
        }

        // Read stroke data
        int readSuccess = 1;
        for (int i = 0; i < numStrokes && readSuccess; i++) {
            if (fscanf(fPtr, "%f %f %d",
                &strokeData[i].x,
                &strokeData[i].y,
                (int*)&strokeData[i].penDown) != 3) {
                readSuccess = 0;
            }
        }

        if (!readSuccess) {
            printf("[ERROR] Failed to read stroke data for ASCII %d\n", ascii);
            free(strokeData);
            continue;
        }

        font->characters[ascii].numStrokes = numStrokes;
        font->characters[ascii].strokes = strokeData;
    }

    printf("[INFO] Font data loading complete.\n");
    fclose(fPtr);
    return font;
}