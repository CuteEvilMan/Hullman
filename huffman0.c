#include <stdio.h>
#include <stdlib.h>

// 定义霍夫曼树的节点结构体
typedef struct node {
    char character;       // 节点存储的字符
    unsigned int frequency;  // 字符出现的频率
    struct node *rightChild; // 右子节点指针
    struct node *leftChild;  // 左子节点指针
} Node;

// 定义链表节点结构体，用于存储节点和指向下一个链表节点的指针
typedef struct list {
    Node *nodeData;        // 链表节点存储的数据
    struct list *nextNode;  // 指向下一个链表节点的指针
} ListNode;

// 定义霍夫曼编码结构体
typedef struct {
    char character;                 // 编码的字符
    int huffmanCode[101];           // 霍夫曼编码数组
} HuffmanCode;

// 函数声明
ListNode* createListNode(Node* nodeData);
Node *createNode(char character, unsigned int frequency);
void swapNodes(ListNode *nodeA, ListNode *nodeB);
int compareFrequencies(ListNode *nodeA, ListNode *nodeB);
void sortList(ListNode *list);
void combineNodes(ListNode *list);
void buildHuffmanTree(ListNode **list);
ListNode* findNode(ListNode *list, char character);
void processInput(ListNode *list, char character);
void freeHuffmanTree(Node* node);
void freeHuffmanList(ListNode *list);
int isLeafNode(const Node* node);
void printCodes(const Node* root, int arr[], int top, FILE* file);

// 创建一个链表节点
ListNode* createListNode(Node* nodeData) {
    ListNode* newList = (ListNode*)malloc(sizeof(ListNode));
    newList->nodeData = nodeData;
    newList->nextNode = NULL;
    return newList;
}

// 创建一个新的数据二叉节点
Node *createNode(char character, unsigned int frequency) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->character = character;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->frequency = frequency;
    return newNode;
}

// 交换两个链表节点的数据
void swapNodes(ListNode *nodeA, ListNode *nodeB) {
    Node *tempNode = nodeA->nodeData;
    nodeA->nodeData = nodeB->nodeData;
    nodeB->nodeData = tempNode;
}

// 比较两个节点的频率
int compareFrequencies(ListNode *nodeA, ListNode *nodeB) {
    return nodeA->nodeData->frequency > nodeB->nodeData->frequency ||
           (nodeA->nodeData->frequency == nodeB->nodeData->frequency &&
            nodeA->nodeData->character > nodeB->nodeData->character);
}

// 将链表排序，使用选择排序算法
void sortList(ListNode *list) {
    if (list == NULL) {
        return;
    }
    ListNode *currentNode = list;
    while (currentNode != NULL) {
        ListNode *nextNode = currentNode->nextNode;
        while (nextNode != NULL) {
            if (compareFrequencies(nextNode, currentNode)) {
                swapNodes(currentNode, nextNode);
            }
            nextNode = nextNode->nextNode;
        }
        currentNode = currentNode->nextNode;
    }
}

// 从链表中提取两个节点，创建一个新的内部节点，并重新排序链表
void combineNodes(ListNode *list) {
    if (list == NULL || list->nextNode == NULL) {
        return;
    }
    ListNode *firstNode = list;
    while (firstNode->nextNode && firstNode->nextNode->nextNode)
        firstNode = firstNode->nextNode;
    if (firstNode->nextNode) {
        Node *newNode = createNode('\0',
            firstNode->nodeData->frequency + firstNode->nextNode->nodeData->frequency);
        newNode->leftChild = firstNode->nodeData;
        newNode->rightChild = firstNode->nextNode->nodeData;
        firstNode->nextNode->nodeData = NULL;
        free(firstNode->nextNode);
        firstNode->nextNode = NULL;
        firstNode->nodeData = newNode;
        sortList(list);
    }
}

// 构建霍夫曼树
void buildHuffmanTree(ListNode **list) {
    sortList(*list);
    if (*list == NULL) {return;}
    while ((*list)->nextNode != NULL) {
        combineNodes(*list);
    }
}

