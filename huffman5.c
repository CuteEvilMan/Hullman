
#pragma once

#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdlib.h>
#define CHAR 256
#define Huffman_max 30
#define end_mark "\n||\n||\n||\n||\n||\n"
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
       fprintf(file, "%15x|", root->character);
        fprintf(file, "%15u\n", root->frequency);
        for (int i = 0; i < top; ++i) {
                huffman_code[root->character+CHAR/2].huffmanCode[i]=arr[i];
                huffman_code[root->character+CHAR/2].huffmanCode[100]=top;
        }
    }
}
 FILE* huffman_encode_r(const char*in,const char*out_main,const char*out_help){

       FILE *fp0 = fopen(in, "r+b");
    if (fp0 == NULL) {
        fclose(fp0);
        fprintf(stderr,"编码输入文件打开错误");
        return NULL;
    }
    FILE *fp1 = fopen(out_main, "w+b");
    if (fp1 == NULL) {
        fclose(fp0);
        fclose(fp1);
        fprintf(stderr,"编码输出文件1打开错误");
        return NULL;
    }
    FILE *fp2 = fopen(out_help, "w+b");
    if (fp2 == NULL)
       {
           fclose(fp0);
        fclose(fp1);
        fclose(fp2);
        fprintf(stderr,"编码输出文件2打开错误");
        return NULL;
       }
    fseek(fp0, 0L,SEEK_SET);
    fseek(fp1, 0L, SEEK_SET);
    fseek(fp2, 0L, SEEK_SET);
    char inputChar;
    int count=0;
    unsigned char cunchu=0b11111111;
    while(1) {
        long total=1L;
        if (fread(&inputChar, sizeof(char), 1, fp0) == 0 || feof(fp0)) {

                fclose(fp0);
                fclose(fp1);
                fclose(fp2);

                break;

        }
        ListNode *huffmanList = createListNode(createNode(inputChar, 1));
        number=1;
        while ( total != Huffman_max    )
            {
           if ( fread(&inputChar, sizeof(char), 1, fp0) == 0)
               break;
            processInput(huffmanList, inputChar);
            total++;
        }
        buildHuffmanTree(&huffmanList);
        int codeArray[100], top = 0;
        huffman_code=(HuffmanCode*)malloc(CHAR*sizeof(HuffmanCode));
        for (int i = 0; i < CHAR; ++i) {
            huffman_code[i].huffmanCode[100] = 0;
        }
        fprintf(fp2, end_mark);
        fprintf(fp2, "%d\n", number);
        printCodes(huffmanList->nodeData, codeArray, top, fp2);

        fseek(fp0, -total, SEEK_CUR);


        total=0L;
        while ( total != Huffman_max)
            {
           if ( fread(&inputChar, sizeof(char), 1,fp0) == 0 )
            break;
               total++;
            for(int i=0;i<huffman_code[inputChar+CHAR/2].huffmanCode[100];++i) {

                if(huffman_code[inputChar+CHAR/2].huffmanCode[i]==0 )
                    { cunchu &= ~(1 << count);}
                else
                    {cunchu |= 1 << count;}
                count++;

                if(count==8) {
                    fwrite(&cunchu, sizeof(unsigned char), 1, fp1);
                    count = 0;
                    cunchu=0b11111111;
                }
            }

        }


        freeHuffmanList(huffmanList);
        huffmanList=NULL;
        free(huffman_code);
        huffman_code=NULL;

    }

    fprintf(fp2, end_mark "%d  ", count);
    for (int i=0;i<count ;i++) {
        fprintf(fp2, "%d ",cunchu & 1<<i ? 1 : 0);
    }
    return fp2;
}


 FILE* huffman_decode_r(const char*in0,const char*in1,const char*out) {

    FILE* In0= fopen(in0, "r+b");
    FILE*In1= fopen(in1, "r+b");
    FILE* Out= fopen(out, "w+b");

    if (In0 ==NULL || In1==NULL || Out == NULL) {
        fclose(In0);
        fclose(In1);
        fclose(Out);
        fprintf(stderr,"文件打开错误");
        return NULL;
    }
    unsigned char bit;
    long total=Huffman_max;
    ListNode *huffmanList = NULL;
    Node *root=NULL;
    Node *currentNode = root;
      while(fread(&bit,sizeof(unsigned char),1,In0) == 1 )
      {
          if (total == Huffman_max) {
              total=0L;
              freeHuffmanList(huffmanList);
              huffmanList = NULL;
              int into=0;
              fscanf(In1, end_mark "%d", &into);


              for (int i = 0; i < into; ++i){
                  int c;
                  unsigned int d;
                  fscanf(In1, "%15x|%15u\n", &c, &d);
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
              root = huffmanList->nodeData;

          }
          currentNode = root;
          if (currentNode == NULL) {
              break;
          }
            for(int i=0;i<8;++i) {
                if(bit & 1<<i) {
                    currentNode = currentNode->rightChild;
                } else
                    currentNode = currentNode->leftChild;

                if( isLeafNode(currentNode)) {
                    fwrite(&currentNode->character, sizeof(char), 1, Out);
                    currentNode = root;
                    total++;
                }
            }



    }
    int lin;
    fscanf(In1, end_mark "%d " , &lin);
    int into;
    for(int i=0;i<lin;++i)
    {
        if (currentNode == NULL) {
            break;
        }
        fscanf(In1, "%d" , &into);
        if(into) {
            currentNode = currentNode->rightChild;
        } else
            currentNode = currentNode->leftChild;

        if( isLeafNode(currentNode)) {
            fwrite(&currentNode->character, sizeof(char), 1, Out);
            currentNode = root;
            total++;
        }
    }
    fclose(In0);
    fclose(In1);
    fclose(Out);

    return Out;

}

#endif //HUFFMAN_H
int main() {

    huffman_encode_r("D:\\1.txt","D:\\test.txt","D:\\0.txt");
    huffman_decode_r("D:\\test.txt","D:\\0.txt","D:\\0hjk.txt");

}

