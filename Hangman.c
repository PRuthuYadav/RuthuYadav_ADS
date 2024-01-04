#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 20
// Queue node structure
struct Node {
    char data;
    struct Node* next;
};
// Queue structure
struct Queue {
    struct Node* front;
    struct Node* rear;
};
// Initialize an empty queue
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (queue == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    queue->front = queue->rear = NULL;
    return queue;
}
// Enqueue a character to the queue
void enqueue(struct Queue* queue, char data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
}

    newNode->data = data;
    newNode->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}
// Dequeue a character from the queue
char dequeue(struct Queue* queue) {
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    char data = queue->front->data;
    struct Node* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    return data;
}
// Display the word with underscores for unguessed letters
void displayWord(const struct Queue* wordQueue, const char* guessedLetters) {
    struct Node* current = wordQueue->front;
    while (current != NULL) {
        if (strchr(guessedLetters, current->data) != NULL) {
            printf("%c ", current->data);
        } else {
            printf("_ ");
        }
        current = current->next;

    }
    printf("\n");
}
// Check if the word has been completely guessed
int isGameOver(const struct Queue* wordQueue, const char* guessedLetters) {
    struct Node* current = wordQueue->front;
    while (current != NULL) {
        if (strchr(guessedLetters, current->data) == NULL) {
            return 0; // Game is not over
        }
        current = current->next;
    }
    return 1; // Game is over
}
// Free memory allocated for the queue
void freeQueue(struct Queue* queue) {
    struct Node* current = queue->front;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}
int main() {
    // Initialize a queue for the word
    struct Queue* wordQueue = createQueue();
    // Get the word to guess from the player
    char word[MAX_WORD_LENGTH];
    printf("Enter the word to guess: ");
    scanf("%s", word);
    // Enqueue each character of the word to the queue
    for (int i = 0; word[i] != '\0'; i++) {
        enqueue(wordQueue, word[i]);

    }
    const int maxAttempts = 6;
    char guessedLetters[maxAttempts + 1]; // +1 for null terminator
    int incorrectAttempts = 0;
    memset(guessedLetters, 0, sizeof(guessedLetters));
    printf("Welcome to Hangman!\n");
    while (1) {
        printf("Guessed letters: %s\n", guessedLetters);
        displayWord(wordQueue, guessedLetters);
        // Ask for a letter
        char guess;
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        // Check if the letter has already been guessed
        if (strchr(guessedLetters, guess) != NULL) {
            printf("You've already guessed that letter.\n");
            continue;
        }
        guessedLetters[strlen(guessedLetters)] = guess;
        // Check if the letter is in the word
        if (strchr(word, guess) == NULL) {
            printf("Incorrect guess!\n");
            incorrectAttempts++;
            // Check if the player has reached the maximum allowed attempts
            if (incorrectAttempts == maxAttempts) {
                printf("Sorry, you've run out of attempts. The word was: %s\n", word);
                break;
            }
        } else {
            printf("Correct guess!\n");
        }
        // Check if the game is over
        if (isGameOver(wordQueue, guessedLetters)) {
            printf("Congratulations! You've guessed the word: %s\n", word);

  break;
     }
    }
    // Free memory allocated for the word queue
    freeQueue(wordQueue);
    return 0;
}
