#include <pthread.h>
#include "parser.h"
char* zip_name[20];

pthread_t thd1, thd2;

void *thread1(void *arg) {
    while(1) {
        writeIndex();
        myPcapCatchAndAnaly();
    }
    return NULL;
}

void *thread2(void *arg) {
    //容量 size
    int size = 20;
    //首先确定文件名称
    for (int i = 0; i < size; i++) {
        char name[50] = "/tmp/group2/zip/data";
        char str[3];
        sprintf(str,"%02d",i);
        strcat(name,str);
        strcat(name,".zip");
        zip_name[i] = name;
        // printf("%s\n", zip_name[i]);
    }
    //记录当前打包和上传的文件数量
    int estab_count = 0;
    int upload_count = 0;
    char* dataname = "/tmp/group2/zip/data.zip";
    while(1) {
        printf("%d\n",access(dataname,F_OK));
        if (!access(dataname,F_OK)) {
            printf("%s\n",zip_name[estab_count]);
            if (rename(dataname, zip_name[estab_count]) != 0) {
                perror("rename");
            }

            printf("!!!!!!!!!\n");
            estab_count++;
            if (estab_count == 20) {
                estab_count = 0;
                if(estab_count == upload_count) {
                    break;
                }
            }
        }
        printf("%d\n",upload(zip_name[upload_count]));
        char tmp[50];
        if (upload(zip_name[upload_count]) == 0) {
            strcpy(tmp,"");
            strcat(tmp,"rm -rf ");
            strcat(tmp,zip_name[upload_count]);
            system(tmp);
            upload_count++;
            if(upload_count == 20) {
                upload_count = 0;
            }
        } else {
            continue;
        }
    }
    return NULL;
}

void thread_create(int a){
    int temp;
    if (a == 1) {
        if((temp = pthread_create(&thd1, NULL, thread1, NULL)) == 0) {
            printf("线程1被创建\n");
        } else {
            printf("线程1创建失败!\n");
        }
    } else {
        if((temp = pthread_create(&thd2, NULL, thread2, NULL)) == 0) {
            printf("线程2被创建\n");
        } else {
            printf("线程2创建失败\n");
        }
    }
}



int main() {
    GAP = 5;
    int comd;
    const char *origin = "http://jxuao.me/upload?user=group2&filename=data.zip";
    strcpy(urls,origin);

    //创建文件夹
    folder_create("/tmp/group2");
    folder_create("/tmp/group2/data");
    folder_create("/tmp/group2/data/hotspot");
    folder_create("/tmp/group2/data/station");
    folder_create("/tmp/group2/zip");
    // remove_dir("/tmp/group2/data/hotspot");
    // remove_dir("/tmp/group2/data/station");
    // remove_dir("/tmp/group2/zip");

    //write the index file for zip file

    printf("Please input a number:\n");
    printf("1&2.run!\n");
    printf("3.set upload information\n");
    while(scanf("%d",&comd) == 1){
        switch(comd){
            case 1:
                    thread_create(1);
                    break;
            case 2:
                    thread_create(2);
                    break;
            case 3:
                    seturls();
                    break;
            default:
                    break;
        }
    }
    return 0;
}
