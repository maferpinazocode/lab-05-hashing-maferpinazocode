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
    int** bucket;
    struct RandomHashFunction hashStrategy;
};

struct HashTable* createHashTable(int attribute, int n) {
    struct HashTable* table = (struct HashTable*)malloc(sizeof(struct HashTable));
    table->attribute = attribute;
    table->bucket = (int**)malloc(n * sizeof(int*));
    initializeRandomHasher(&(table->hashStrategy), n, n);

    for (int i = 0; i < n; i++) {
        table->bucket[i] = NULL;
    }

    return table;
}

int miMod(int x, int n) {
    return x % n;
}

void insert(struct HashTable* table, int x) {
    int index = table->hashStrategy.hashTable[x];
    int* newList = NULL;
    int length = 0;

    if (table->bucket[index] == NULL) {
        newList = (int*)malloc(2 * sizeof(int));
        newList[0] = x;
        newList[1] = -1;
    } else {
        while (table->bucket[index][length] != -1) {
            length++;
        }
        newList = (int*)malloc((length + 2) * sizeof(int));

        for (int i = 0; i < length; i++) {
            newList[i] = table->bucket[index][i];
        }
        newList[length] = x;
        newList[length + 1] = -1;
        free(table->bucket[index]);
    }

    table->bucket[index] = newList;
}

void delete(struct HashTable* table, int x) {
    int index = table->hashStrategy.hashTable[x];
    if (table->bucket[index] != NULL) {
        int length = 0;
        while (table->bucket[index][length] != -1) {
            length++;
        }

        int* updatedList = (int*)malloc((length - 1) * sizeof(int));
        int i = 0;
        int j = 0;

        while (i < length) {
            if (table->bucket[index][i] != x) {
                updatedList[j] = table->bucket[index][i];
                j++;
            }
            i++;
        }

        free(table->bucket[index]);
        if (j > 0) {
            updatedList[j] = -1;
            table->bucket[index] = updatedList;
        } else {
            table->bucket[index] = NULL;
        }
    }
}