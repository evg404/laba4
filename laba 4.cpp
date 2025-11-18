#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

// Структура узла бинарного дерева
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* root = NULL;

// Функция создания нового узла
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Функция определения глубины узла
int getDepth(struct Node* node, int data, int depth) {
    if (node == NULL) return -1;
    if (node->data == data) return depth;

    if (data < node->data)
        return getDepth(node->left, data, depth + 1);
    else
        return getDepth(node->right, data, depth + 1);
}

// Функция проверки существования элемента
int exists(struct Node* node, int data) {
    if (node == NULL) return 0;
    if (node->data == data) return 1;

    if (data < node->data)
        return exists(node->left, data);
    else
        return exists(node->right, data);
}

// Функция добавления элемента в дерево
struct Node* addElement(struct Node* node, int data) {
    if (node == NULL) {
        return createNode(data);
    }

    if (data < node->data) {
        node->left = addElement(node->left, data);
    }
    else {
        node->right = addElement(node->right, data);
    }

    return node;
}

// Функция поиска элемента в дереве
struct Node* searchElement(struct Node* node, int key) {
    if (node == NULL || node->data == key) {
        return node;
    }

    if (key < node->data) {
        return searchElement(node->left, key);
    }
    else {
        return searchElement(node->right, key);
    }
}

// Функция подсчета вхождений элемента 
int countOccurrences(struct Node* node, int key) {
    if (node == NULL) {
        return 0;
    }

    if (key < node->data) {
        return countOccurrences(node->left, key);
    }
    else if (key > node->data) {
        return countOccurrences(node->right, key);
    }
    else {
        return 1 + countOccurrences(node->left, key) + countOccurrences(node->right, key);
    }
}

// Объединенная функция поиска и подсчета
void findAndCount(int key) {
    struct Node* result = searchElement(root, key);
    int count = countOccurrences(root, key);

    if (result != NULL) {
        printf("Элемент %d найден в дереве и встречается %d раз(а)!\n", key, count);
    }
    else {
        printf("Элемент %d не найден в дереве.\n", key);
    }
}

// Функция симметричного вывода дерева
void printInOrder(struct Node* node) {
    if (node != NULL) {
        printInOrder(node->left);
        printf("%d ", node->data);
        printInOrder(node->right);
    }
}

// Функция красивого вывода дерева
void printTreeVertical(struct Node* node, int level) {
    if (node == NULL) {
        return;
    }
    printTreeVertical(node->right, level + 1);

    for (int i = 0; i < level; i++) {
        printf("    ");
    }

    printf("%d\n", node->data);

    printTreeVertical(node->left, level + 1);
}

// Очистка дерева
void freeTree(struct Node* node) {
    if (node != NULL) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

// Функция для чтения строки с проверкой на *
int readInput(char* buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return -1;
    }

    // Удаляем символ новой строки
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }

    // Проверка на *
    if (strcmp(buffer, "*") == 0) {
        return 1;
    }

    return 0;
}

int main() {
    setlocale(LC_ALL, "");

    char input[100];
    int inputValue;
    int searchValue;

    // Устанавливаем корень дерева в 0
    root = createNode(0);

    printf("=== ПОСТРОЕНИЕ БИНАРНОГО ДЕРЕВА ПОИСКА ===\n");
    printf("Введите числа для построения дерева\n");
    printf("* - окончание ввода\n\n");

    while (1) {
        printf("Введите число: ");

        int result = readInput(input, sizeof(input));

        if (result == -1) {
            printf("Ошибка ввода.\n");
            continue;
        }
        else if (result == 1) {
            printf("Построение дерева завершено.\n\n");
            break;
        }

        // Проверка корректности ввода
        int valid = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (i == 0 && input[i] == '-') continue; 
            if (!isdigit(input[i])) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Некорректное значение. Введите целое число или '*' для завершения.\n");
            continue;
        }

        inputValue = atoi(input);

        // Проверка на повторяющийся элемент ниже 6 уровня
        if (exists(root, inputValue)) {
            int depth = getDepth(root, inputValue, 0);
            if (depth >= 6) {
                printf("Элемент %d уже существует на уровне %d. Повторные элементы ниже 6 уровня запрещены.\n",
                    inputValue, depth);
                continue;
            }
        }

        // Добавляем элемент (кроме корня, который уже 0)
        if (inputValue != 0) {
            root = addElement(root, inputValue);
        }
    }

    // Вывод построенного дерева
    printf("\n=== ПОСТРОЕННОЕ ДЕРЕВО ===\n");
    printf("Симметричный вывод: ");
    printInOrder(root);
    printf("\n\nВертикальное представление:\n");
    printTreeVertical(root, 0);

    // Поиск элементов
    printf("\n=== ПОИСК ЭЛЕМЕНТОВ В ДЕРЕВЕ ===\n");
    printf("Введите значение для поиска (* для выхода): ");

    if (readInput(input, sizeof(input)) == 1) {
        printf("Выход из программы.\n");
        freeTree(root);
        root = NULL;
        printf("Программа завершена.\n");
        return 0;
    }
    else {
        searchValue = atoi(input);
        findAndCount(searchValue);
    }

    // Дополнительный интерактивный поиск
    printf("\n=== ДОПОЛНИТЕЛЬНЫЙ ПОИСК ===\n");
    printf("Вводите значения для поиска (* для выхода):\n");

    while (1) {
        printf("Введите значение для поиска: ");

        int result = readInput(input, sizeof(input));

        if (result == 1) {
            break;
        }
        else if (result == -1) {
            printf("Ошибка ввода.\n");
            continue;
        }

        searchValue = atoi(input);
        findAndCount(searchValue);
        printf("\n");
    }

    // Освобождение памяти
    freeTree(root);
    root = NULL;

    printf("Программа завершена.\n");
    return 0;
}