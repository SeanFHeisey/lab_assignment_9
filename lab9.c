#include <stdio.h>
#include <stdlib.h>

// Seam Heisey
// COP 3502
// 4/12/2023
// lab9

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
    // create a next pointer (for linkedlist structure)
	struct RecordType *next;
};

// Fill out this structure
struct HashType
{
    // create a pointer to a RecordType
	struct RecordType *data;
};

// Compute the hash function
int hash(int x, int tableSize)
{
    // hash function is x % tableSize
	return x % tableSize;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
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

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    int recordSize = parseData("input.txt", &record);
    for(int i=0; i<recordSize; i++){
        // call the hash function to get the index
        int index = hash(record[i].id, tableSize);
        // if the RecordType at that index is NULL
        if(hashTable[index].data==NULL){
            struct RecordType* pRecordIndex = &(record[index]);
            // set 'record' equal to the HashType pointer in the table at index
            hashTable[index].data = malloc(sizeof(struct RecordType));
            hashTable[index].data = pRecordIndex;
            hashTable[index].data->next = NULL;
        // else
        }else{
            struct RecordType *temp = NULL, 
            *node = malloc(sizeof(struct RecordType));
            node->next = NULL;
            node->id = record[i].id;
            node->name = record[i].name;
            node->order = record[i].order;
            temp = hashTable[index].data;
            // traverse to the end of the linkedlist and add 'record' to the end of it
            while(temp!=NULL&&temp->next!=NULL){
                temp = temp->next;
            }
            if(temp!=NULL)
                temp->next = node;
            else
                hashTable[index].data = node;
        }
     }
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	// for each entry in the table
    for(int i=0; i<hashSz; i++){
        // print the contents in that index
        if(pHashArray[i].data->next==NULL)
            printf("Index %d -> %d, %c, %d  -> NULL", i, pHashArray[i].data->id, pHashArray[i].data->name, pHashArray[i].data->order);
        else{
            printf("Index %d -> %d, %c, %d", i, pHashArray[i].data->id, pHashArray[i].data->name, pHashArray[i].data->order);
            // if index is occupied with any records, print all
            while(pHashArray[i].data->next != NULL){
                pHashArray[i].data = pHashArray[i].data->next;
                printf(" -> %d, %c, %d", pHashArray[i].data->id, pHashArray[i].data->name, pHashArray[i].data->order);
            }
            printf(" -> NULL");
        }
        printf("\n\n");
    }
    // The output should look like this (for example): "Index 1 -> 21715, Q, 16 -> 28876, a, 26 -> NULL"
}

// main function
int main(void)
{
	struct RecordType *pRecords;
	int recordSize = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSize);
	// Your hash implementation
	// Initialize the hash table
    struct HashType *pHashTable;
    // create a variable hashTableSize and assign it a value
    int hashTableSize = 11;
    // initialize a hashTable, use calloc (so everything is assigned to NULL)
    pHashTable = calloc(hashTableSize, sizeof(struct HashType));
    // for each record in pRecords, insert it into the hash table using the insertRecord function
    insertRecord(pHashTable, pRecords, hashTableSize);
    // call the display records function
    displayRecordsInHash(pHashTable, hashTableSize);
    // free all the allocated memory
    free(pRecords);
    free(pHashTable);
    return 0;
}