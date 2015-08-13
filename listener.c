#include "listener.h"
#include "saveXML.h"
/****************************hotspot*************************************/

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
            sprintf(raw_pointer->encryption_type, "01");     // wep
        }
        // wpa
        else {
            switch(e->wpa_version) {
                case 3:
                sprintf(raw_pointer->encryption_type, "99");  // wpa/wpa2
                break;

                case 2:
                sprintf(raw_pointer->encryption_type, "03");   // wpa2
                break;

                case 1:
                sprintf(raw_pointer->encryption_type, "02");   // wpa
                break;
                default:
                break;
            }
        }
    }
}

int fillHotspotData(const RADIOTAP_C_HEADER *rHeader, const u_char * packet, RAW_HOTSPOT_XML_DATA* raw_pointer, const struct pcap_pkthdr * pkthdr)
{
    int l1= rHeader->len[1];
    int l2 = rHeader->len[0];
    int radiotap_len = (l1 << 8) + l2;
    // packet len which have not been visited yet
    int len_count = pkthdr->len;
    int i;
    const IEEE80211BEACON_FRAME * bHeader = (IEEE80211BEACON_FRAME*)(packet + radiotap_len);
    // get time
    // output recieved time to struct RAW_HOTSPOT_XML_DATA raw
    sprintf(raw_pointer->recieved_time, "%ld", pkthdr->ts.tv_sec);
    // get signal
    getSignal(rHeader, packet, raw_pointer);
    // get channel
    getChannel(rHeader, packet, raw_pointer);
    // update len_count;
    len_count -= radiotap_len;
    // SOURCE MAC
    // output source mac to struct RAW_HOTSPOT_XML_DATA raw
    sprintf(raw_pointer->mac, "%02X-%02X-%02X-%02X-%02X-%02X", bHeader->address2[0], bHeader->address2[1],
           bHeader->address2[2], bHeader->address2[3], bHeader->address2[4], bHeader->address2[5]);

    if (bHeader -> ssid_tag_length) {
        // get SSID
        for (i = 0; i < bHeader->ssid_tag_length; ++i) {
            sprintf(raw_pointer->ssid + i, "%c", bHeader->ssid[i]);
        }
    } else {
        sprintf(raw_pointer->ssid, "Broadcast");
        return 0;
    }
    if (!addNewHotspot(raw_pointer)) {
        return 0;
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
            } else if (!memcmp(p_encryption + 3, wpa2_oui, 3)) {
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

        // if tag is VERDOR_SPECIFIC
        } else if (tag_num == VERDOR_SPECIFIC) {
            if (!memcmp(p_encryption + 1, wpa_oui, 3) && p_encryption[4] == 1) {
                e.wpa_version +=1;
            }
        }
        p_encryption += (tag_len + 1);
        len_count -= (tag_len + 1);
    }
    print_encry(&e, raw_pointer);
    //printf("\n");
    return 1;
}

/****************************station*************************************/

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

