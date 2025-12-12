#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    int id;
    char lastname[30];
    int birth_year;
    float grade;
} Student;

typedef struct Node {
    Student data;
    struct Node* nextName;   
    struct Node* nextGrade;  
} Node;

Node* headName = NULL;
Node* headGrade = NULL;

Node* createNode(Student st) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->data = st;
    n->nextName = NULL;
    n->nextGrade = NULL;
    return n;
}


void insertByName(Node* newNode) {
    if (!headName || strcmp(newNode->data.lastname, headName->data.lastname) < 0) {
        newNode->nextName = headName;
        headName = newNode;
        return;
    }

    Node* cur = headName;
    while (cur->nextName &&
           strcmp(cur->nextName->data.lastname, newNode->data.lastname) < 0) {
        cur = cur->nextName;
    }

    newNode->nextName = cur->nextName;
    cur->nextName = newNode;
}


void insertByGrade(Node* newNode) {
    if (!headGrade || newNode->data.grade > headGrade->data.grade) {
        newNode->nextGrade = headGrade;
        headGrade = newNode;
        return;
    }

    Node* cur = headGrade;
    while (cur->nextGrade && cur->nextGrade->data.grade > newNode->data.grade) {
        cur = cur->nextGrade;
    }

    newNode->nextGrade = cur->nextGrade;
    cur->nextGrade = newNode;
}

void insertStudent(Student st) {
    Node* newNode = createNode(st);
    insertByName(newNode);
    insertByGrade(newNode);
}

void printByName() {
    Node* cur = headName;
    printf("\nСписок по фамилии:\n");
    while (cur) {
        printf("%s (балл %.1f)\n", cur->data.lastname, cur->data.grade);
        cur = cur->nextName;
    }
}

void printByGrade() {
    Node* cur = headGrade;
    printf("\nСписок по баллу:\n");
    while (cur) {
        printf("%s (%.1f)\n", cur->data.lastname, cur->data.grade);
        cur = cur->nextGrade;
    }
}

void printByNameRec(Node* node) {
    if (!node) return;
    printByNameRec(node->nextName);
    printf("%s %.1f\n", node->data.lastname, node->data.grade);
}

Node* findByName(const char* lastname) {
    Node* cur = headName;
    while (cur) {
        if (strcmp(cur->data.lastname, lastname) == 0) return cur;
        cur = cur->nextName;
    }
    return NULL;
}

void deleteByName(const char* lastname) {
    Node *cur, *prev;

    cur = headName;
    prev = NULL;
    while (cur && strcmp(cur->data.lastname, lastname) != 0) {
        prev = cur;
        cur = cur->nextName;
    }
    if (!cur) return;

    if (!prev) headName = cur->nextName;
    else prev->nextName = cur->nextName;

    Node* del = cur;
    cur = headGrade;
    prev = NULL;
    while (cur && cur != del) {
        prev = cur;
        cur = cur->nextGrade;
    }

    if (cur) {
        if (!prev) headGrade = cur->nextGrade;
        else prev->nextGrade = cur->nextGrade;
    }

    free(del);
}

int main() {
    Student s1 = {1, "Ivanov", 2003, 4.7};
    Student s2 = {2, "Petrov", 2002, 3.9};
    Student s3 = {3, "Sidorov", 2004, 4.9};
    Student s4 = {4, "Alexeev", 2001, 4.7};

    insertStudent(s1);
    insertStudent(s2);
    insertStudent(s3);
    insertStudent(s4);

    printByName();
    printByGrade();

    printf("\nРекурсивный вывод по фамилии:\n");
    printByNameRec(headName);

    printf("\nПоиск 'Petrov':\n");
    Node* f = findByName("Petrov");
    if (f) printf("Найден: %s, %.1f\n", f->data.lastname, f->data.grade);

    printf("\nУдаляем 'Alexeev'\n");
    deleteByName("Alexeev");
    printByName();
    printByGrade();

    return 0;
}
