#include <stdio.h>
#include <stdlib.h>

// RecordType structure
struct RecordType {
    int id;
    char name;
    int order;
};

// HashNode structure for chaining
struct HashNode {
    struct RecordType record;
    struct HashNode *next;
};

#define HASH_SIZE 10

//Fill out this structure
struct HashType {
    struct HashNode *table[HASH_SIZE];
};

// Compute the hash function
int hash(int x) {
    return x % HASH_SIZE;
}

// Parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void displayRecordsInHash(struct HashType *pHashArray, int hashSz) {
    int i;

    for (i = 0; i < hashSz; ++i) {
        printf("Index %d -> ", i);
        struct HashNode *current = pHashArray->table[i];
        while (current != NULL) {
            printf("%d %c %d -> ", current->record.id, current->record.name, current->record.order);
            current = current->next;
        }
        printf("\n");
    }
}

int main(void) {
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashType hashTable;
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable.table[i] = NULL;
    }

    for (int i = 0; i < recordSz; i++) {
        struct RecordType record = pRecords[i];
        int index = hash(record.id);

        struct HashNode *newNode = (struct HashNode *) malloc(sizeof(struct HashNode));
        if (newNode == NULL) {
            printf("Memory allocation failed\n");
            exit(-1);
        }
        newNode->record = record;
        newNode->next = NULL;

        if (hashTable.table[index] == NULL) {
            hashTable.table[index] = newNode;
        } else {
            struct HashNode *current = hashTable.table[index];
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    displayRecordsInHash(&hashTable, HASH_SIZE);

    for (int i = 0; i < HASH_SIZE; i++) {
        struct HashNode *current = hashTable.table[i];
        while (current != NULL) {
            struct HashNode *temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(pRecords);

    return 0;
}