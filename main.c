#include <stdio.h>
#include <stdlib.h>
#define CHAR 256
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
    extern int number;
    ListNode *foundNode = findNode(list, character);
    if (foundNode == NULL) {
        ListNode *endNode = list;
        while (endNode->nextNode != NULL) {
            endNode = endNode->nextNode;
        }
        endNode->nextNode = createListNode(createNode(character, 1));
        number++;

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
    extern HuffmanCode*hhh;
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
       fprintf(file, "%d||", root->character);
        fprintf(file, "%d||", root->frequency);
        for (int i = 0; i < top; ++i) {
            fprintf(file, "%d", arr[i]);
                hhh[root->character+CHAR/2].huffmanCode[i]=arr[i];
                hhh[root->character+CHAR/2].huffmanCode[100]=top;
        }
        fprintf(file, "\n");
    }
}
HuffmanCode *hhh;
int number=1;
// 主函数
int main(void) {
    int in=1;
    scanf("%d", &in);
    if(in==0) {
        FILE *fileInput = fopen("D:\\1.txt", "r+b");
        if (fileInput == NULL) {exit(23);}
        FILE *fileOutput = fopen("D:\\2.txt", "w+b");
        if (fileOutput == NULL) {exit(24);}
        fseek(fileInput, 0l, SEEK_SET);

        char inputChar;
        if (fread(&inputChar, sizeof(char), 1, fileInput) == 0) {
            if(inputChar == EOF) {
                exit(25);
            }
        }
        ListNode *huffmanList = createListNode(createNode(inputChar, 1));
        while (fread(&inputChar, sizeof(char), 1, fileInput) == 1) {
            processInput(huffmanList, inputChar);

        }

        buildHuffmanTree(&huffmanList);
        int codeArray[100], top = 0;
        hhh=(HuffmanCode*)malloc(CHAR*sizeof(HuffmanCode));
        for (int i = 0; i < CHAR; ++i) {
            hhh[i].huffmanCode[100] = 0;
        }
        fprintf(fileOutput, "%d\n", number);
        printCodes(huffmanList->nodeData, codeArray, top, fileOutput);
        fseek(fileInput, 0l, SEEK_SET);
        while (fread(&inputChar, sizeof(char), 1, fileInput) == 1) {
            for(int i=0;i<hhh[inputChar+CHAR/2].huffmanCode[100];++i) {
                fprintf(fileOutput, "%d", hhh[inputChar+CHAR/2].huffmanCode[i]);

            }
        }
        fclose(fileInput);
        fclose(fileOutput);
        freeHuffmanList(huffmanList);
        free(hhh);
        return 0;
    }
        else {
            FILE *fp0 = fopen("D:\\2.txt", "r");
            if (fp0 == NULL) {
                perror("Error opening file for reading");
                exit(23);
            }
            FILE *fp1 = fopen("D:\\1.txt", "w");
            if (fp1 == NULL) {
                fclose(fp0);
                perror("Error opening file for writing");
                exit(24);
            }

            int s;
            fscanf(fp0, "%d", &s); // 读取字符总数

            ListNode *huffmanList = NULL;
            for (int i = 0; i < s; ++i) {
                char c;
                unsigned int d;
                // 假设文件格式是 "字符||频率||字符"，例如 "65||97||A\n"
                fscanf(fp0, "%d||%d||%c\n", &d, &d, &c);
                Node *newNode = createNode(c, d);
                if (huffmanList == NULL) {
                    huffmanList = createListNode(newNode);
                } else {
                    ListNode *current = huffmanList;
                    while (current->nextNode != NULL) {
                        current = current->nextNode;
                    }
                    current->nextNode = createListNode(newNode);
                }
            }

            buildHuffmanTree(&huffmanList);

            Node *root = huffmanList->nodeData; // 假设 huffmanList->nodeData 是根节点
            char bit;
            Node *currentNode = root;
            while ((bit = fgetc(fp0)) != EOF) {
                if (bit == '0') {
                    currentNode = currentNode->leftChild;
                } else if (bit == '1') {
                    currentNode = currentNode->rightChild;
                } else {
                    fprintf(stderr, "Invalid bit encountered: %c\n", bit);
                    fclose(fp0);
                    fclose(fp1);
                    freeHuffmanList(huffmanList);
                    exit(1);
                }

                if (isLeafNode(currentNode)) {
                    fprintf(fp1, "%c", currentNode->character);
                    currentNode = root; // 重置为根节点以开始新的解码
                }
            }

            fclose(fp0);
            fclose(fp1);
            freeHuffmanList(huffmanList);
            return 0;
        }
}