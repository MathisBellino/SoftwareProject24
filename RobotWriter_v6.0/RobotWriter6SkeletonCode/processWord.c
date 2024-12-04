/*
 * processWord.c
 * Part of CNC Plotter Project for MMME3085
 * 
 * Purpose: Processes individual words for plotter drawing
 * Handles word placement, line breaks, and character drawing
 * 
 * Author: [Mathis Bellino]
 * Student ID: [20342807]
 * Date: December 2024
 */

#include "font.h"
#include <stdio.h>

#define SUCCESS 0
#define ERROR_INVALID_PARAMS -1
#define ERROR_PROCESSING -2

int processWord(const char* word, struct FontData* font, float* currentX, float* currentY, float height) {
    if (!word || !font || !currentX || !currentY) {
        printf("[ERROR] Invalid parameters in processWord\n");
        return ERROR_INVALID_PARAMS;
    }

    // Calculate word width
    float wordWidth = calculateWordWidth(word, font, height);
    float maxWidth = 100.0f;  // Define maximum line width

    // Handle line breaks when the word doesn't fit
    if (*currentX + wordWidth > maxWidth) {
        int result = handleLineBreak(currentX, currentY, &height); // Pass address of height
        if (result != SUCCESS) {
            printf("[ERROR] Failed to handle line break\n");
            return ERROR_PROCESSING;
        }
    }

    // Process each character in the word
    for (int i = 0; word[i] != '\0'; i++) {
        char c = word[i];
        if (font->characters[(int)c].numStrokes > 0) {  // Only process valid characters
            int result = generateGCodeForCharacter(c, font, currentX, currentY, height);
            if (result != SUCCESS) {
                printf("[ERROR] Failed to generate G-code for character '%c'\n", c);
                return ERROR_PROCESSING;
            }
        }
    }

    return SUCCESS;
}