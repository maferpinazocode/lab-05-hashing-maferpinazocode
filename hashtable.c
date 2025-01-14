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

int find(struct HashTable* table, int x) {
    int index = table->hashStrategy.hashTable[x];
    if (table->bucket[index] != NULL) {
        int i = 0;
        while (table->bucket[index][i] != -1) {
            if (table->bucket[index][i] == x) {
                return x;
            }
            i++;
        }
    }
    return -1;
}

void deleteHashTable(struct HashTable* table) {
    for (int i = 0; i < table->attribute; i++) {
        if (table->bucket[i] != NULL) {
            free(table->bucket[i]);
        }
    }
    free(table->bucket);
    destroyRandomHasher(&(table->hashStrategy));
    free(table);
}

int main() {
    struct HashTable* hashTable = createHashTable(7, 10);

    insert(hashTable, 7);
    insert(hashTable, 15);
    insert(hashTable, 25);

    printf("Searching 15: %d\n", find(hashTable, 15)); //should print 15
    printf("Searching 7: %d\n", find(hashTable, 7));  //should print 7

    delete(hashTable, 15);

    printf("Searching 15 after deleting: %d\n", find(hashTable, 15)); //should print -1
    
    deleteHashTable(hashTable);

    return 0;
}