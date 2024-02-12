#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
// Generate password
    unsigned int seed = (unsigned int)time(NULL);
    seed *= 1103515245 + 12345;
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$^&*()_=+[]{}|;:,.<>?";
    int minlength = 12;
    unsigned int length = minlength + seed % 5;
// Dynamically allocate memory for the generated password
    char *generatedPassword = (char*)malloc((length + 1) * sizeof(char));  // +1 for null terminator
    if (generatedPassword == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (unsigned int i = 0; i < length; i++) {
        unsigned int index = seed % (sizeof(charset) - 1);
        generatedPassword[i] = charset[index];
    }
    generatedPassword[length] = '\0';  // Null-terminate the string
// Print generated password
    printf("Generated Password Length: %u\n", length);
    printf("Generated Password: %s\n", generatedPassword);
// Open and append generated password to GeneratedPasswd.txt
    FILE *generatedFile = fopen("GeneratedPasswd.txt", "a");
    if (generatedFile == NULL) {
        fprintf(stderr, "Error opening GeneratedPasswd.txt\n");
        free(generatedPassword);  // Free allocated memory before returning
        return 1;
    }
    fprintf(generatedFile, "Generated Password Length: %u\n", length);
    fprintf(generatedFile, "Generated Password: %s\n", generatedPassword);
    fclose(generatedFile);
// Measure time before comparison
    clock_t start_time = clock();
// Open CommonPasswd.txt for reading
    FILE *commonFile = fopen("CommonPasswd.txt", "r");
    if (commonFile == NULL) {
        fprintf(stderr, "Error opening CommonPasswd.txt\n");
        free(generatedPassword);  // Free allocated memory before returning
        return 1;
    }
// Read each line from CommonPasswd.txt and compare with generated password
    char commonLine[100];  // Assuming maximum line length is 100 characters
    int matchFound = 0; // Flag to indicate if a match is found
    while (fgets(commonLine, sizeof(commonLine), commonFile) != NULL) {
        // Remove newline character
        commonLine[strcspn(commonLine, "\n")] = '\0';
        // Compare generated password with common password
        if (strcmp(generatedPassword, commonLine) == 0) {
            printf("Generated password matches a common password in CommonPasswd.txt: %s\n", commonLine);
            matchFound = 1;
            break;  // Exit loop if match found
        }
    }
// Print password strength
    if (matchFound) {
        printf("The generated password is weak.\n");
    } else {
        printf("The generated password is strong.\n");
    }
// Measure time after comparison
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken for comparison: %.6f seconds\n", elapsed_time);
// Close CommonPasswd.txt
    fclose(commonFile);
// Do not free allocated memory to keep the generated passwords
    return 0;
}