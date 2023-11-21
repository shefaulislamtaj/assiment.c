#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100

struct Record {
    int id;
    char name[50];
    float value;
};

void addRecord(FILE *file, struct Record *records, int *recordCount);
void displayRecords(FILE *file);
void searchRecords(FILE *file);
void modifyRecord(FILE *file);
void deleteRecord(FILE *file);

int main() {
    FILE *file = fopen("data.txt", "a+");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct Record records[MAX_RECORDS];
    int recordCount = 0;

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add new data\n");
        printf("2. Display contents\n");
        printf("3. Search for data\n");
        printf("4. Modify data\n");
        printf("5. Delete data\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addRecord(file, records, &recordCount);
                break;
            case 2:
                displayRecords(file);
                break;
            case 3:
                searchRecords(file);
                break;
            case 4:
                modifyRecord(file);
                break;
            case 5:
                deleteRecord(file);
                break;
            case 6:
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }

    } while (choice != 6);

    fclose(file);

    return 0;
}

void addRecord(FILE *file, struct Record *records, int *recordCount) {
    struct Record newRecord;

    printf("Enter ID: ");
    scanf("%d", &newRecord.id);
    printf("Enter name: ");
    scanf("%s", newRecord.name);
    printf("Enter value: ");
    scanf("%f", &newRecord.value);

    fprintf(file, "%d %s %.2f\n", newRecord.id, newRecord.name, newRecord.value);


    records[*recordCount] = newRecord;
    (*recordCount)++;
}

void displayRecords(FILE *file) {
    rewind(file);
    char buffer[100];

    printf("\nContents of the file:\n");
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
}

void searchRecords(FILE *file) {
    rewind(file);
    char search[50];
    char buffer[100];

    printf("Enter search term: ");
    scanf("%s", search);

    printf("\nMatching records:\n");
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, search) != NULL) {
            printf("%s", buffer);
        }
    }
}

void modifyRecord(FILE *file) {
    rewind(file);
    int targetID;
    printf("Enter the ID of the record to modify: ");
    scanf("%d", &targetID);


    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        exit(1);
    }

    struct Record modifiedRecord;
    while (fscanf(file, "%d %s %f", &modifiedRecord.id, modifiedRecord.name, &modifiedRecord.value) == 3) {
        if (modifiedRecord.id == targetID) {
            printf("Enter new name: ");
            scanf("%s", modifiedRecord.name);
            printf("Enter new value: ");
            scanf("%f", &modifiedRecord.value);
        }
        fprintf(tempFile, "%d %s %.2f\n", modifiedRecord.id, modifiedRecord.name, modifiedRecord.value);
    }

    fclose(file);
    fclose(tempFile);

    remove("data.txt");
    rename("temp.txt", "data.txt");

    file = fopen("data.txt", "a+");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
}

void deleteRecord(FILE *file) {
    rewind(file);
    int targetID;
    printf("Enter the ID of the record to delete: ");
    scanf("%d", &targetID);


    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        exit(1);
    }

    struct Record deletedRecord;
    while (fscanf(file, "%d %s %f", &deletedRecord.id, deletedRecord.name, &deletedRecord.value) == 3) {
        if (deletedRecord.id != targetID) {
            fprintf(tempFile, "%d %s %.2f\n", deletedRecord.id, deletedRecord.name, deletedRecord.value);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("data.txt");
    rename("temp.txt", "data.txt");

    file = fopen("data.txt", "a+");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
}
