/*
 * freeFontData.c
 * Part of CNC Plotter Project for MMME3085
 *
 * Purpose: Memory management for font data structures
 *          Handles proper cleanup of dynamically allocated font data
 *
 * Author: [Mathis Bellino]
 * Student ID: [20342807]
 * Date: December 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "font.h"

/**
 * Frees dynamically allocated memory for the font data structure.
 * This function properly frees the memory used by the strokes of each character
 * and the font data structure itself to avoid memory leaks.
 * 
 * @param font Pointer to the font data structure to be freed.
 */

void freeFontData(struct FontData* font) {
    // Check if the font pointer is valid before attempting to free
    if (!font) {
        printf("Error: Cannot free NULL font data\n");
        return;  // Exit if font data is NULL
    }
    
    // Loop through all characters and free their stroke data
    for (int i = 0; i < 256; i++) {
        // Check if stroke data exists for the character
        if (font->characters[i].strokes) {
            // Free the dynamically allocated memory for strokes
            free(font->characters[i].strokes);
            font->characters[i].strokes = NULL;  // Avoid dangling pointer
        }
    }
    
    // Free the font data structure itself
    free(font);
}
