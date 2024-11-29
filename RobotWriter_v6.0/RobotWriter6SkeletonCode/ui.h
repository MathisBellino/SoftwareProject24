#ifndef UI_H
#define UI_H

#include "font.h"

typedef enum {
    STATE_INIT,
    STATE_HEIGHT_SET,
    STATE_FILE_LOADED,
    STATE_FONT_LOADED,
    STATE_PROCESSING_TEXT,
    STATE_GENERATING_GCODE,
    STATE_COMPLETE,
    STATE_ERROR
} ProcessState;

void displayProgress(ProcessState state);
void showFilePrompt(void);
void showHeightPrompt(void);
void showProcessingStatus(const char* word, int wordCount, int lineCount, float x, float y);
void showSummary(int wordCount, int lineCount, float finalX, float finalY, double timeTaken);
void showError(const char* message);

#endif // UI_H