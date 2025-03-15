
#include "huffman_linux.h"
#include <string.h>
#define WORK_FILE "/huffman_workfile"
int main() {
    char *homeo=getenv("HOME");
    char*workpath=malloc(sizeof(char)*(strlen(homeo)+strlen(WORK_FILE)+1));
    
     strcat(workpath,homeo);
     strcat(workpath,WORK_FILE);
     fputs(workpath,stdout);
     putchar('\n');
    FILE*home=fopen(workpath,"r+b");

    if(home == NULL) {
     //   fclose(home);
        home=fopen(workpath, "w+b");
        if(home!=NULL)
        fclose(home);
        return 1;
    }

    int xuanz;
    char TXT[3][250];
    unsigned long huffman_max;
    fseek(home, 0, SEEK_SET);
   if ( fscanf(home, "huffman_MAX:%20lu\n", &huffman_max) == 0 ) {
       fseek(home, 0, SEEK_SET);
       huffman_max=0L;
   }

        if (huffman_max > Huffman_max ) {
            Huffman_max = huffman_max;
            printf("Huffman_max已使用%lu\n",Huffman_max);
        }

  while (  fscanf(home,"%d",&xuanz ) == 1)
    {
      while (getc(home) != '\n') continue;
        fgets(TXT[0],250,home);
        fgets(TXT[1],250,home);
        fgets(TXT[2],250,home);
        for(int i=0;i<3;i++) {
            int j=0;
            while (TXT[i][j] != '\n' && TXT[i][j] != '\0' && j<250 ) {
                j++;
            }
            TXT[i][j]='\0';
        }
        puts("处理文件名为:\n");
        puts(TXT[0]);
        puts(TXT[1]);
        puts(TXT[2]);
        if (xuanz>0) {
            puts("开始编码");
          if(  huffman_encode_r(TXT[0],TXT[1],TXT[2] ) == NULL ) {
              perror("编码过程错误");
              puts("输入任何字符退出");
              free(workpath);
              getchar();
              fclose(home);
              exit(6);
          }
        }
        else if (xuanz<0) {
            puts("开始解码");
           if ( huffman_decode_r(TXT[0],TXT[1],TXT[2] ) == NULL ) {
               perror("解码过程错误");
               puts("输入任何字符退出");
               free(workpath);
               getchar();
               fclose(home);
               exit(6);
           }
        }
    }

    puts("输入任何字符退出");
    getchar();
    fclose(home);
    exit(0);
}
