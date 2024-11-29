#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include "font.h"
#include "ui.h"
#include "serial.h"
#include <windows.h>
#include <time.h>

#define bdrate 115200

void SendCommands(char* buffer);
int processTextFile(const char* filename, struct FontData* font, float height);
void processWord(const char* word, struct FontData* font, float* currentX, float* currentY, float height);


int main() {
    char buffer[100];
    float height;
    struct FontData* font = NULL;
    char textFilename[256];
    char fontFilename[256];

    // Required port initialization
    if (CanRS232PortBeOpened() == -1) {
        printf("\nUnable to open the COM port (specified in serial.h)");
        exit(0);
    }

    // Robot wake-up sequence (must preserve)
    printf("\nAbout to wake up the robot\n");
    sprintf(buffer, "\n");
    PrintBuffer(&buffer[0]);
    Sleep(100);
    WaitForDollar();

    // Initialize robot (must preserve)
    sprintf(buffer, "G1 X0 Y0 F1000\n");
    SendCommands(buffer);
    sprintf(buffer, "M3\n");
    SendCommands(buffer);
    sprintf(buffer, "S0\n");
    SendCommands(buffer);

    // Prompt for text height
    do {
        printf("Enter text height (4-10mm): ");
        scanf("%f", &height);
    } while (height < 4 || height > 10);

    // Load font data with error handling
    while (true) {
        printf("Enter font file name: ");
        scanf("%s", fontFilename);

        printf("Loading font data...\n");
        font = storeFontData(fontFilename);
        if (!font) {
            printf("[ERROR] Could not load font file. Please try again.\n");
            continue;  // Prompt for the font file name again
        }
        displayProgress(STATE_FONT_LOADED);
        break;  // Exit the loop if the font is loaded successfully
    }

    // Prompt for text file name with error handling
    while (true) {
        printf("Enter text file name: ");
        scanf("%s", textFilename);

        // Process text file
        printf("Processing text file...\n");
        displayProgress(STATE_PROCESSING_TEXT);

        if (!processTextFile(textFilename, font, height)) {
            printf("[ERROR] Text processing failed. Please try again.\n");
            continue;  // Prompt for the text file name again
        } else {
            displayProgress(STATE_COMPLETE);
            printf("Text processing complete!\n");
            break;  // Exit the loop if the text file is processed successfully
        }
    }


    // Cleanup
    if (font) freeFontData(font);
    CloseRS232Port();  // Required cleanup
    printf("Com port now closed\n");

    return 0;
}

void SendCommands(char* buffer) {
    PrintBuffer(buffer);
    WaitForReply();
    Sleep(100);
}

int processTextFile(const char* filename, struct FontData* font, float height) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\n[ERROR] Could not open text file: %s\n", filename);
        return 0;
    }

    // Start timing
    clock_t start_time = clock();

    char currentChar;
    char word[256] = {0};
    int wordIndex = 0;
    float currentX = 0.0f;
    float currentY = 0.0f;
    int wordCount = 0;
    int lineCount = 1;

    printf("\nProcessing text file: %s\n", filename);
    printf("----------------------------------------\n");
    printf("Height: %.2f mm\n", height);
    printf("Maximum line width: 100 mm\n");
    printf("----------------------------------------\n\n");

    while ((currentChar = fgetc(file)) != EOF) {
        if (currentChar == ' ' || currentChar == '\n') {
            // End of word reached
            word[wordIndex] = '\0';
            
            // Display progress
            printf("\rProcessing Word %d (Line %d): '%s'", ++wordCount, lineCount, word);
            fflush(stdout);
            
            processWord(word, font, &currentX, &currentY, height);
            wordIndex = 0;
            
            if (currentChar == '\n') {
                lineCount++;
                handleLineBreak(&currentX, &currentY);
                printf("\n");  // New line in console for better readability
            }
        } else {
            word[wordIndex++] = currentChar;
        }
    }

    // Process last word if exists
    if (wordIndex > 0) {
        word[wordIndex] = '\0';
        printf("\rProcessing Word %d (Line %d): '%s'", ++wordCount, lineCount, word);
        processWord(word, font, &currentX, &currentY, height);
    }

    // Calculate time taken
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\n\nText Processing Summary:\n");
    printf("----------------------------------------\n");
    printf("Total Words Processed: %d\n", wordCount);
    printf("Total Lines: %d\n", lineCount);
    printf("Final Position: X=%.2f, Y=%.2f\n", currentX, currentY);
    printf("Time Taken: %.2f seconds\n", time_taken);
    printf("----------------------------------------\n");

    fclose(file);
    return 1;
}
