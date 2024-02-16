#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_PASSWORD_LENGTH 12
#define MAX_PASSWORD_LENGTH 20
#define CHARSET_LENGTH 75
#define COMMON_FILENAME "CommonPasswd.txt"
#define GENERATED_FILENAME "GeneratedPasswd.txt"

char* generatePassword() {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$^&*()_=+[]{}|;:,.<>?";
    const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
    const char numbers[] = "0123456789";
    const char symbols[] = "!@#$^&*()_=+[]{}|;:,.<>?";
    int length = MIN_PASSWORD_LENGTH + rand() % (MAX_PASSWORD_LENGTH - MIN_PASSWORD_LENGTH + 1);

    char *generatedPassword = (char *)malloc((length + 1) * sizeof(char));
    if (generatedPassword == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

// Assign the first four characters based on the patterns
    generatedPassword[0] = uppercase[rand() % strlen(uppercase)];
    generatedPassword[1] = lowercase[rand() % strlen(lowercase)];
    generatedPassword[2] = numbers[rand() % strlen(numbers)];
    generatedPassword[3] = symbols[rand() % strlen(symbols)];

// Assign the rest of the characters randomly from the charset
    for (int i = 4; i < length; i++) {
        generatedPassword[i] = charset[rand() % CHARSET_LENGTH];
    }
    generatedPassword[length] = '\0';

// Shuffle the password to randomize the order
    for (int i = length - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = generatedPassword[i];
        generatedPassword[i] = generatedPassword[j];
        generatedPassword[j] = temp;
    }

    return generatedPassword;
}

void savePasswordToFile(const char *password, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening %s\n", filename);
        exit(1);
    }
    fprintf(file, "Generated Password Length: %zu\n", strlen(password));
    fprintf(file, "Generated Password: %s\n", password);
    fclose(file);
}

int isPasswordCommon(const char *password, const char *commonFilename) {
    FILE *file = fopen(commonFilename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening %s\n", commonFilename);
        exit(1);
    }

    char commonLine[MAX_PASSWORD_LENGTH];
    while (fgets(commonLine, sizeof(commonLine), file) != NULL) {
        commonLine[strcspn(commonLine, "\n")] = '\0'; // Remove newline character
        if (strcmp(password, commonLine) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    clock_t start_time = clock();

    char *password = generatePassword();
    printf("Generated Password: %s\n", password);

    savePasswordToFile(password, GENERATED_FILENAME);

    int weak = isPasswordCommon(password, COMMON_FILENAME);

    double elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    printf("Time taken for comparison: %.6f seconds\n", elapsed_time);

    if (weak) {
        printf("The generated password is weak.\n");
    } else {
        printf("The generated password is strong.\n");
    }

    free(password);

    return 0;
}
