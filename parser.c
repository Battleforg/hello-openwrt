#include "listener.h"

struct raw_hotspot_xml_data raw;

//  record  MAC addresses of  known hotspot
char knownHotspotMAC[PACKET_NUMBER][20];
// count the number of records
int records_count = 0;

// add new hotspot record to knownHotspotMAC
int addNewHotspot(struct raw_hotspot_xml_data* raw_pointer)
{
    // first find out if the hotspot has already in record.
    int i;
    for (i = 0; i < records_count && i < PACKET_NUMBER; ++i) {
        // if the hotspot is in record
        if (!strcmp(raw_pointer ->mac, knownHotspotMAC[i])) {
            // do nothing but return 0 means old record is detected
            return 0;
        }   
    }
    // add new hotspot record and record is not full
    if (records_count < PACKET_NUMBER) {
        strcpy(knownHotspotMAC[records_count], raw_pointer->mac);
    }
    // update record count
    records_count++;
    // new record has been added and return 1
    return 1;
}

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)
{
    RADIOTAP_C_HEADER * rHeader = (RADIOTAP_C_HEADER*)packet;
    // calculate radiotap header length
    int l1= rHeader->len[1];
    int l2 = rHeader->len[0];
    int radiotap_len = (l1 << 8) + l2;
    
    // packet len which have not been visited yet
    int len_count = pkthdr->len;
    
    // skip to 802.11frame header
    const IEEE80211BEACON_FRAME * bHeader = (IEEE80211BEACON_FRAME*)(packet + radiotap_len);

    // iterator used in this function
    int i;
     
    // if this frame is a 802.11 beacon frame
    if ((bHeader->frame_control[0]) == BEACON_FRAME &&
        bHeader->frame_control[1] == 0x00) {

        // get time
        // output recieved time to struct RAW_HOTSPOT_XML_DATA raw
        sprintf(raw.recieved_time, "%ld", pkthdr->ts.tv_sec);
        // get signal
        getSignal(rHeader, packet, &raw);
        // get channel
        getChannel(rHeader, packet, &raw);
        // update len_count;
        len_count -= radiotap_len;
        // SOURCE MAC
        // output source mac to struct RAW_HOTSPOT_XML_DATA raw
        sprintf(raw.mac, "%02X-%02X-%02X-%02X-%02X-%02X", bHeader->address2[0], bHeader->address2[1], 
           bHeader->address2[2], bHeader->address2[3], bHeader->address2[4], bHeader->address2[5]);
        // SSID
        if (bHeader -> ssid_tag_length) {
            // get SSID 
            for (i = 0; i < bHeader->ssid_tag_length; ++i) {
                //printf("%c", bHeader->ssid[i]);
                sprintf(raw.ssid + i, "%c", bHeader->ssid[i]);
            }
        }
        else {
            sprintf(raw.ssid, "Broadcast");
        }
      
        ENCRYPTION e;
        e.wpa_version = 0;
        e.group_ciphers = 0;
        e.pair_ciphers = 0;
        e.auth_algs = 0;

        static unsigned char wpa_oui[3] = {0x00, 0x50, 0xf2};
        static unsigned char wpa2_oui[3] = {0x00, 0x0f, 0xac};
        // begin scan data to find rsn and wpa type 
        const u_char * p_encryption = &(bHeader->ssid[bHeader->ssid_tag_length]);
        len_count -= (38 + bHeader->ssid_tag_length);

        while (len_count > 0) {
            // get tag number;
            int tag_num = *p_encryption;
            // move to tag length and get it
            p_encryption +=1;
            len_count -= 1;
            int tag_len = *p_encryption;
            // if tag number is RSN
            if (tag_num == RSN) {
                if (!memcmp(p_encryption + 3, wpa_oui, 3)) {
                    e.wpa_version +=1;
                 }
                else if (!memcmp(p_encryption + 3, wpa2_oui, 3)) {
                     e.wpa_version +=2;
                }

                if (!memcmp(p_encryption + 3, wpa_oui, 3) || !memcmp(p_encryption + 3, wpa2_oui, 3)) {
                    switch(p_encryption[6]) {
                        case 1: e.group_ciphers |= MY_CIPHER_WEP40; break;
                        case 2: e.group_ciphers |= MY_CIPHER_TKIP; break;
                        case 4: e.group_ciphers |= MY_CIPHER_CCMP; break;
                        case 5: e.group_ciphers |= MY_CIPHER_WEP104; break;
                        case 6: /* AES-128-MAC */ break;
                        default: break;
                    }
                 }

            }

            // if tag is VERDOR_SPECIFIC
            else if (tag_num == VERDOR_SPECIFIC) {
                 if (!memcmp(p_encryption + 1, wpa_oui, 3) && p_encryption[4] == 1) {
                    e.wpa_version +=1;
                }
            }
            p_encryption += (tag_len + 1);
            len_count -= (tag_len + 1);
         }
        print_encry(&e, &raw);
        //printf("\n");
        // a pointer to RAW_HOTSPOT_XML_DATA raw
        struct raw_hotspot_xml_data* tmp = &raw;

        // if hotspot ssid is not null(Broadcast) and is a new record
        if(strcmp(tmp->ssid,"Broadcast") != 0 && addNewHotspot(tmp)) {
            save_hotspot(tmp);
        }

        // if not a beacon frame
    } else {
        //printf("----------------------------------------\n");
        //printf("id: %d\n", ++(*id));
        //printf("not beacon frame\n");
        // get time
        //printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec)); 

    }
}

int main()
{
    int status=0;
    int header_type; 
    pcap_t *handle=0;
    char errbuf[PCAP_ERRBUF_SIZE];
    /* linux */
    char *dev=(char *)"wlan0";
    
    /* macbook pro */
    //char* dev=(char *)"en0";

    handle=pcap_create(dev,errbuf); //为抓取器打开一个句柄
    
    if (handle == NULL)  {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 0;
    }
    else {
        printf("Opened device %s\n",dev);
    }
    
    if(pcap_can_set_rfmon(handle)) {    
         //查看是否能设置为监控模式
        //printf("Device %s can be opened in monitor mode\n",dev);
    }
    else {
            //printf("Device %s can't be opened in monitor mode!!!\n",dev);
    }
    
    pcap_set_rfmon(handle,0);   //设置为监控模式
     
    if(pcap_set_rfmon(handle,1)!=0) { 
        fprintf(stderr, "Device %s couldn't be opened in monitor mode\n", dev);
        return 0;
    }
    else {
        printf("Device %s has been opened in monitor mode\n", dev);
    }
    pcap_set_promisc(handle,0);   //不设置混杂模式
    pcap_set_snaplen(handle,65535);   //设置最大捕获包的长度
        
    status=pcap_activate(handle);   //激活
    if(status!=0) {
        pcap_perror(handle,(char*)"pcap error: ");
        return 0;
    }
        
    header_type=pcap_datalink(handle);  //返回链路层的类型
    if(header_type!=DLT_IEEE802_11_RADIO) {
            //printf("Error: incorrect header type - %d",header_type);
            return 0;            
    }
     
    int id = 0;
    /* wait loop until PACKET_NUMBER */
    pcap_loop(handle, PACKET_NUMBER, getPacket, (u_char*)&id);
  
    pcap_close(handle);
    return 0;
}
