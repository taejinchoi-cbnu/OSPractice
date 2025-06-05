#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "smalloc.h"

#define TABLE_SIZE 100

typedef struct Contact {
    char name[50];
    char number[15];
    int age;
    struct Contact* next;
} Contact;

Contact* hashTable[TABLE_SIZE];

void ClearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int IsValidName(const char* name) {
    return (name != NULL && strlen(name) > 0);
}

int IsValidNumber(const char* number) {
    if (number == NULL || strlen(number) == 0) return 0;
    for (int i = 0; number[i] != '\0'; i++)
        if (!((number[i] >= '0' && number[i] <= '9') || number[i] == '-'))
            return 0;
    return 1;
}

int IsValidAge(int age) {
    return age > 0 && age < 150;
}

unsigned int Hash(char* name) {
    unsigned int hashValue = 0;
    while (*name) {
        hashValue = (hashValue * 31) + *name;
        name++;
    }
    return hashValue % TABLE_SIZE;
}

void InsertContact(char* name, char* number, int age) {
    if (!IsValidName(name) || !IsValidNumber(number) || !IsValidAge(age)) {
        printf("오류: 이름, 전화번호 또는 나이가 유효하지 않습니다.\n");
        return;
    }
    unsigned int idx = Hash(name);
    Contact* current = hashTable[idx];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("오류: 이미 존재하는 연락처입니다.\n");
            return;
        }
        current = current->next;
    }
    Contact* newContact = (Contact*)smalloc(sizeof(Contact));
    if (newContact == NULL) {
        printf("오류: 메모리 할당 실패.\n");
        return;
    }
    strcpy(newContact->name, name);
    strcpy(newContact->number, number);
    newContact->age = age;
    newContact->next = hashTable[idx];
    hashTable[idx] = newContact;
    printf("연락처 저장 완료.\n");
}

void PrintAllContact() {
    int empty = 1;
    printf("\n===== 연락처 목록 =====\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while (current != NULL) {
            printf("이름: %s | 전화번호: %s | 나이: %d\n", current->name, current->number, current->age);
            current = current->next;
            empty = 0;
        }
    }
    if (empty) printf("저장된 연락처가 없습니다.\n");
    printf("=======================\n\n");
}

void FindContact(char* name) {
    if (!IsValidName(name)) {
        printf("오류: 유효하지 않은 이름.\n");
        return;
    }
    unsigned int idx = Hash(name);
    Contact* current = hashTable[idx];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("이름: %s| 전화번호: %s, 나이: %d\n", current->name, current->number, current->age);
            return;
        }
        current = current->next;
    }
    printf("해당 연락처가 없습니다.\n\n");
}

void DeleteContact(char* name) {
    if (!IsValidName(name)) {
        printf("오류: 유효하지 않은 이름.\n");
        return;
    }
    unsigned int idx = Hash(name);
    Contact* current = hashTable[idx];
    Contact* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) hashTable[idx] = current->next;
            else previous->next = current->next;
            sfree(current);
            printf("연락처 삭제 완료.\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("해당 연락처가 없습니다.\n");
}

void UpdateContactInfo(char* name) {
    if (!IsValidName(name)) {
        printf("오류: 유효하지 않은 이름.\n");
        return;
    }
    unsigned int idx = Hash(name);
    Contact* current = hashTable[idx];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("현재: %s, %s, %d\n", current->name, current->number, current->age);
            char new_number[15];
            int new_age;
            char choice;

            printf("전화번호 업데이트? (y/n): ");
            ClearInputBuffer();
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                printf("새 전화번호: ");
                scanf("%s", new_number);
                if (IsValidNumber(new_number)) {
                    strcpy(current->number, new_number);
                    printf("전화번호 업데이트 완료.\n");
                } else {
                    printf("유효하지 않은 전화번호.\n");
                }
            }

            printf("나이 업데이트? (y/n): ");
            ClearInputBuffer();
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                printf("새 나이: ");
                scanf("%d", &new_age);
                if (IsValidAge(new_age)) {
                    current->age = new_age;
                    printf("나이 업데이트 완료.\n");
                } else {
                    printf("유효하지 않은 나이.\n");
                }
            }
            return;
        }
        current = current->next;
    }
    printf("해당 연락처가 없습니다.\n");
}

