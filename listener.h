/* $FreeBSD: src/sys/net80211/ieee80211_radiotap.h,v 1.5 2005/01/22 20:12:05 sam Exp $ */
/* $NetBSD: ieee80211_radiotap.h,v 1.11 2005/06/22 06:16:02 dyoung Exp $ */

/*-
 * Copyright (c) 2003, 2004 David Young.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of David Young may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY DAVID YOUNG ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL DAVID
 * YOUNG BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 */

/*
 * Modifications to fit into the linux IEEE 802.11 stack,
 * Mike Kershaw (dragorn@kismetwireless.net)
 */

 /*
  * Modifications to meet project request
  * Li Yicong(lycgaming@163.com)
  */

#ifndef LISTENER_H
#define LISTENER_H

#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


// present flags present[0]
#define IEEE80211_RADIOTAP_TSFT 0x01    // Time Synchronization Function timer
#define IEEE80211_RADIOTAP_FLAGS 0x02
#define IEEE80211_RADIOTAP_RATE 0x04
#define IEEE80211_RADIOTAP_CHANNEL 0x08
#define IEEE80211_RADIOTAP_FHSS 0x10
#define IEEE80211_RADIOTAP_DBM_ANTSIGNAL 0x20
#define IEEE80211_RADIOTAP_DBM_ANTNOISE 0x40
#define IEEE80211_RADIOTAP_LOCK_QUALITY  0x80

// present[1]
#define IEEE80211_RADIOTAP_TX_ATTENUATION 0x01
#define IEEE80211_RADIOTAP_DB_TX_ATTENUATION 0x02
#define IEEE80211_RADIOTAP_DBM_TX_POWER 0x04
#define IEEE80211_RADIOTAP_ANTENNA 0x08
#define IEEE80211_RADIOTAP_DB_ANTSIGNAL 0x10
#define IEEE80211_RADIOTAP_DB_ANTNOISE 0x20
#define IEEE80211_RADIOTAP_RX_FLAGS 0x40
#define IEEE80211_RADIOTAP_TX_FLAGS 0x80

// present[2]
#define HT_INFO 0x08                                                        // MCS infomation 3bytes  

// presnent[3]
#define IEEE80211_RADIOTAP_EXT 0x80



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
#define MY_KMGMT_8021X         (1 << 1)
#define MY_KMGMT_PSK           (1 << 2)

#define MY_AUTH_OPEN            (1 << 0)
#define MY_AUTH_SHARED          (1 << 1)

#define RSN                       0x30
#define VERDOR_SPECIFIC           0xdd

// type/subtype
#define BEACON_FRAME            0x80
#define PROBE_REQUEST           0x40
#define QOS_DATA                0x88
#define RTS                     0xb4 


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
    u_char address2[6]; 
};

struct raw_sta_xml_data {
	// should have more varibles
	char mac[20];
	int rssi;
	char recieved_time[100];
};


typedef struct radiotap_c_header RADIOTAP_C_HEADER;
typedef struct ieee80211beacon_frame IEEE80211BEACON_FRAME;
typedef struct encry ENCRYPTION;
typedef struct raw_hotspot_xml_data RAW_HOTSPOT_XML_DATA;
typedef struct ieee80211_common_header IEEE80211_COMMON_HEADER;
typedef struct raw_sta_xml_data RAW_STA_XML_DATA;
// get SIGNAL in dBm
void getSignal(const RADIOTAP_C_HEADER *rHeader, const u_char * packet, RAW_HOTSPOT_XML_DATA* raw_pointer);
// get channel
void getChannel(const RADIOTAP_C_HEADER *rHeader,const u_char * packet, RAW_HOTSPOT_XML_DATA* raw_pointer);
// print encryption informaiton
void print_encry(ENCRYPTION * e, RAW_HOTSPOT_XML_DATA* raw_pointer);
// get station mac address from different kinds of packet
int getStationMAC(const IEEE80211_COMMON_HEADER * cHeader, RAW_STA_XML_DATA* raw_pointer);

// fill the data of station and hotspot
int fillStaData(const RADIOTAP_C_HEADER *rHeader, const u_char * packet, RAW_STA_XML_DATA* raw_pointer, const struct pcap_pkthdr * pkthdr);
int fillHotspotData(const RADIOTAP_C_HEADER *rHeader, const u_char * packet, RAW_HOTSPOT_XML_DATA* raw_pointer, const struct pcap_pkthdr * pkthdr);



#endif              /* LISTENER_H */



