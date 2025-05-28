#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// 연락처 구조체 정의
typedef struct Contact {
    char name[50];
    char number[15];
    int age;
    struct Contact* next;
} Contact;

// 해시 테이블 선언
Contact* hashTable[TABLE_SIZE];

// 해시 함수
unsigned int Hash(char* name) {
    unsigned int hashValue = 0;
    while (*name) {
        hashValue = (hashValue * 31) + *name;
        name++;
    }
    return hashValue % TABLE_SIZE;
}

// 연락처 저장
void InsertContact(char* name, char* number, int age) {
    unsigned int idx = Hash(name);
    Contact* current = hashTable[idx];

    // 중복 확인
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("이미 존재하는 연락처입니다.\n");
            return;
        }
        current = current->next;
    }

    // 새로운 연락처 삽입
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->number, number);
    newContact->age = age;
    newContact->next = hashTable[idx];
    hashTable[idx] = newContact;
    printf("연락처가 저장되었습니다.\n");
}

// 연락처 전체 출력
void PrintAllContact() {
    int empty = 1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while (current != NULL) {
            printf("이름 : %s, 전화번호 : %s, 나이 : %d\n", current->name, current->number, current->age);
            current = current->next;
            empty = 0;
        }
    }
    if (empty) {
        printf("저장된 연락처가 없습니다.\n");
    }
}

// 연락처 찾기
void FindContact(char* name) {
    unsigned int idx = Hash(name);
    Contact* current = hashTable[idx];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("이름 : %s, 전화번호 : %s, 나이 : %d\n", current->name, current->number, current->age);
            return;
        }
        current = current->next;
    }
    printf("해당 연락처가 없습니다.\n");
}

// 연락처 삭제
void DeleteContact(char* name) {
    unsigned int idx = Hash(name);
    Contact* current = hashTable[idx];
    Contact* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) {
                hashTable[idx] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("연락처가 삭제되었습니다.\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("해당 연락처가 없습니다.\n");
}

// 프로그램 종료 전 메모리 해제
void FreeAllContacts() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while (current != NULL) {
            Contact* temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}

// 메뉴 화면 출력 함수
void PrintMenu() {
    printf("-------전화번호부-------\n");
    printf("1. 전화번호부 전체 출력\n");
    printf("2. 전화번호부에 번호 저장\n");
    printf("3. 전화번호 찾기 (이름 입력)\n");
    printf("4. 전화번호 삭제 (이름 입력)\n");
    printf("5. 전화번호부 종료\n\n");
}

// 메인 함수
int main(void) {
    int cmd;

    // 해시 테이블 초기화
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    while (1) {
        PrintMenu();  // 메뉴 화면 출력
        printf("명령어를 입력하세요: ");
        scanf("%d", &cmd);

        if (cmd == 5) {
            FreeAllContacts();
            printf("프로그램을 종료합니다.\n");
            break;
        }

        switch (cmd) {
        case 1: {
            PrintAllContact();
            printf("\n");
            break;
        }

        case 2: {
            char name[50];
            char number[15];
            int age;

            printf("이름 입력: ");
            scanf("%s", name);
            printf("전화번호 입력: ");
            scanf("%s", number);
            printf("나이 입력: ");
            scanf("%d", &age);

            InsertContact(name, number, age);
            printf("\n");
            break;
        }

        case 3: {
            char name[50];
            printf("찾고자하는 번호의 이름을 입력하세요 : ");
            scanf("%s", name);
            FindContact(name);
            printf("\n");
            break;
        }

        case 4: {
            char name[50];
            printf("삭제할 번호의 이름을 입력하세요 : ");
            scanf("%s", name);
            DeleteContact(name);
            printf("\n");
            break;
        }

        default:
            printf("잘못된 입력입니다.\n\n");
            break;
        }
    }

    return 0;
}