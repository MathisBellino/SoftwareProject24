/*
 * handleLineBreak.c
 * Part of CNC Plotter Project for MMME3085
 *
 * Purpose: Manages text positioning and line breaks for the CNC plotter
 *          Ensures text stays within drawable bounds and maintains proper spacing
 *
 * Author: [Mathis Bellino]
 * Student ID: [20342807]
 * Date: December 2024
 */

#include <stdio.h>
#include "font.h"

#define MIN_Y -50.0f
#define MAX_Y 0.0f
#define LINE_SPACING 5.0f
#define SUCCESS 0
#define ERROR_INVALID_PARAM -1
#define ERROR_OUT_OF_BOUNDS -2

int handleLineBreak(float* currentX, float* currentY, float* height) {
 // Validate that the pointers are not NULL
    if (!currentX || !currentY) {
        fprintf(stderr, "Error: Invalid parameters in handleLineBreak\n");
        return ERROR_INVALID_PARAM;  // Return error if parameters are invalid
    }

    // Reset the X position to 0 to start a new line
    *currentX = 0.0f;

    // Move the Y position down by the defined line spacing
    *currentY -= LINE_SPACING + *height;

    // Ensure the Y position does not exceed the lower limit (MIN_Y)
    if (*currentY < MIN_Y) {
        fprintf(stderr, "Error: Text exceeds vertical bounds (Y=%.2f)\n", *currentY);
        return ERROR_OUT_OF_BOUNDS;  // Return error if text exceeds bounds
    }

    return SUCCESS;  // Return success if line break is handled properly
}

