#include "font.h"

float calculateWordWidth(const char* word, struct FontData* font, float height) {
    if (!word || !font) return 0.0f;
    
    float scale = height / 18.0f;
    float currentWidth = 0.0f;
    
    // Calculate width by finding max X coordinate of each character
    for (int i = 0; word[i] != '\0'; i++) {
        struct CharacterData* charData = &font->characters[(int)word[i]];
        
        // Find rightmost X coordinate of current character
        float charMaxX = 0.0f;
        for (int j = 0; j < charData->numStrokes; j++) {
            if (charData->strokes[j].x > charMaxX) {
                charMaxX = charData->strokes[j].x;
            }
        }
        
        // Add scaled character width
        currentWidth += charMaxX * scale;
    }
    
    return currentWidth;
}