void RenameContact(char* oldName) {
    if (!IsValidName(oldName)) {
        printf("오류: 유효하지 않은 이름.\n");
        return;
    }
    unsigned int old_idx = Hash(oldName);
    Contact* current = hashTable[old_idx];
    Contact* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->name, oldName) == 0) break;
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("연락처를 찾을 수 없습니다.\n");
        return;
    }
    char newName[50];
    printf("새 이름 입력: ");
    scanf("%s", newName);
    if (!IsValidName(newName)) {
        printf("유효하지 않은 이름.\n");
        return;
    }
    if (strcmp(oldName, newName) != 0) {
        unsigned int new_idx = Hash(newName);
        Contact* check = hashTable[new_idx];
        while (check != NULL) {
            if (strcmp(check->name, newName) == 0) {
                printf("이미 존재하는 이름입니다.\n");
                return;
            }
            check = check->next;
        }
    }
    if (previous == NULL) hashTable[old_idx] = current->next;
    else previous->next = current->next;
    current->next = NULL;
    strcpy(current->name, newName);
    unsigned int new_idx = Hash(newName);
    current->next = hashTable[new_idx];
    hashTable[new_idx] = current;
    printf("이름 변경 완료.\n");
}

void FreeAllContacts() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while (current != NULL) {
            Contact* temp = current;
            current = current->next;
            sfree(temp);
        }
        hashTable[i] = NULL;
    }
    printf("모든 메모리 해제 완료.\n");
}

void PrintMenu() {
    printf("==== 전화번호부 ====\n");
    printf("1. 모든 연락처 출력\n");
    printf("2. 새 연락처 저장\n");
    printf("3. 이름으로 연락처 찾기\n");
    printf("4. 이름으로 연락처 삭제\n");
    printf("5. 연락처 정보 업데이트\n");
    printf("6. 이름 변경\n");
    printf("7. 프로그램 종료\n");
    printf("====================\n");
}

int main(void) {
    int cmd;
    for (int i = 0; i < TABLE_SIZE; i++) hashTable[i] = NULL;
    while (1) {
        PrintMenu();
        printf("명령어 입력 (1-7): ");
        if (scanf("%d", &cmd) != 1) {
            printf("숫자를 입력해주세요.\n");
            ClearInputBuffer();
            continue;
        }
        ClearInputBuffer();
        if (cmd == 7) {
            FreeAllContacts();
            printf("종료합니다.\n");
            break;
        }
        switch (cmd) {
            case 1: PrintAllContact(); break;
            case 2: {
                char name[50], number[15];
                int age;
                printf("이름: "); scanf("%49s", name);
                printf("전화번호: "); scanf("%14s", number);
                printf("나이: ");
                if (scanf("%d", &age) != 1) {
                    printf("나이는 숫자여야 합니다.\n");
                    ClearInputBuffer();
                    break;
                }
                ClearInputBuffer();
                InsertContact(name, number, age);
                break;
            }
            case 3: {
                char name[50];
                printf("찾을 이름: "); scanf("%49s", name);
                ClearInputBuffer();
                FindContact(name);
                break;
            }
            case 4: {
                char name[50];
                printf("삭제할 이름: "); scanf("%49s", name);
                ClearInputBuffer();
                DeleteContact(name);
                break;
            }
            case 5: {
                char name[50];
                printf("업데이트할 이름: "); scanf("%49s", name);
                ClearInputBuffer();
                UpdateContactInfo(name);
                break;
            }
            case 6: {
                char name[50];
                printf("이름 변경할 기존 이름: "); scanf("%49s", name);
                ClearInputBuffer();
                RenameContact(name);
                break;
            }
            default:
                printf("1에서 7 사이 숫자를 입력하세요.\n");
        }
        printf("\n");
    }
    return 0;
}