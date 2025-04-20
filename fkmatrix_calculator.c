#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MATRICES 10
#define NAME_SIZE 50

typedef struct {
    char name[NAME_SIZE];
    int **data;
    int rows;
    int cols;
} NamedMatrix;

NamedMatrix vault[MAX_MATRICES];
int vaultCount = 0;

// Function to allocate memory for a matrix
int** allocateMatrix(int rows, int cols) {
    int **matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
        matrix[i] = (int*)malloc(cols * sizeof(int));
    return matrix;
}

// Function to free matrix memory
void freeMatrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++)
        free(matrix[i]);
    free(matrix);
}

// Input matrix values
void inputMatrix(int **matrix, int rows, int cols) {
    printf("Enter elements (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            scanf("%d", &matrix[i][j]);
}

// Print matrix
void printMatrix(int **matrix, int rows, int cols) {
    printf("Matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

// Find matrix by name
NamedMatrix* findMatrixByName(char *name) {
    for (int i = 0; i < vaultCount; i++)
        if (strcmp(vault[i].name, name) == 0)
            return &vault[i];
    return NULL;
}

// Store matrix
void storeMatrix(int **matrix, int rows, int cols) {
    if (vaultCount >= MAX_MATRICES) {
        printf("Vault full! Cannot store more matrices.\n");
        return;
    }
    printf("Enter name to store this matrix: ");
    scanf("%s", vault[vaultCount].name);
    vault[vaultCount].data = matrix;
    vault[vaultCount].rows = rows;
    vault[vaultCount].cols = cols;
    vaultCount++;
    printf("Stored as '%s'\n", vault[vaultCount - 1].name);
}

// Add matrices
void addMatrices() {
    char a[NAME_SIZE], b[NAME_SIZE];
    printf("Enter first matrix name: ");
    scanf("%s", a);
    printf("Enter second matrix name: ");
    scanf("%s", b);
    
    NamedMatrix *m1 = findMatrixByName(a);
    NamedMatrix *m2 = findMatrixByName(b);
    
    if (!m1 || !m2 || m1->rows != m2->rows || m1->cols != m2->cols) {
        printf("Matrix not found or size mismatch.\n");
        return;
    }
    
    int **result = allocateMatrix(m1->rows, m1->cols);
    for (int i = 0; i < m1->rows; i++)
        for (int j = 0; j < m1->cols; j++)
            result[i][j] = m1->data[i][j] + m2->data[i][j];
    
    printMatrix(result, m1->rows, m1->cols);
    storeMatrix(result, m1->rows, m1->cols);
}

// Multiply matrices
void multiplyMatrices() {
    char a[NAME_SIZE], b[NAME_SIZE];
    printf("Enter first matrix name: ");
    scanf("%s", a);
    printf("Enter second matrix name: ");
    scanf("%s", b);
    
    NamedMatrix *m1 = findMatrixByName(a);
    NamedMatrix *m2 = findMatrixByName(b);
    
    if (!m1 || !m2 || m1->cols != m2->rows) {
        printf("Matrix not found or incompatible for multiplication.\n");
        return;
    }
    
    int **result = allocateMatrix(m1->rows, m2->cols);
    for (int i = 0; i < m1->rows; i++)
        for (int j = 0; j < m2->cols; j++) {
            result[i][j] = 0;
            for (int k = 0; k < m1->cols; k++)
                result[i][j] += m1->data[i][k] * m2->data[k][j];
        }
    
    printMatrix(result, m1->rows, m2->cols);
    storeMatrix(result, m1->rows, m2->cols);
}

// Transpose matrix
void transposeMatrix() {
    char name[NAME_SIZE];
    printf("Enter matrix name to transpose: ");
    scanf("%s", name);
    
    NamedMatrix *m = findMatrixByName(name);
    if (!m) {
        printf("Matrix not found.\n");
        return;
    }
    
    int **result = allocateMatrix(m->cols, m->rows);
    for (int i = 0; i < m->rows; i++)
        for (int j = 0; j < m->cols; j++)
            result[j][i] = m->data[i][j];
    
    printMatrix(result, m->cols, m->rows);
    storeMatrix(result, m->cols, m->rows);
}

// Create and store new matrix
void createMatrix() {
    int r, c;
    printf("Enter rows and cols: ");
    scanf("%d %d", &r, &c);
    int **matrix = allocateMatrix(r, c);
    inputMatrix(matrix, r, c);
    printMatrix(matrix, r, c);
    storeMatrix(matrix, r, c);
}

// List all stored matrices
void listMatrices() {
    if (vaultCount == 0) {
        printf("No matrices stored.\n");
        return;
    }
    printf("Stored Matrices:\n");
    for (int i = 0; i < vaultCount; i++)
        printf("%d. %s (%dx%d)\n", i + 1, vault[i].name, vault[i].rows, vault[i].cols);
}

int main() {
    int choice;
    while (1) {
        printf("\n=== MATRIX VAULT MENU ===\n");
        printf("1. Create New Matrix\n");
        printf("2. Add Matrices\n");
        printf("3. Multiply Matrices\n");
        printf("4. Transpose Matrix\n");
        printf("5. List Stored Matrices\n");
        printf("6. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: createMatrix(); break;
            case 2: addMatrices(); break;
            case 3: multiplyMatrices(); break;
            case 4: transposeMatrix(); break;
            case 5: listMatrices(); break;
            case 6:
                for (int i = 0; i < vaultCount; i++)
                    freeMatrix(vault[i].data, vault[i].rows);
                printf("Exiting...\n");
                return 0;
            default: printf("Invalid option.\n");
        }
    }
}
