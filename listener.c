#include "listener.h"

// get SIGNAL in dBm
void getSignal(const RADIOTAP_C_HEADER *rHeader, const u_char * packet, RAW_HOTSPOT_XML_DATA* raw_pointer)
{
    // skip constant header
    const u_char * currentPos = (u_char*)(packet + 4);
    
    int signal_count = 0;
    int present_count = 0;
    while(currentPos[3] &IEEE80211_RADIOTAP_EXT) {
        if (currentPos[0] & IEEE80211_RADIOTAP_DBM_ANTSIGNAL) {
            signal_count++;
        }
        currentPos += 4;
        present_count++;
    }

    // last present should be added
    if (currentPos[0] & IEEE80211_RADIOTAP_DBM_ANTSIGNAL) {
        signal_count++;
    }
    currentPos += 4;
    present_count++;

    // after the steps above, we get total amount of present
    if ((rHeader->present[0] & IEEE80211_RADIOTAP_DBM_ANTSIGNAL) && (signal_count > 0)) { 
        // how many bit we should shift due to TSFT, FLAGS, RATE, CHANNEL and FHSS
        int shift = 0;
        int i;
        u_char s;
        int temp_signal = 0;
        for (i = 0; i < present_count; ++i) {
            if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_TSFT) {
                /*  has Time Synchronization Function timer  */
                shift += 8;
            }
            if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_FLAGS) {
                /*  has flags */
                shift += 1;
            }

            if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_RATE) {
                /*  has data rate */
                shift += 1;
            }

            if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_CHANNEL) {
                /*  has channel information */
                shift += 4;
            }

             if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_FHSS) {
                /*  has FHSS */
                shift += 2;
            }

            if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_DBM_ANTSIGNAL) {
                /* code */
                s = currentPos[shift];
                // do not opeartion and add 1
                s = (~s) + 1;
                temp_signal += s;
            
                shift += 1;
            }

            if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_DBM_ANTNOISE) {
                /* has noise */
                shift += 1;
            }

            if (rHeader->present[4 * i] & IEEE80211_RADIOTAP_LOCK_QUALITY) {
                /* has lock quality */
                shift += 2;
            }

            if (rHeader->present[4 * i + 1] & IEEE80211_RADIOTAP_ANTENNA) {
                /* has antenna */
                shift += 1;
            }

            if (rHeader->present[4 * i + 1] & IEEE80211_RADIOTAP_RX_FLAGS) {
                /* has RX flags */
                if ((16 - shift) == 1) {
                    // if  RX alignment does not meet requirement(2)
                    shift += 1;
                }
                shift += 2;
            }

            if (rHeader->present[4*i + 2] & HT_INFO)
            {
                shift += 3;
            }
            currentPos += shift ;
            shift = 0;
        }
        
        temp_signal /= signal_count;
        temp_signal *= -1;
        // output rssi to struct RAW_HOTSPOT_XML_DATA  pointer raw_pointer
        raw_pointer->rssi = temp_signal;
    }
}

// get channel
void getChannel(const RADIOTAP_C_HEADER *rHeader,const u_char * packet, RAW_HOTSPOT_XML_DATA* raw_pointer)
{
    // skip constant header
    const u_char * currentPos  = (u_char*)(packet + 4);
    
    int present_count = 0;
    while(currentPos[3] & IEEE80211_RADIOTAP_EXT) {
        currentPos += 4;
        present_count++;
    }

    // last present should be added
    currentPos += 4;
    present_count++;

    int shift = 0;
    if (rHeader->present[0] & IEEE80211_RADIOTAP_TSFT) {
        /*  has Time Synchronization Function timer  */
        shift += 8;
    }
    if (rHeader->present[0] & IEEE80211_RADIOTAP_FLAGS) {
        /*  has flags */
        shift += 1;
    }

    if (rHeader->present[0] & IEEE80211_RADIOTAP_RATE) {
        /*  has data rate */
        shift += 1;
    }

    if (rHeader->present[0] & IEEE80211_RADIOTAP_CHANNEL) {
        /* calculate channel type */
        int t1 = currentPos[shift+1];
        int t2 = currentPos[shift];

        // output channel to struct RAW_HOTSPOT_XML_DATA raw
        switch((t1<<8) + t2) {
            case CHANNEL_1:
            raw_pointer->channel = 1;
            break;

            case CHANNEL_2:
            raw_pointer->channel  = 2;
            break;

            case CHANNEL_3:
            raw_pointer->channel  = 3;
             break;

            case CHANNEL_4:
            raw_pointer->channel  = 4;
            break;

            case CHANNEL_5:
            raw_pointer->channel  = 5;
            break;

            case CHANNEL_6:
            raw_pointer->channel  = 6;
            break;

            case CHANNEL_7:
            raw_pointer->channel  = 7;
            break;

            case CHANNEL_8:
            raw_pointer->channel  = 8;
            break;

            case CHANNEL_9:
            raw_pointer->channel  = 9;
            break;

            case CHANNEL_10:
            raw_pointer->channel  = 10;
            break;

            case CHANNEL_11:
            raw_pointer->channel  = 11;
            break;

            case CHANNEL_12:
            raw_pointer->channel  = 12;
            break;

            case CHANNEL_13:
            raw_pointer->channel  = 13;
            break;

            case CHANNEL_14:
            raw_pointer->channel  = 14;
            break;

            default:
            break;
        }
    }
}

