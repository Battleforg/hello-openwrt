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

#endif              /* TEST_H */