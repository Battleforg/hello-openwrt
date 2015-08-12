#include "parser.h"
void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet) {
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

    // if(pcap_can_set_rfmon(handle)) {
    //      //查看是否能设置为监控模式
    //     printf("Device %s can be opened in monitor mode\n",dev);
    // }
    // else {
    //     printf("Device %s can't be opened in monitor mode!!!\n",dev);
    // }

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
    //loop
    printf("Get Packets Start!\n");
    pcap_loop(handle,1, getPacket, (u_char*)&id);
    pcap_close(handle);
    return 0;
}

