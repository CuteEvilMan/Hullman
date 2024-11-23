
#pragma once
#include <stdio.h>
#include <stdlib.h>
#ifndef HUFFMAN_H
#define HUFFMAN_H

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

HuffmanCode *huffman_code;
int number=1;
// 函数声明
inline ListNode* createListNode(Node* nodeData);
inline  Node *createNode(char character, unsigned int frequency);
inline void swapNodes(ListNode *nodeA, ListNode *nodeB);
inline int compareFrequencies(const ListNode *nodeA, const ListNode *nodeB);
inline void sortList(ListNode *list);
inline void combineNodes(ListNode *list);
inline void buildHuffmanTree(ListNode **list);
inline ListNode* findNode(ListNode *list, char character);
inline void processInput(ListNode *list, char character);
inline void freeHuffmanTree(Node* node);
inline void freeHuffmanList(ListNode *list);
inline int isLeafNode(const Node* node);
inline void printCodes(const Node* root, int arr[], int top, FILE* file);
inline FILE* huffman_encode(const char*,const char*);
inline FILE* huffman_decode(const char*,const char*);
inline FILE* huffman_encode_r(const char*,const char*,const char*);
inline FILE* huffman_decode_r(const char*,const char*,const char*);
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
int  compareFrequencies(const ListNode *nodeA,const ListNode *nodeB) {
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
    extern HuffmanCode*huffman_code;
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
                huffman_code[root->character+CHAR/2].huffmanCode[i]=arr[i];
                huffman_code[root->character+CHAR/2].huffmanCode[100]=top;
        }
        fprintf(file, "\n");
    }
}
FILE* huffman_encode(const char*in,const char*out) {
    FILE *fileInput = fopen(in, "r+b");
    if (fileInput == NULL) {
        fprintf(stderr,"输入文件打开错误");
       return NULL;
    }
    FILE *fileOutput = fopen(out, "w+b");
    if (fileOutput == NULL) {
        fprintf(stderr,"输出文件打开错误");
        return NULL;
    }
    fseek(fileInput, 0l, SEEK_SET);

    char inputChar;
    if (fread(&inputChar, sizeof(char), 1, fileInput) == 1) {
        if(inputChar == EOF) {

            return fileOutput;
        }
    }
    ListNode *huffmanList = createListNode(createNode(inputChar, 1));
    number=1;
    while (fread(&inputChar, sizeof(char), 1, fileInput) == 1) {
        processInput(huffmanList, inputChar);

    }

    buildHuffmanTree(&huffmanList);
    int codeArray[100], top = 0;
    huffman_code=(HuffmanCode*)malloc(CHAR*sizeof(HuffmanCode));
    for (int i = 0; i < CHAR; ++i) {
        huffman_code[i].huffmanCode[100] = 0;
    }
    fprintf(fileOutput, "%d\n", number);
    printCodes(huffmanList->nodeData, codeArray, top, fileOutput);
    fseek(fileInput, 0l, SEEK_SET);
    while (fread(&inputChar, sizeof(char), 1, fileInput) == 1) {
        for(int i=0;i<huffman_code[inputChar+CHAR/2].huffmanCode[100];++i) {
            fprintf(fileOutput, "%d", huffman_code[inputChar+CHAR/2].huffmanCode[i]);

        }
    }
    fclose(fileInput);
    fclose(fileOutput);
    freeHuffmanList(huffmanList);
    free(huffman_code);
    return fileOutput;
}
FILE *huffman_decode(const char *in, const char *out) {
FILE *fp0 = fopen(in, "r+b");
        if (fp0 == NULL) {
            fprintf(stderr,"输入文件打开错误");
            return NULL;
        }
        FILE *fp1 = fopen(out, "w+b");
        if (fp1 == NULL) {
            fclose(fp0);
            fprintf(stderr,"输出文件打开错误");
            return NULL;
        }

        int s;
        fscanf(fp0, "%d", &s); // 读取字符总数

        ListNode *huffmanList = NULL;
        for (int i = 0; i < s; ++i) {
            while(fgetc(fp0) != '\n') {
            }
            int c;
            unsigned int d;
            // 假设文件格式是 "字符||频率||字符"，例如 "65||97||A\n"
            fscanf(fp0, "%d||%ud||\n", &c, &d);
            Node *newNode = createNode((char)c, d);
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
        while (fread(&bit, sizeof(char), 1, fp0) == 1) {
            if(bit=='\n') break;
        }
        while (fread(&bit, sizeof(char), 1, fp0) == 1) {
            if (bit == '0') {
                if(currentNode == NULL) {return NULL;}
                currentNode = currentNode->leftChild;
            } else if (bit == '1') {
                currentNode = currentNode->rightChild;
            } else {
                fprintf(stderr, "Invalid bit encountered: %c\n", bit);
                fclose(fp0);
                fclose(fp1);
                freeHuffmanList(huffmanList);
                return NULL;
            }

            if (isLeafNode(currentNode)) {
                fprintf(fp1, "%c", currentNode->character);
                currentNode = root; // 重置为根节点以开始新的解码
            }
        }

        fclose(fp0);
        fclose(fp1);
        freeHuffmanList(huffmanList);
        return fp1;
    }

 FILE* huffman_encode_r(const char*in,const char*out_main,const char*out_help){
       FILE *fp0 = fopen(in, "r+b");
    if (fp0 == NULL) {
        fclose(fp0);
        fprintf(stderr,"输入文件打开错误");
        return NULL;
    }
    FILE *fp1 = fopen(out_main, "w+b");
    if (fp1 == NULL) {
        fclose(fp0);
        fclose(fp1);
        fprintf(stderr,"输出文件1打开错误");
        return NULL;
    }
    FILE *fp2 = fopen(out_help, "w+b");
    if (fp2 == NULL)
       {
           fclose(fp0);
        fclose(fp1);
        fclose(fp2);
        fprintf(stderr,"输出文件2打开错误");
        return NULL;
       }
    fseek(fp0, 0L,SEEK_SET);
    fseek(fp1, 0L, SEEK_SET);
    fseek(fp2, 0L, SEEK_SET);
    char inputChar;
    if (fread(&inputChar, sizeof(char), 1, fp0) == 1) {
        if(inputChar == EOF) {

            return fp1;
        }
    }
    ListNode *huffmanList = createListNode(createNode(inputChar, 1));
    number=1;
    while (fread(&inputChar, sizeof(char), 1, fp0) == 1) {
        processInput(huffmanList, inputChar);
    }
    buildHuffmanTree(&huffmanList);
    int codeArray[100], top = 0;
    huffman_code=(HuffmanCode*)malloc(CHAR*sizeof(HuffmanCode));
    for (int i = 0; i < CHAR; ++i) {
        huffman_code[i].huffmanCode[100] = 0;
    }
    fprintf(fp2, "%d\n", number);
    printCodes(huffmanList->nodeData, codeArray, top, fp2);
    fseek(fp0, 0l, SEEK_SET);
    int count=0;
    unsigned char cunchu=0b11111111;
    while (fread(&inputChar, sizeof(char), 1,fp0) == 1) {

        for(int i=0;i<huffman_code[inputChar+CHAR/2].huffmanCode[100];++i) {
            if(count==8) {
                fwrite(&cunchu, sizeof(unsigned char), 1, fp1);
                count = 0;
                cunchu=0b11111111;
            }

                if(huffman_code[inputChar+CHAR/2].huffmanCode[i]==0)
                { cunchu &= ~(1 << count);}
                else {
                    cunchu |= 1 << count;
                }
                count++;
              //  fwrite(&huffman_code[inputChar+CHAR/2].huffmanCode[i], sizeof(int), 1, stdout);
               // printf("%d", huffman_code[inputChar+CHAR/2].huffmanCode[i]);


        }
    }
    /*if(count)
    fwrite(&cunchu, sizeof(unsigned char), 1, fp1);
    fprintf(fp2, "\n   %d", count);*/
    fprintf(fp2, "                    ");
    for (int i=0;i<count ;i++) {
        fprintf(fp2, "%d ",(cunchu & 1<<i) ?1:0);
    }
    fclose(fp0);
    fclose(fp1);
    fclose(fp2);
    freeHuffmanList(huffmanList);
    return fp2;
}


 FILE* huffman_decode_r(const char*in0,const char*in1,const char*out) {
    FILE* In0= fopen(in0, "r+b");
    if(In0 == NULL) {
        fclose(In0);
        fprintf(stderr,"主输入文件打开错误");
        return NULL;
    }
    FILE*In1= fopen(in1, "r+b");
    if(In1 == NULL) {
        fclose(In0);
        fclose(In1);
        fprintf(stderr,"副输入文件打开错误");
        return In0;
    }

    FILE* Out= fopen(out, "w+b");
    if (Out == NULL) {
        fclose(In0);
        fclose(In1);
        fclose(Out);
        fprintf(stderr,"输出文件打开错误");
        return In1;
    }
    fseek(In0, 0L, SEEK_SET);
    fseek(In1, 0L, SEEK_SET);
    fseek(Out, 0L, SEEK_SET);
    int into=0;
        fscanf(In1, "%d", &into);

    ListNode *huffmanList = NULL;
    for (int i = 0; i < into; ++i) {
        char df;
        while((df = fgetc(In1)) != '\n' && df != EOF ) {

        }
            int c;
        unsigned int d;
        // 假设文件格式是 "字符||频率||字符"，例如 "65||97||A\n"
        fscanf(In1, "%d||%ud||\n", &c, &d);
        Node *newNode = createNode((char)c, d);
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
    unsigned char bit;
    Node *currentNode = root;
   // unsigned char cw = 0;
    while (fread(&bit,sizeof(unsigned char),1,In0) == 1 ) {
        /*if(fread(&cw,sizeof(unsigned char),1,In0) == 1) {
            fseek(In0,-1L,SEEK_SET);
        } else break;*/
      //  putchar('%');
        for(int i=0;i<8;++i) {
            if(bit & 1<<i) {
                currentNode = currentNode->rightChild;
            } else
            currentNode = currentNode->leftChild;

           if( isLeafNode(currentNode)) {
              fwrite(&currentNode->character, sizeof(char), 1, Out);
               currentNode = root;
           }
        }
    }
        fseek(In1, -18L, SEEK_END);
    while (fscanf(In1, "%d", &into) == 1) {
        if(into==1) {
            currentNode = currentNode->rightChild;
        } else
            currentNode = currentNode->leftChild;

        if( isLeafNode(currentNode)) {
            fwrite(&currentNode->character, sizeof(char), 1, Out);
            currentNode = root;
        }
    }


    fclose(In0);
    fclose(In1);
    fclose(Out);
    freeHuffmanList(huffmanList);
    return Out;
}

#endif //HUFFMAN_H
