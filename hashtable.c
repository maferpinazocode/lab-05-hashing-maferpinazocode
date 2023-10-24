#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct RandomHashFunction {
    int* hashTable;
};

void initializeRandomHasher(struct RandomHashFunction* hasher, int M, int n) {
    hasher->hashTable = (int*)malloc(M * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < M; i++) {
        hasher->hashTable[i] = rand() % n;
    }
}

void destroyRandomHasher(struct RandomHashFunction* hasher) {
    free(hasher->hashTable);
}

struct HashTable {
    int attribute;
    int** slots;
    struct RandomHashFunction hashStrategy;
};