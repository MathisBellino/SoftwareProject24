/*
 * font.h
 * Part of CNC Plotter Project for MMME3085
 *
 * Purpose: Core data structures and function declarations for font handling
 * Provides stroke data management for CNC plotter text generation
 *
 * Author: [Mathis Bellino]
 * Student ID: [20342807]
 * Date: December 2024
 */

#ifndef FONT_H
#define FONT_H

/* Data structures for font representation */
struct MovementData {
    float x;          /* X coordinate of stroke point */
    float y;          /* Y coordinate of stroke point */
    int penDown;      /* Pen state: 0=up, 1=down */
};

struct CharacterData {
    int numStrokes;                 /* Number of strokes in character */
    struct MovementData* strokes;   /* Dynamic array of stroke data */
};

struct FontData {
    struct CharacterData characters[256];  /* Full ASCII range support */
};

/* Common error codes from Week 9 lectures */
#define SUCCESS 0
#define ERROR_INVALID_PARAM -1
#define ERROR_PROCESSING -2

/* Function declarations with input/output documentation */
struct FontData* storeFontData(const char* filename);   /* Returns NULL on error */
void freeFontData(struct FontData* font);               /* Safe cleanup */
float calculateWordWidth(const char* word, struct FontData* font, float height);
int generateGCodeForCharacter(char c, struct FontData* font, 
                            float* currentX, float* currentY, float height);
int handleLineBreak(float* currentX, float* currentY, float* height); 
int processWord(const char* word, struct FontData* font, float* currentX, float* currentY, float height);
int processTextFile(const char* filename, struct FontData* font, float height);
#endif /* FONT_H */