// print encryption informaiton
void print_encry(ENCRYPTION * e, RAW_HOTSPOT_XML_DATA* raw_pointer)
{
    // unknown encryption type
    if (!e) {
        // output encryption type to struct RAW_HOTSPOT_XML_DATA pointed by raw_pointer
        snprintf(raw_pointer->encryption_type, sizeof(raw_pointer->encryption_type),"unknown");
    }
    // wep, wpa, wpa2 or mixed
    else {
        // wep
        if (!e->wpa_version) {
            // output encryption type to struct RAW_HOTSPOT_XML_DATA pointed by raw_pointer
            sprintf(raw_pointer->encryption_type, "WEP");
        }
        // wpa
        else {
            int pos = 0;
            switch(e->wpa_version) {
                case 3:
                pos = sprintf(raw_pointer->encryption_type, "mixed WPA/WPA2"); 
                break;

                case 2:
                pos = sprintf(raw_pointer->encryption_type, "WPA2"); 
                break;

                case 1: 
                pos = sprintf(raw_pointer->encryption_type, "WPA"); 
                break;
                default: 
                break;
            }

            // group ciphers
            switch(e->group_ciphers) {
                case MY_CIPHER_NONE:
                sprintf(raw_pointer->encryption_type + pos, "NONE");  
                break;

                case MY_CIPHER_WEP40:
                sprintf(raw_pointer->encryption_type + pos, "WEP40"); 
                break;

                case MY_CIPHER_TKIP:
                sprintf(raw_pointer->encryption_type + pos, "TKIP"); 
                break;

                case MY_CIPHER_WRAP:
                sprintf(raw_pointer->encryption_type + pos, "WRAP"); 
                break;

                case MY_CIPHER_CCMP:
                sprintf(raw_pointer->encryption_type + pos, "CCMP"); 
                break;

                case MY_CIPHER_WEP104:
                sprintf(raw_pointer->encryption_type + pos, "WEP104"); 
                break;

                case MY_CIPHER_AESOCB:
                sprintf(raw_pointer->encryption_type + pos, "AESOCB"); 
                break;

                case MY_CIPHER_CKIP:
                sprintf(raw_pointer->encryption_type + pos, "CKIP"); 
                break;

            }
        }
    }
}


// record MAC addresses of know station
char knownStaMAC[PACKET_NUMBER][20];
// count the number of records
int sta_records_count = 0;

// add new station record to knownStaMAC
int addNewStation(RAW_STA_XML_DATA* raw_pointer)
{
    // first find out if the staion has already in record.
    int i;
    for (i = 0; i < sta_records_count && i < PACKET_NUMBER; ++i) {
        // if the hotspot is in record
        if (!strcmp(raw_pointer ->mac, knownStaMAC[i])) {
            // do nothing but return 0 means old record is detected
            return 0;
        }   
    }
    // add new hotspot record and record is not full
    if (sta_records_count < PACKET_NUMBER) {
        strcpy(knownStaMAC[sta_records_count], raw_pointer->mac);
    }
    // update record count
    sta_records_count++;
    // new record has been added and return 1
    return 1;
}

// get station mac address from different kinds of packet
int getStationMAC(const IEEE80211_COMMON_HEADER * cHeader, RAW_STA_XML_DATA* raw_pointer)
{
    if (raw_pointer == NULL || cHeader == NULL ) {
        return 0;
    }

    switch(cHeader->frame_control[0]) {
        // probe request
        case PROBE_REQUEST:{
            sprintf(raw_pointer->mac, "%02X-%02X-%02X-%02X-%02X-%02X",  cHeader->address2[0], cHeader->address2[1], 
                cHeader->address2[2], cHeader->address2[3], cHeader->address2[4], cHeader->address2[5]);
            //printf("address2:%s\n", raw_pointer->mac);
            return 1;
            }
            break;
        // rts
        case RTS:{
            sprintf(raw_pointer->mac, "%02X-%02X-%02X-%02X-%02X-%02X",  cHeader->address2[0], cHeader->address2[1], 
                cHeader->address2[2], cHeader->address2[3], cHeader->address2[4], cHeader->address2[5]);
            //printf("address2:%s\n", raw_pointer->mac);
            return 1;
            }
            break;

     

        default: break;
    }
    return 0;
}

// fill the data of station
int fillStaData(const RADIOTAP_C_HEADER *rHeader, const u_char * packet, RAW_STA_XML_DATA* raw_pointer, const struct pcap_pkthdr * pkthdr)
{
     // calculate radiotap header length
    int l1= rHeader->len[1];
    int l2 = rHeader->len[0];
    int radiotap_len = (l1 << 8) + l2;
    const IEEE80211_COMMON_HEADER * cHeader = (IEEE80211_COMMON_HEADER *)(packet + radiotap_len);
    // fill mac address
    if (!(getStationMAC(cHeader, raw_pointer) && addNewStation(raw_pointer))) {
        return 0;
    }
    // fill rssi
    struct raw_hotspot_xml_data temp_raw = {"", "", 0, "", "", 0};
    getSignal(rHeader, packet, &temp_raw);
    raw_pointer->rssi = temp_raw.rssi;
    
    // fill recieved time
    sprintf(raw_pointer->recieved_time, "%ld", pkthdr->ts.tv_sec);
    // successful fill process
    return 1;
}