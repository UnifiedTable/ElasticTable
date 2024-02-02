#include "driver/driver.h"
#include <bits/stdc++.h>
#include <arpa/inet.h>

#include <bits/stdint-uintn.h>
#include<unordered_map>
#include<vector>
#include<iostream>

#define USE_OFFCHIP_TEST_SPEED

using namespace std;
struct aclEntry {
    uint32_t srcIP;
    uint32_t dstIP;
    uint16_t srcPort;
    uint16_t dstPort;
    uint8_t action;
};


const uint32_t maxACLCapacity = 1024;
uint32_t ACLSize = 0;

//dpdk
dpdk_driver driver;

//MAC table, rib and acl
unordered_map<uint64_t, uint32_t> macTable{};
vector<unordered_map<uint32_t, uint32_t>> partitionRIB(32, unordered_map<uint32_t, uint32_t>{});

vector<aclEntry> acl;
vector<unordered_map<uint32_t, vector<uint32_t>>> srcIPLPMForACL(32, unordered_map<uint32_t, vector<uint32_t>>{});
vector<unordered_map<uint32_t, vector<uint32_t>>> dstIPLPMForACL(32, unordered_map<uint32_t, vector<uint32_t>>{});
unordered_map<uint16_t, vector<uint32_t>> srcPortEMForACL{};
unordered_map<uint16_t, vector<uint32_t>> dstPortEMForACL{};





// offChip MAC table lookup
uint32_t queryMACTable(uint64_t MACAddr) {
    return macTable[MACAddr];
}

// offChip RIB lookup
uint32_t queryRIB(uint32_t prefix, size_t maskLenth) {
    return partitionRIB[maskLenth-1][prefix];
}

//offChip ACL lookup
uint8_t queryACL(uint32_t srcPrefix, size_t srcMaskLenth, uint32_t dstPrefix, size_t dstMaskLenth, uint16_t srcPort, uint16_t dstPort) {
    const vector<uint32_t> & srcIPBitMap = srcIPLPMForACL[srcMaskLenth-1][srcPrefix];
    const vector<uint32_t> & dstIPBitMap = dstIPLPMForACL[dstMaskLenth-1][dstPrefix];
    const vector<uint32_t> & srcPortBitMap = srcPortEMForACL[srcPort];
    const vector<uint32_t> & dstPortBitMap = dstPortEMForACL[dstPort];
    size_t len = srcIPBitMap.size();
    size_t idx = 0;
    for(size_t i = 0; i < len; ++i) {
        uint32_t res = srcIPBitMap[i] & dstIPBitMap[i] & srcPortBitMap[i] & dstPortBitMap[i];
        if(res != 0) {
            for(int j = 0; j < 32; ++j) {
                if(((1<<(31-j)) & res) != 0) {
                    idx += i;
                    break;
                }
            }
            break;
        }
        else {
            idx += 32;
        }
    }
    if(idx >= ACLSize) {
        //default action
        return 0;
    }
    return acl[idx].action;
}

