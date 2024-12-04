/*
 * main.c
 * Part of CNC Plotter Project for MMME3085
 * 
 * Purpose: Main control program for the CNC text plotting system.
 *          Manages user input, font loading, text processing, and robot communication.
 *          Includes robust error handling and adheres to coding best practices.
 * 
 * Author: [Mathis Bellino]
 * Student ID: [20342807]
 * Date: December 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  // For true/false definitions
#include <conio.h>
#include "font.h"
#include "serial.h"
#include <windows.h>
#include <time.h>

/* Constants */
#define bdrate 115200
#define MIN_HEIGHT 4.0f
#define MAX_HEIGHT 10.0f
#define BUFFER_SIZE 100
#define FILENAME_SIZE 256

/* Function Prototypes */
void SendCommands(char* buffer);  // Sends G-code commands to the robot
int processTextFile(const char* filename, struct FontData* font, float height);  // Processes text for plotting

int main() {
    char buffer[BUFFER_SIZE];  // Buffer for G-code commands
    float height;              // Text height input by the user
    struct FontData* font = NULL;  // Pointer to loaded font data
    char textFilename[FILENAME_SIZE];  // Name of the text file to process
    char fontFilename[FILENAME_SIZE];  // Name of the font file to load

    /* Initialize the RS232 port for robot communication */
    if (CanRS232PortBeOpened() == -1) {
        printf("[ERROR] Unable to open the COM port (specified in serial.h)\n");
        exit(EXIT_FAILURE);  // Use standard exit codes for clarity
    }

    /* Wake up the robot */
    printf("[INFO] About to wake up the robot\n");
    sprintf(buffer, "\n");
    PrintBuffer(buffer);
    Sleep(100);  // Synchronization delay
    WaitForDollar();

    /* Initialize robot position */
    sprintf(buffer, "G1 X0 Y0 F1000\n");
    SendCommands(buffer);
    sprintf(buffer, "M3\n");
    SendCommands(buffer);
    sprintf(buffer, "S0\n");
    SendCommands(buffer);

    /* Get text height with validation */
    do {
        printf("Enter text height (%.1f-%.1f mm): ", MIN_HEIGHT, MAX_HEIGHT);
        if (scanf("%f", &height) != 1 || height < MIN_HEIGHT || height > MAX_HEIGHT) {
            printf("[ERROR] Invalid input. Please enter a value between %.1f and %.1f.\n", MIN_HEIGHT, MAX_HEIGHT);
            while (getchar() != '\n');  // Clear invalid input from the buffer
        } else {
            break;
        }
    } while (true);

    /* Load font data */
    while (true) {
        printf("Enter font file name: ");
        scanf("%s", fontFilename);

        printf("[INFO] Loading font data...\n");
        font = storeFontData(fontFilename);
        if (!font) {
            printf("[ERROR] Could not load font file. Please try again.\n");
            continue;
        }
        printf("[INFO] Font loaded successfully!\n");
        break;
    }

    /* Process text file */
    printf("Enter text file name: ");
    scanf("%s", textFilename);
    printf("[INFO] Processing text file...\n");

    while (processTextFile(textFilename, font, height) != SUCCESS) {
        printf("[ERROR] Text processing failed. Please try again.\n");
        printf("Enter text file name: ");
        scanf("%s", textFilename);
    }

    printf("[INFO] Text processing complete!\n");

    /* Cleanup */
    if (font) {
        freeFontData(font);  // Free dynamically allocated memory
    }

    /* Return pen to origin after drawing finished */
    SendCommands("G0 X0 Y0\n");

    CloseRS232Port();  // Ensure the port is properly closed
    printf("[INFO] COM port now closed\n");


    return EXIT_SUCCESS;  // Program completed successfully
}

/**
 * Processes a text file by sending G-code commands for each word/line to the robot.
 * 
 * @param filename The name of the text file to process.
 * @param font Pointer to the loaded font data structure.
 * @param height Height of the text to be plotted.
 * @return SUCCESS if the processing was successful; an error code otherwise.
 */
int processTextFile(const char* filename, struct FontData* font, float height) {
    printf("[DEBUG] Starting text processing...\n");

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[ERROR] Could not open text file: %s\n", filename);
        return EXIT_FAILURE;  // Return failure if file cannot be opened
    }

    char currentChar;
    char word[256] = {0};
    int wordIndex = 0;
    float currentX = 0.0f;
    float currentY = -height;  // Start the first line below the Y=0 line
    int lineCount = 1;

    while ((currentChar = fgetc(file)) != EOF) {
        if (currentChar == ' ' || currentChar == '\n') {
            word[wordIndex] = '\0';
            if (wordIndex > 0) {
                if (processWord(word, font, &currentX, &currentY, height) != SUCCESS) {
                    printf("[ERROR] Word processing failed for '%s'\n", word);
                    fclose(file);  // Ensure file is closed before returning
                    return EXIT_FAILURE;
                }
            }
            wordIndex = 0;

            if (currentChar == '\n') {
                lineCount++;
                if (handleLineBreak(&currentX, &currentY, &height) != SUCCESS) {
                    printf("[ERROR] Line break failed at line %d\n", lineCount);
                    fclose(file);
                    return EXIT_FAILURE;
                }
            }
        } else {
            word[wordIndex++] = currentChar;
        }
    }

    if (wordIndex > 0) {
        word[wordIndex] = '\0';
        if (processWord(word, font, &currentX, &currentY, height) != SUCCESS) {
            printf("[ERROR] Word processing failed for '%s'\n", word);
            fclose(file);
            return EXIT_FAILURE;
        }
    }

    fclose(file);
    return SUCCESS;
}

/**
 * Sends G-code commands to the robot and ensures synchronization.
 * 
 * @param buffer Null-terminated string containing the G-code command.
 */
void SendCommands(char* buffer) {
    PrintBuffer(buffer);
    WaitForReply();
    Sleep(100);  // Required delay for synchronization
}
