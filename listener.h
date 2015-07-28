#ifndef TEST_H
#define TEST_H

#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


// flags 
#define TSFT 0x01    // Time Synchronization Function timer
#define FLAGS 0x02
#define RATE 0x04
#define CHANNEL 0x08
#define FHSS 0x10
#define SIGNAL 0x20
#define NOISE 0x40
#define LOCK  0x80

#define ANTENNA 0x08
#define RX_FLAGS 0x40

#define EXT 0x80

#define CHANNEL_1 2412
#define CHANNEL_2 2417
#define CHANNEL_3 2422
#define CHANNEL_4 2427
#define CHANNEL_5 2432
#define CHANNEL_6 2437
#define CHANNEL_7 2442
#define CHANNEL_8 2447
#define CHANNEL_9 2452
#define CHANNEL_10 2457
#define CHANNEL_11 2462
#define CHANNEL_12 2467
#define CHANNEL_13 2472
#define CHANNEL_14 2484

// learn definitions from openwrt iwinfo srouce code
#define MY_80211_A    (1 << 0)
#define MY_80211_B    (1 << 1)
#define MY_80211_C    (1 << 2)
#define MY_80211_D    (1 << 3)

#define MY_CIPHER_NONE      (1 << 0)
#define MY_CIPHER_WEP40     (1 << 1)
#define MY_CIPHER_TKIP         (1 << 2)
#define MY_CIPHER_WRAP       (1 << 3)
#define MY_CIPHER_CCMP       (1 << 4)
#define MY_CIPHER_WEP104   (1 << 5)
#define MY_CIPHER_AESOCB   (1 << 6)
#define MY_CIPHER_CKIP          (1 << 7)

#define MY_KMGMT_NONE          (1 << 0)
#define MY_KMGMT_8021X          (1 << 1)
#define MY_KMGMT_PSK              (1 << 2)

#define MY_AUTH_OPEN              (1 << 0)
#define MY_AUTH_SHARED          (1 << 1)

#define RSN                                    0x30
#define VERDOR_SPECIFIC           0xdd

#define BEACON_FRAME              0x80

#define PACKET_NUMBER            500


struct radiotap_c_header
{
    u_char version;
    u_char pad;
    u_char len[2];
    u_char present[]; // be careful! present may be more than 4*1

};

struct ieee80211beacon_frame
{
    u_char frame_control[2];
    u_char duration[2];
    u_char address1[6]; // DA
    u_char address2[6]; // SA
    u_char address3[6]; // BSS ID
    u_char sequence[2];
    // we not have addr4
    // FIXED 12 BYTES parameters
    u_char timestamp[8];
    u_char beacon_interval[2];
    u_char capa_info[2]; // capability info

    // SSID
    u_char ssid_tag_number;
    u_char ssid_tag_length;
    u_char ssid[];

};

struct encry
{
    uint8_t wpa_version;
    uint8_t group_ciphers;
    uint8_t pair_ciphers;
    uint8_t auth_algs;
};

struct raw_hotspot_xml_data
{
    char mac[20];                          // source MAC address

    char ssid[256];                        // SSID
    
    int channel;                             // channel is a int variable raning from 1 to 14

    char encryption_type[100];  // encryption type

    char recieved_time[100];     // recieved time
    
    int rssi;                                    // rssi is a negative int variable ranging form 0 to -infinity
    
};

struct ieee80211_common_header
{
    u_char frame_control[2];
    u_char duration[2];
    u_char address1[6]; 
};

typedef struct radiotap_c_header RADIOTAP_C_HEADER;
typedef struct ieee80211beacon_frame IEEE80211BEACON_FRAME;
typedef struct encry ENCRYPTION;
typedef struct raw_hotspot_xml_data RAW_HOTSPOT_XML_DATA;
typedef struct ieee80211_common_header IEEE80211_COMMON_HEADER;

// get SIGNAL in dBm
void getSignal(const RADIOTAP_C_HEADER *rHeader, const u_char * packet, struct raw_hotspot_xml_data* raw_pointer);
// get channel
void getChannel(const RADIOTAP_C_HEADER *rHeader,const u_char * packet, struct raw_hotspot_xml_data* raw_pointer);
// print encryption informaiton
void print_encry(ENCRYPTION * e, struct raw_hotspot_xml_data* raw_pointer);
// get station mac address from different kinds of packet
void getStationMAC(IEEE80211_COMMON_HEADER * cHeader);

#endif              /* TEST_H */



