#pragma once
#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdio.h>
#include <stdlib.h>
#define CHAR 256
#define end_mark "\n|\n|\n"
const long Huffman_max = 102400L;
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
inline ListNode* createListNode(Node* nodeData);
inline  Node *createNode(char character, unsigned int frequency);
inline void swapNodes(ListNode *nodeA, ListNode *nodeB);
inline int compareFrequencies(const ListNode *nodeA, const ListNode *nodeB);
inline void sortList(ListNode *list);
inline void combineNodes(ListNode *list);
inline void buildHuffmanTree(ListNode **list);
inline ListNode* findNode(ListNode *list, char character);
inline void processInput(ListNode *list, char character,int *number);
inline void freeHuffmanTree(Node* node);
inline void freeHuffmanList(ListNode *list);
inline int isLeafNode(const Node* node);
inline void printCodes(const Node* root, int*, int top, FILE* file,HuffmanCode* );
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
void processInput(ListNode *list, char character,int*kindnumber) {
    ListNode *foundNode = findNode(list, character);
    if (foundNode == NULL) {
        ListNode *endNode = list;
        while (endNode->nextNode != NULL) {
            endNode = endNode->nextNode;
        }
        endNode->nextNode = createListNode(createNode(character, 1));
        (*kindnumber)++;

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
void printCodes(const Node* root, int arr[], int top, FILE *file,HuffmanCode* huffman_code) {

    if (top >= 100 || root==NULL||huffman_code==NULL) {
        return;
    }
    if (root->leftChild) {
        arr[top] = 0;
        printCodes(root->leftChild, arr, top + 1, file,huffman_code);
    }
    if (root->rightChild) {
        arr[top] = 1;
        printCodes(root->rightChild, arr, top + 1, file,huffman_code);
    }
    if (isLeafNode(root)) {
       fprintf(file, "%15x|%15u\n", root->character,root->frequency);
        if (top==0) {
            huffman_code[root->character+CHAR/2].huffmanCode[0]=0;
            huffman_code[root->character+CHAR/2].huffmanCode[100]=1;
        } else {
            for (int i = 0; i < top; ++i) {
                huffman_code[root->character+CHAR/2].huffmanCode[i]=arr[i];
            }
            huffman_code[root->character+CHAR/2].huffmanCode[100]=top;
        }
    }
}
 FILE* huffman_encode_r(const char*in,const char*out_main,const char*out_help)
{
    FILE *fp0 = fopen(in, "r+b");
    FILE *fp1 = fopen(out_main, "w+b");
    FILE *fp2 = fopen(out_help, "w+b");
    if (fp0==NULL||fp1==NULL||fp2==NULL)
       {
        fclose(fp0);
        fclose(fp1);
        fclose(fp2);
        fprintf(stderr,"解码文件打开错误");
        return NULL;
       }

    char inputChar;/*存储文件中的一位*/

    int count=0;//编码过程中检测编码了多少位
    unsigned char cunchu=0b11111111;//用于暂时存储编码
    while(1) {
        if (fread(&inputChar, sizeof(char), 1, fp0) == 0 || feof(fp0))  //通过尝试读取一个字节
            {break;}                                                                    //检测文件是否为空
        ListNode *huffmanList = createListNode(createNode(inputChar, 1));//开始创建Huffman树
        long total=1L;//记录输入字符总数，判断是否需要重新开始
       int number=1;//记录输入字符种类，输入到辅助解码文件
        while (total != Huffman_max)
            {/*当总数超过规定数量或读到文件尾时停止*/
           if ( fread(&inputChar, sizeof(char), 1, fp0) != 1 || feof(fp0))
               break;
            processInput(huffmanList, inputChar,&number);
            total++;
        }
        buildHuffmanTree(&huffmanList);//将Huffman树构建出来
        HuffmanCode* huffman_code=(HuffmanCode*)malloc(CHAR*sizeof(HuffmanCode));
        for (int i = 0; i < CHAR; ++i) {/*存储Huffman编码并将频率输出到辅助解码文件，帮助解码*/
            huffman_code[i].huffmanCode[100] = 0;
        }
        fprintf(fp2, end_mark);
        fprintf(fp2, "%d\n", number);
        int codeArray[100], top = 0;
        printCodes(huffmanList->nodeData, codeArray, top, fp2,huffman_code);
        fseek(fp0, -total, SEEK_CUR);
        total=0L;
        while ( total != Huffman_max) {
            if (fread(&inputChar, sizeof(char), 1,fp0) == 0||feof(fp0))
            {break;}
               total++;
            for(int i=0;i<huffman_code[inputChar+CHAR/2].huffmanCode[100];++i) {
                if(huffman_code[inputChar+CHAR/2].huffmanCode[i]==0 )
                    { cunchu &= ~(1 << count);}else{cunchu |= 1 << count;}
                count++;if(count==8) {
                    fwrite(&cunchu, sizeof(unsigned char), 1, fp1);
                    count = 0;cunchu=0b11111111;
                }
            }
        }
        freeHuffmanList(huffmanList);
        huffmanList=NULL;
        free(huffman_code);
        huffman_code=NULL;
    }
   fprintf(fp2, "" end_mark "%d  ", count)==0;
    for (int i=0;i<count ;i++) {
        fprintf(fp2, "%d ",cunchu & 1<<i ? 1 : 0);
    }
    fclose(fp0);
    fclose(fp1);
    fclose(fp2);
    return fp2;
}

 FILE* huffman_decode_r(const char*in0,const char*in1,const char*out) {
    FILE* In0= fopen(in0, "r+b");/*编码后文件存储的位置*/
    FILE*In1= fopen(in1, "r+b");/*存储字节对应频率的文件*/
    FILE* Out= fopen(out, "w+b");
    if (In0 ==NULL || In1==NULL || Out == NULL) {
        fclose(In0);
        fclose(In1);
        fclose(Out);
        fprintf(stderr,"文件打开错误");
        return NULL;
    }
    unsigned char bit;//存储读取的字节
    long total=0L;//用于记录解码字节的数量
    ListNode *huffmanList = NULL;
    Node *root=NULL;//huffman树树根
    Node *currentNode;;
    int into=0;
    fscanf(In1, end_mark "%d", &into);
    for (int i = 0; i < into; ++i){
        int c;
        unsigned int d;
        fscanf(In1, "%15x|%15u|\n", &c, &d);
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
    currentNode = root;
      while(fread(&bit,sizeof(unsigned char),1,In0) == 1 && !feof(In0))
      {
            for(int i=0;i<8 && currentNode!=NULL;++i) {
                if(bit & 1<<i) {
                    currentNode = currentNode->rightChild;
                } else
                    currentNode = currentNode->leftChild;

                if( isLeafNode(currentNode)) {
                    fwrite(&currentNode->character, sizeof(char), 1, Out);
                    currentNode = root;
                    total++;
                    if (total == Huffman_max || currentNode==NULL ) {
                        total=0L;
                        freeHuffmanList(huffmanList);
                        huffmanList = NULL;
                        fscanf(In1, end_mark "%d", &into);
                        for (int ij = 0; ij < into; ++ij){
                            int c;
                            unsigned int d;
                            fscanf(In1, "%15x|%15u|\n", &c, &d);
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
                        currentNode = root;
                    }
                }
            }
    }

    int lin;
    fscanf(In1, end_mark "%d " , &lin);
   // int into;
    for(int i=0;i<lin && currentNode!=NULL;++i)
    {
        fscanf(In1, "%d" , &into);
        if(into) {
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

    return Out;

}
#endif //HUFFMAN_H
int main() {
    huffman_encode_r("D:\\1.txt","D:\\test.txt","D:\\0.txt");
    huffman_decode_r("D:\\test.txt","D:\\0.txt","D:\\0hjk.txt");

}
