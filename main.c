#include <stdio.h>
#include <stdlib.h>
 unsigned int charnumber=0;
typedef struct node {
    char data;
    unsigned int fred;
    struct node *right;
    struct node *left;
} node;

typedef struct list {
    node *data;
    struct list *next;
} list;
    typedef struct {
        char data;
        int huffman_code[100];
    } Huffmancode;
list* create_list(node*data);
node *newnode(char data,unsigned int fred);
list* insert_node( list *list0, node *data);
void swap_list(list *list0 ,list *temp);
list* insert_node( list *list0, node *data);
int compire_fred(list *temp,list *list0) ;
void sort_list(list *list1);
void extract_node(list *list2);
void hullman(list *list0) ;
list* list_find(list *list0,char q);
void see_input (list *list1,char into);
void huffman_free_(node* temp);
void huffman_free(list *list0) ;
int isLeaf(const node* root);
void printCodes(const node* root, int arr[], int top,FILE*);
//创建一个链表节点
list* create_list(node*data) {
    list* new_list = (list*)malloc(sizeof(list));
    new_list->data = data;
    new_list->next = NULL;
    return new_list;
}

//创建一个新的数据二叉节点
node *newnode(char data,unsigned int fred) {
    node *temp = (node *)malloc(sizeof(node));
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    temp->fred = fred;
    return temp;
}

//交换两个链表节点的数据
void swap_list(list *list0 ,list *temp) {
    node *temp_ = list0->data;
    list0->data = temp->data;
    temp->data = temp_;
}

//创建一个新的链表节点并存储二叉节点的指针
list* insert_node( list *list0, node *data) {
    while(list0->next !=NULL) {
        list0 = list0->next;
    }
    list0->next = create_list(data);
    list0->next->next = NULL;
    return list0;
}

int compire_fred(list *temp,list *list0) {
    return temp->data->fred > list0->data->fred  ||
                    ( temp->data->fred == list0->data->fred &&
                        temp->data->data > list0->data->data) ;
}

//将链表排序
void sort_list(list *list1) {
    //putchar('$');
    list *list0 = list1;
    if (list0 == NULL) {
        return;
    }
    {
        while (list0  != NULL) {
            list* temp = list0->next;
            while (temp!= NULL   ) {
                if( compire_fred(temp,list0) ) {
                    swap_list(temp,list0);
                    }
                temp = temp->next;
            }
            list0 = list0->next;
        }
    }


   //rintf("sort_list end\n");
}
//
void extract_node(list *list2) {
//    putchar('%');
    list *list1 = list2;
    if (list1 == NULL) {
        return;
    }

    // sort_list(list1);
    list *list0 = list1;
    while (list0->next != NULL && list0->next->next != NULL)
        list0 = list0->next;
     if (list0->next != NULL)  {
        node *new_node = newnode('\0', list0->data->fred + list0->next->data->fred);
        new_node->left = list0->data;
        new_node->right = list0->next->data;
   //       putchar('&');
        list0->next->data=NULL;
        free(list0->next);
        list0->next = NULL;
        list0->data = new_node;

         list* list3 = list1;
         while (list3->next != list0) {
             list3 = list3->next;
         }

         if (compire_fred(list0,list1)) {
             swap_list(list0,list1);
         } else if (compire_fred(list3,list0)) {
             return;
         }

            while(list1!=list3) {
                if (compire_fred(list1,list0) && compire_fred(list0,list1->next) ){
                   break;
                }
                list1 = list1->next;
            }

         list0->next = list1->next  ;
         list1->next = list0;
         list3->next = NULL;

}
}

void hullman(list *list0) {
    sort_list(list0);
    if (list0 == NULL) {return;}
    while(  list0->next != NULL ) {
        //  putchar('%');
        extract_node(list0);
    }
}
list* list_find(list *list0,char q) {
    while(list0!=NULL && list0->data->data != q) {
        list0 = list0->next;
    }
    return list0;
}
//处理输入字符
void see_input (list *list1,char into) {
list* list0 = list1;
    list0=list_find(list0,into);
    if (list0 == NULL) {
        list* list2 = list1;
        while( list2->next != NULL ) {
            list2 = list2->next;
        }
        list2->next = create_list(newnode(into,1));
        charnumber++;
    } else
    if (list0->data->data ==into) {
        list0->data->fred++;
    }

}

void huffman_free_(node* temp) {
    if (temp == NULL) {return;}
    {
        node* temp0 = temp->left;
        node* temp1 = temp->right;
        free(temp);
        huffman_free_(temp0);
        huffman_free_(temp1);
    }

}

void huffman_free(list *list0) {
    if (list0 == NULL) {return;}
  {
        node* temp = list0->data->right;
        node* temp1 = list0->data->left;
        free(list0->data);
        huffman_free_(temp);
        huffman_free_(temp1);
    }
}/*
void huffman_display(node *data0) {
    / int huffman[100];
      if (data0 == NULL) {return;}
      if(data0->right == NULL && data0->left == NULL) {
          printf("0 %c" ,data0->data);
      } else if (data0->right!=NULL) {

      }
}

*/
int isLeaf(const node* root) {
    return !root->left && !root->right; // 如果没有左右子节点，则是叶子节点
}

void printCodes(const node* root, int arr[], int top,FILE *fp1) {
    if(top>=100)
        exit(0);
    if (root->left) { // 如果存在左子节点，标记为0并递归
        arr[top] = 0;
        printCodes(root->left, arr, top + 1,fp1);
    }
    if (root->right) { // 如果存在右子节点，标记为1并递归
        arr[top] = 1;
        printCodes(root->right, arr, top + 1,fp1);
    }
    if (isLeaf(root)) { // 如果是叶子节点，打印字符及其编码
        fprintf(fp1,"%c: ", root->data);
        for (int i = 0; i < top; ++i)
            fprintf(fp1,"%d", arr[i]);
        putc('\n',fp1);

    }
}



int main(void) {
    char input[100];
    printf("Enter the string:\n");
    scanf("%s",input);
     FILE*fp0= fopen(input,"r+b");
    if (fp0 == NULL) {exit(0);}
    scanf("%s",input);
    FILE*fp1 = fopen(input,"w+b");
    char ch=getc(fp0);
    if(ch==EOF)
        return 0;

    list* er = create_list(newnode(ch,1));
    charnumber++;
    while((ch = getc(fp0))!=EOF) {
        see_input(er,ch);
    }
      // huffman_display(er->data);
        hullman(er);
    int arr[100], top = 0; // 存储霍夫曼编码的数组
    printCodes(er->data, arr, top,fp1); // 打印霍夫曼编码
    fclose(fp0);
    fclose(fp1);
    huffman_free(er);
    free(er);
}
