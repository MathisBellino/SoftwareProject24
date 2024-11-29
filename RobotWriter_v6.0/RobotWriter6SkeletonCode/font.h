#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include <stdbool.h>

// Data structures
struct MovementData {
    float x;
    float y;
    int penDown;
};

struct CharacterData {
    int numStrokes;
    struct MovementData* strokes;
};

struct FontData {
    struct CharacterData characters[256];
};

// Function declarations
struct FontData* storeFontData(const char* filename);
void freeFontData(struct FontData* font);
void generateGCodeForCharacter(char c, struct FontData* font, float* currentX, float* currentY, float height);
void handleLineBreak(float* currentX, float* currentY);
float calculateWordWidth(const char* word, struct FontData* font, float height);
void processWord(const char* word, struct FontData* font, float* currentX, float* currentY, float height);

#endif // FONT_H