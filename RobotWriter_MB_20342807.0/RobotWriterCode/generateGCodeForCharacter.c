/**
 * generateGCodeForCharacter.c 
 * Part of CNC Plotter Project for MMME3085
 * 
 * Purpose: Converts character stroke data into G-code commands for the plotter
 *          Handles pen movement, scaling, and serial communication with the robot.
 * 
 * Author: [Mathis Bellino]
 * Student ID: [20342807]
 * Date: December 2024 
 * 
 * @param c Character to generate G-code for
 * @param font Pointer to font data structure
 * @param currentX Pointer to current X position (updated by function)
 * @param baselineY Pointer to baseline Y position
 * @param height Desired character height in mm (4-10mm)
 * @return SUCCESS or error code
 *
 */

#include "font.h"
#include "serial.h"
#include <stdio.h>

/* Constants defining pen states and success/error codes */
#define SUCCESS 0
#define ERROR_INVALID_PARAMS -1
#define ERROR_NO_STROKE_DATA -2

/* Track current pen state to minimize unnecessary pen movements */
static int currentPenState = 0;  // 0 = up, 1 = down

int generateGCodeForCharacter(char c, struct FontData* font, 
                            float* currentX, float* baselineY, float height) {
    /* Parameter validation */
    if (!font || !currentX || !baselineY || height < 4.0f || height > 10.0f) {
        printf("[ERROR] Invalid parameters in generateGCodeForCharacter\n");
        return ERROR_INVALID_PARAMS;
    }

    /* Get character data and validate */
    struct CharacterData* charData = &font->characters[(int)c];
    if (!charData || charData->numStrokes <= 0) {
        printf("[WARNING] No stroke data for character '%c'\n", c);
        return ERROR_NO_STROKE_DATA;
    }

    /* Calculate scaling factor */
    float scale = height / 18.0f;

    /* Process each stroke */
    for (int i = 0; i < charData->numStrokes; i++) {
        struct MovementData* stroke = &charData->strokes[i];
        
        /* Calculate scaled coordinates */
        float targetX = *currentX + (stroke->x * scale);
        float targetY = *baselineY + (stroke->y * scale);

        /* Check if pen state needs to change */
        if (currentPenState != stroke->penDown) {
            char buffer[100];
            sprintf(buffer, "S%d\n", stroke->penDown ? 1000 : 0);
            PrintBuffer(buffer);
            WaitForReply();
            currentPenState = stroke->penDown;
        }

        /* Generate movement command with conditional feed rate */
        static int feedRateIncluded = 0;  // Track if feed rate has been added
        char buffer[100];

        if (stroke->penDown && !feedRateIncluded) {
            sprintf(buffer, "G1 X%.2f Y%.2f F1000\n", targetX, targetY);
            feedRateIncluded = 1;  // Mark feed rate as included
        } else {
            sprintf(buffer, "G%d X%.2f Y%.2f\n", stroke->penDown ? 1 : 0, targetX, targetY);
        }

        PrintBuffer(buffer);
        WaitForReply();

    }

    /* Update X position for next character */
    float characterWidth = charData->strokes[charData->numStrokes - 1].x * scale;
    *currentX += characterWidth + (scale * 1.0f);  // Add character spacing

    return SUCCESS;
}