void pkt_handler(uint8_t *context, uint16_t payload_len) {
    uint8_t table_flag = *(context+20);
    uint8_t *info_payload = context+26;

    if(table_flag == 0b0 || table_flag == 0b1) {     //something error
        //do nothing
    }
    else if(table_flag == 0b10) {    //fast FIB forward
        //FIB lookup
        uint32_t dst_ip = htonl(*(uint32_t*)(context+10));
        for(int i = 0; i < 20; ++i) {
            if(*(info_payload+i) != 0) {
                size_t maskLength = i+1;
                uint32_t prefix = dst_ip & (~((1<<(31-i))-1));
                uint32_t nextHop = htonl(queryRIB(prefix, maskLength));
                memcpy(context+22, &nextHop, sizeof(uint32_t));
                break;
            }
        }
    }
    else if(table_flag == 0b100) {  //fast ACL forward
        uint32_t src_ip = htonl(*(uint32_t*)(context+6));
        uint32_t dst_ip = htonl(*(uint32_t*)(context+10));
        uint16_t src_port = htons(*(uint16_t*)(context+14));
        uint16_t dst_port = htons(*(uint16_t*)(context+16));
        size_t srcMaskLen = 0, dstMaskLen = 0;
        uint32_t srcPrefix = 0, dstPrefix = 0;
        for(int i = 0; i < 10; ++i) {
            if(*(info_payload+i) != 0) {
                srcMaskLen = i+1;
                srcPrefix = src_ip & (~((1<<(31-i))-1));
                break;
            }
        }
        for(int i = 0; i < 10; ++i) {
            if(*(info_payload+10+i) != 0) {
                dstMaskLen = i+1;
                dstPrefix = dst_ip & (~((1<<(31-i))-1));
                break;
            }
        }
        uint32_t action = htonl((uint32_t)queryACL(srcPrefix, srcMaskLen, dstPrefix, dstMaskLen, src_port, dst_port));
        memcpy(context+22, &action, sizeof(uint32_t));
    }
    else if(table_flag == 0b1000) {     //slow mac query
        uint64_t mac_addr = 0;
        for(int i = 0; i < 6; ++i) {
            mac_addr |= ((uint64_t)(*(context+i)) << ((5-i)<<3));
            uint32_t port = queryMACTable(mac_addr);
            memcpy(context+22, &port, sizeof(uint32_t));
        }
    }
    else if(table_flag == 0b10000) {    //slow FIB forward
        //query from longest prefix
        uint32_t dst_ip = htonl(*(uint32_t*)(context+10));
        for(int i = 31; i >= 0; --i) {
            uint32_t prefix = dst_ip & (~((1<<(31-i))-1));
            if(partitionRIB[i].find(prefix) != partitionRIB[i].end()) {
                uint32_t nextHop = htonl(partitionRIB[i][prefix]);
                memcpy(context+22, &nextHop, sizeof(uint32_t));
                break;
            }
        }
    }
    else if(table_flag == 0b100000) {   //slow ACL lookup
        uint32_t src_ip = htonl(*(uint32_t*)(context+6));
        uint32_t dst_ip = htonl(*(uint32_t*)(context+10));
        uint16_t src_port = htons(*(uint16_t*)(context+14));
        uint16_t dst_port = htons(*(uint16_t*)(context+16));
        size_t srcMaskLen = 0, dstMaskLen = 0;
        uint32_t srcPrefix = 0, dstPrefix = 0;
        for(int i = 31; i >= 0; --i) {
            uint32_t prefix = src_ip & (~((1<<(31-i))-1));
            if(srcIPLPMForACL[i].find(prefix) != srcIPLPMForACL[i].end()) {
                srcPrefix = prefix;
                srcMaskLen = i+1;
                break;
            }
        }
        for(int i = 31; i >= 0; --i) {
            uint32_t prefix = dst_ip & (~((1<<(31-i))-1));
            if(dstIPLPMForACL[i].find(prefix) != dstIPLPMForACL[i].end()) {
                dstPrefix = prefix;
                dstMaskLen = i+1;
                break;
            }
        }
        uint32_t action = htonl((uint32_t)queryACL(srcPrefix, srcMaskLen, dstPrefix, dstMaskLen, src_port, dst_port));
        memcpy(context+22, &action, sizeof(uint32_t));

    }
    else {
        //do nothing
    }
    memset(context+18, 0xfa, sizeof(uint16_t));
    return;
}

void exit_handler(int signo) {
  exit(0);
}

int main(int argc, char *argv[]) {
    driver.init(argc, argv);

    struct sigaction new_sa, old_sa;

    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = 0;
    new_sa.sa_handler = exit_handler;
    sigaction(SIGINT, &new_sa, &old_sa);

    char *msg = new char[1500];
    uint16_t payload_len;
    while (true) {
        driver.recv_pkt(msg, 1500, payload_len);

        pkt_handler((uint8_t *)msg, payload_len);

        driver.send_pkt(msg, payload_len);
    }
    delete[] msg;
}