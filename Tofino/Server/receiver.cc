#include "driver/driver.h"
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>

using namespace std;
#pragma pack(push, 1)
struct packet {
  char key[13];
  uint16_t len;
  double ts;
};
#pragma pack(pop)

dpdk_driver driver;
queue<rte_mbuf *> recv_queue;
queue<long> ts_queue;
auto start_ts = chrono::high_resolution_clock::now();
auto cur_ts = chrono::high_resolution_clock::now();
long last_interval_ts = 0, cur_ts_diff = 0;
const long record_interval_ts = 100000; //0.1s = 100ms = 100,000 us
std::vector<double> throughput_results{};     //MPPS
std::string result_path{};
uint32_t nb_pkt = 0;

void exit_handler(int signo) {
    throughput_results.push_back(nb_pkt*10.0/1000000.0);
    ofstream of(result_path);
    for(auto res: throughput_results) {
        of << res << endl;
    }
    exit(0);
}

void recv_pkt() {
    uint16_t port = 0;
    unsigned nb_rx_pkt;

    struct rte_mbuf  *mbuf,*mbufs[BURST_SIZE];
    struct rte_ether_hdr  *hdr_eth; 

    if (recv_queue.empty()) {
        /* Send burst of TX packets, to second port of pair. */
        uint16_t nb_rx;

        /* Free any unsent packets. */
        do {
            nb_rx = rte_eth_rx_burst(port, 0, mbufs, 4);
        } while (unlikely(nb_rx < 1));
        cur_ts = chrono::high_resolution_clock::now();
        cur_ts_diff = chrono::duration_cast<chrono::microseconds>(cur_ts - start_ts).count();
        for (int buf = 0; buf < nb_rx; buf++) {
            recv_queue.push(mbufs[buf]);
            ts_queue.push(cur_ts_diff);
        }
    }

    mbuf = recv_queue.front();
    cur_ts_diff = ts_queue.front();
    recv_queue.pop();
    ts_queue.pop();
    if(cur_ts_diff - last_interval_ts >= record_interval_ts) {
        throughput_results.push_back(nb_pkt*10.0/1000000.0);
        nb_pkt = 1;
        last_interval_ts += record_interval_ts;
        while(cur_ts_diff > last_interval_ts+record_interval_ts) {
            last_interval_ts += record_interval_ts;
            throughput_results.push_back(0);
        }
    }
    else {
        nb_pkt += 1;
    }
    hdr_eth = rte_pktmbuf_mtod(mbuf, struct rte_ether_hdr  *);
    rte_pktmbuf_free(mbuf);
}

int main(int argc, char *argv[]) {
    driver.init(argc-1, argv);
    result_path = string(argv[argc-1]);
    struct sigaction new_sa, old_sa;

    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = 0;
    new_sa.sa_handler = exit_handler;
    sigaction(SIGINT, &new_sa, &old_sa);

    start_ts = chrono::high_resolution_clock::now();
    while (true) {
        recv_pkt();
    }
}