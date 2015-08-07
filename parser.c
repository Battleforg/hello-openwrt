// #include "listener.h"
#include "saveXML.h"
#include "delete.h"
#include "upload.h"

int GAP =  5;
struct raw_hotspot_xml_data raw;
struct raw_sta_xml_data raw_sta;

long globalSecond;

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet) {
    long seconds = time((time_t*)NULL);
    // upload interval is 30s
    if (seconds - globalSecond > GAP) {
        refreshAndUpload();
        globalSecond = seconds;
        return;
    }
    RADIOTAP_C_HEADER * rHeader = (RADIOTAP_C_HEADER*)packet;
    // calculate radiotap header length
    int l1= rHeader->len[1];
    int l2 = rHeader->len[0];
    int radiotap_len = (l1 << 8) + l2;
    // skip to 802.11frame header
    const IEEE80211BEACON_FRAME * bHeader = (IEEE80211BEACON_FRAME*)(packet + radiotap_len);
    // if this frame is a 802.11 beacon frame
    if ((bHeader->frame_control[0]) == BEACON_FRAME && bHeader->frame_control[1] == 0x00) {
        if(fillHotspotData(rHeader, packet, &raw, pkthdr)){
            save_hotspot(&raw);
        }
    // if it some frame like RTS transmitted by station
    // fillStaData(rHeader, packet, &raw_sta, pkthdr);
    // if station is a new record
    } else if (fillStaData(rHeader, packet, &raw_sta, pkthdr)) {
        save_sta(&raw_sta);
    }
}

int myPcapCatchAndAnaly() {
    int status=0;
    int header_type;
    pcap_t *handle=0;
    char errbuf[PCAP_ERRBUF_SIZE];
    /* openwrt && linux */
    char *dev=(char *)"wlan0";

    /* mac os */
    //test
    // char* dev=(char *)"en0";

    handle=pcap_create(dev,errbuf); //为抓取器打开一个句柄

    if (handle == NULL)  {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 0;
    }
    else {
        printf("Opened device %s\n",dev);
    }

    // 由于在该路由器测试时，发现在该openwrt系统上不支持libpcap设置monitor模式，在激活的时候会产生错误
    // 将采用手动设置并检测网卡是否为monitor模式

    if(pcap_can_set_rfmon(handle)) {
         //查看是否能设置为监控模式
        //printf("Device %s can be opened in monitor mode\n",dev);
    }
    else {
        //printf("Device %s can't be opened in monitor mode!!!\n",dev);
    }

    // 若是mac os系统，则可以支持
    // test
    // if(pcap_set_rfmon(handle,1)!=0) {
    //     fprintf(stderr, "Device %s couldn't be opened in monitor mode\n", dev);
    //     return 0;
    // } else {
    //     printf("Device %s has been opened in monitor mode\n", dev);
    // }
    pcap_set_promisc(handle,0);   //不设置混杂模式
    pcap_set_snaplen(handle,65535);   //设置最大捕获包的长度
    status=pcap_activate(handle);   //激活

    if(status!=0) {
        pcap_perror(handle,(char*)"pcap error: ");
        return 0;
    }

    header_type=pcap_datalink(handle);  //返回链路层的类型
    if(header_type!=DLT_IEEE802_11_RADIO) {
        printf("Error: incorrect header type - %d",header_type);
        return 0;
    }

    int id = 0;
    globalSecond = time((time_t*)NULL);
    //loop
    printf("Get Packets Start!\n");
    pcap_loop(handle, -1, getPacket, (u_char*)&id);
    pcap_close(handle);
    return 0;
}

void folder_create( const char * foldername){
    if(access(foldername,F_OK) != 0){
        mkdir(foldername,0777);
    }
}

int main() {
    int comd;
    const char *origin = "http://jxuao.me/upload?user=group2&filename=data.zip";
    strcpy(urls,origin);

    printf("Please input a number:\n");
    printf("1.run!\n");
    printf("2.set upload information\n");
    while(scanf("%d",&comd) == 1){
        switch(comd){
            case 1:
                    writeIndex();
                    myPcapCatchAndAnaly();
                    break;
            case 2:
                    seturls();
        }
    }
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
    return 0;
}