// 在链表中查找特定字符的节点
ListNode* findNode(ListNode *list, char character) {
    while (list != NULL && list->nodeData->character != character) {
        list = list->nextNode;
    }
    return list;
}

// 处理输入字符，更新链表中相应节点的频率或添加新节点
void processInput(ListNode *list, char character) {
    ListNode *foundNode = findNode(list, character);
    if (foundNode == NULL) {
        ListNode *endNode = list;
        while (endNode->nextNode != NULL) {
            endNode = endNode->nextNode;
        }
        endNode->nextNode = createListNode(createNode(character, 1));
    } else if (foundNode->nodeData->character == character) {
        foundNode->nodeData->frequency++;
    }
}

// 递归释放霍夫曼树的内存
void freeHuffmanTree(Node* node) {
    if (node == NULL) {return;}
    freeHuffmanTree(node->leftChild);
    freeHuffmanTree(node->rightChild);
    free(node);
}

// 释放链表的内存
void freeHuffmanList(ListNode *list) {
    if (list == NULL) {return;}
    freeHuffmanList(list->nextNode);
    freeHuffmanTree(list->nodeData);
    free(list);
}

// 判断节点是否为叶子节点
int isLeafNode(const Node* node) {
    return node->leftChild == NULL && node->rightChild == NULL;
}

// 打印霍夫曼编码
void printCodes(const Node* root, int arr[], int top, FILE *file) {
    HuffmanCode* huffmanCodes;
    if (top >= 100) {
        exit(0);
    }
    if (root->leftChild) {
        arr[top] = 0;
        printCodes(root->leftChild, arr, top + 1, file);
    }
    if (root->rightChild) {
        arr[top] = 1;
        printCodes(root->rightChild, arr, top + 1, file);
    }
    if (isLeafNode(root)) {
        fprintf(file, "%c: ", root->character);
        for (int i = 0; i < top; ++i) {
            fprintf(file, "%d", arr[i]);
            if(root->character >= 0) {
                huffmanCodes[root->character].huffmanCode[i] = arr[i];
                huffmanCodes[root->character].huffmanCode[100] = top;
            }
        }
        fprintf(file, "\n");
    }
}

int charCount = 0;
HuffmanCode *huffmanCodes;

// 主函数
int main(void) {
    FILE *fileInput = fopen("D:\\q.txt", "r+b");
    if (fileInput == NULL) {exit(23);}
    FILE *fileOutput = fopen("D:\\1111.txt", "w+b");
    if (fileOutput == NULL) {exit(24);}
    fseek(fileInput, 1l, SEEK_SET);

    char inputChar;
    if (fread(&inputChar, sizeof(char), 1, fileInput) == 0) {
        if(inputChar == EOF) {
            exit(25);
        }
    }
    ListNode *huffmanList = createListNode(createNode(inputChar, 1));
    while (fread(&inputChar, sizeof(char), 1, fileInput) == 1) {
        processInput(huffmanList, inputChar);
        charCount++;
    }

    buildHuffmanTree(&huffmanList);
    int codeArray[100], top = 0;
    huffmanCodes = (HuffmanCode*)malloc(charCount * sizeof(HuffmanCode));
    for (int i = 0; i < charCount; ++i) {
        huffmanCodes[i].huffmanCode[100] = 0;
    }
    printCodes(huffmanList->nodeData, codeArray, top, fileOutput);
    fseek(fileInput, 1l, SEEK_SET);
    while (fread(&inputChar, sizeof(char), 1, fileInput) == 1) {
        if (inputChar >= 0) {
            for(int i = 0; i < huffmanCodes[inputChar].huffmanCode[100]; ++i) {
                fprintf(fileOutput, "%d", huffmanCodes[inputChar].huffmanCode[i]);
            }
        }
    }
    fclose(fileInput);
    fclose(fileOutput);
    freeHuffmanList(huffmanList);
    free(huffmanCodes);
    return 0;
}
