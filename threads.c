#include <pthread.h>
#include "parser.h"
char *zip_name[20];

pthread_t thread[2];
// pthread_mutex_t mut;

void name_ini(){
    int i;
    for(i = 0; i < 20; i++){
        strcat(*zip_name[i],"/tmp/group2/zip/data");
        strcat(*zip_name[i],(char)i);
        strcat(*zip_name[i],".zip");
    }
}

void thread1(){
    while(1){
        writeIndex();
        myPcapCatchAndAnaly();
    }
}

void thread2(){
    int estab_count = 0;
    int upload_count = 0;
    char tmp[50];
    while(1){
        if(zip_num > 0){
            rename("/tmp/group2/zip/data.zip",*zip_name[estab_count]);
            system("rm - rf /tmp/group2/zip/data.zip");
            zip_num--;
            estab_count++;
            if(estab_count == 20){
                estab_count = 0;
                if(estab_count == upload _count){
                    break;
                }
            }
            if(estab_count - upload_count == -1){
                break;
            }
        }
        if(0 = upload(&zip_name[upload_count]){
            strcpy(tmp,"");
            strcat(tmp,"rm -rf ");
            strcat(tmp,*zip_name[upload_count]);
            system(tmp[50]);
            upload_count++;
            if(upload_count == 20){
                upload_count = 0;
            }
        }else{
            continue;
        }
    }
}

void thread_create(){
    int temp;
    memset(&thread, 0, sizeof(thread)); 
    if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0) 
        printf("线程1创建失败!\n");
    else
        printf("线程1被创建\n");
    if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0) 
        printf("线程2创建失败");
    else
        printf("线程2被创建\n");
}
}

int main() {
    zip_num = 0;
    int comd;
    const char *origin = "http://jxuao.me/upload?user=group2&filename=data.zip";
    strcpy(urls,origin);

     //openwrt
    folder_create("/tmp/group2");
    folder_create("/tmp/group2/data");
    folder_create("/tmp/group2/data/hotspot");
    folder_create("/tmp/group2/data/station");
    folder_create("/tmp/group2/zip");
    remove_dir("/tmp/group2/data/hotspot");
    remove_dir("/tmp/group2/data/station");
    remove_dir("/tmp/group2/zip");

    //test
    // folder_create("data");
    // folder_create("data/hotspot");
    // folder_create("data/station");
    // folder_create("zip");
    // remove_dir("data/hotspot");
    // remove_dir("data/station");
    // remove_dir("zip");
    //write the index file for zip file

    printf("Please input a number:\n");
    printf("1.run!\n");
    printf("2.set upload information\n");
    while(scanf("%d",&comd) == 1){
        switch(comd){
            case 1:
                    thread_create();
                    break;
            case 2:
                    seturls();
        }
    }

    return 0;
}
