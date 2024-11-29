#include "font.h"
#include "serial.h"
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

static bool currentPenState = false;

static void setPenState(bool penDown) {
    if (currentPenState != penDown) {
        char buffer[100];
        sprintf(buffer, "S%d\n", penDown ? 1000 : 0);
        PrintBuffer(buffer);
        WaitForReply();
        currentPenState = penDown;
    }
}

static void moveToPosition(float x, float y, bool drawing) {
    char buffer[100];
    sprintf(buffer, "G%d X%.2f Y%.2f%s\n", 
            drawing ? 1 : 0, x, y, 
            drawing ? " F1000" : "");
    PrintBuffer(buffer);
    WaitForReply();
}

void generateGCodeForCharacter(char c, struct FontData* font, float* currentX, float* currentY, float height) {
    if (!font || !currentX || !currentY) return;
    
    struct CharacterData* charData = &font->characters[(int)c];
    float scale = height / 18.0f;

    for (int i = 0; i < charData->numStrokes; i++) {
        struct MovementData* stroke = &charData->strokes[i];
        float x = *currentX + (stroke->x * scale);
        float y = *currentY + (stroke->y * scale);

        setPenState(false);
        moveToPosition(x, y, false);

        if (stroke->penDown) {
            setPenState(true);
            moveToPosition(x, y, true);
        }

        *currentX = x;
        *currentY = y;
    }
}

void handleLineBreak(float* currentX, float* currentY) {
    if (!currentX || !currentY) return;
    
    *currentX = 0.0f;
    *currentY += 5.0f;
    
    setPenState(false);
    moveToPosition(0.0f, *currentY, false);
}