#include "ui.h"
#include <stdio.h>
#include <windows.h>

void displayProgress(ProcessState state) {
    printf("\n[Progress]: ");
    const char* states[] = {
        "Init", "Height Set", "File Loaded", "Font Loaded",
        "Processing", "Generating", "Complete", "Error"
    };
    
    for (int i = 0; i < 8; i++) {
        if (i > 0) printf(" -> ");
        
        if ((int)i == (int)state) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                  FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("[%s]", states[i]);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                                  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        } else {
            printf("%s", states[i]);
        }
    }
    printf("\n\n");
}

void showFilePrompt() {
    printf("\n----------------------------------------\n");
    printf("Please enter the name of your text file:\n");
    printf("(The file should be in the same directory)\n");
    printf("----------------------------------------\n");
}

void showHeightPrompt() {
    printf("\n----------------------------------------\n");
    printf("Enter text height (4-10mm):\n");
    printf("This will determine the size of the text\n");
    printf("----------------------------------------\n");
}

void showProcessingStatus(const char* word, int wordCount, int lineCount, float x, float y) {
    printf("\rProcessing: Word %d, Line %d | Current Position: (%.2f, %.2f) | Current Word: '%s'",
           wordCount, lineCount, x, y, word);
    fflush(stdout);
}

void showSummary(int wordCount, int lineCount, float finalX, float finalY, double timeTaken) {
    printf("\n\n========== Drawing Summary ==========\n");
    printf("Total Words: %d\n", wordCount);
    printf("Total Lines: %d\n", lineCount);
    printf("Final Position: (%.2f, %.2f)\n", finalX, finalY);
    printf("Time Taken: %.2f seconds\n", timeTaken);
    printf("===================================\n\n");
}

void showError(const char* message) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                          FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("\n[ERROR] %s\n", message);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 
                          FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}