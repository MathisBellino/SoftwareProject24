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

    int marker, ascii, numStrokes;
    int totalCharacters = 128;  // Assuming ASCII characters (0–127)
    int processedCharacters = 0;

    printf("Font data being allocated to memory...\n");

    while (fscanf(fPtr, "%d %d %d", &marker, &ascii, &numStrokes) == 3) {
        if (marker == 999 && ascii >= 0 && ascii < 128) {
            font->characters[ascii].numStrokes = numStrokes;
            font->characters[ascii].strokes = (struct MovementData*)calloc(numStrokes, sizeof(struct MovementData));

            for (int i = 0; i < numStrokes; i++) {
                fscanf(fPtr, "%f %f %d", 
                    &font->characters[ascii].strokes[i].x, 
                    &font->characters[ascii].strokes[i].y, 
                    (int*)&font->characters[ascii].strokes[i].penDown);
            }
        }

        // Update progress
        processedCharacters++;
        float progress = (float)processedCharacters / totalCharacters * 100.0f;

        // Print progress bar
        printf("\r[");
        int barWidth = 50;  // Adjust for desired width
        int pos = (int)(barWidth * (progress / 100.0f));
        for (int j = 0; j < barWidth; j++) {
            if (j < pos) printf("=");
            else if (j == pos) printf(">");
            else printf(" ");
        }
        printf("] %.2f%%", progress);
        fflush(stdout);

        // Simulate time for reading (remove this in real processing)
        Sleep(100);  // 100 milliseconds
    }

    printf("\n[DEBUG] Finished reading font file.\n");
    fclose(fPtr);
    return font;
}
