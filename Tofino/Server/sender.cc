#include "driver/driver.h"
#include <arpa/inet.h>
#include <bits/stdc++.h>

using namespace std;
#pragma pack(push, 1)
struct packet {
  char key[13];
  uint16_t len;
  double ts;
};
#pragma pack(pop)

dpdk_driver driver;

int nb_send = 0;
double nb_byte = 0;

int burst_send = 0;
double burst_byte = 0;

auto tb = chrono::high_resolution_clock::now();
auto te = chrono::high_resolution_clock::now();

void exit_handler(int signo) {
  te = chrono::high_resolution_clock::now();
  cout << endl << "nb_send: " << nb_send << endl;
  cout << endl << "nb_byte: " << nb_byte << endl;
  cout << endl
       << "time: "
       << chrono::duration_cast<chrono::milliseconds>(te - tb).count() / 1000.0
       << endl;
  exit(0);
}

void send_pkt(uint8_t mac_addr[],char *flow_key, unsigned flow_len) {
  char *msg = new char[1500];
  unsigned msg_len = 0;

  if (flow_len > 1400) {
    flow_len = 1400;
  }

  memset(msg, 0, 1500);

  msg_len += inject_flag_hdr(msg + msg_len, mac_addr, flow_key, flow_len);
  if (msg_len + 28 < flow_len) {
    msg_len = flow_len - 28;
  } 
  // for (int i = 0; i < 13; i++) {
  //   cout << (int)(unsigned char)flow_key[i] << " ";
  // }
  // cout << endl;

  driver.send_pkt(msg, msg_len);
  nb_send += 1;
  nb_byte += flow_len;
  burst_send += 1;
  burst_byte += (msg_len + 20 + 14 + 8);

  delete[] msg;
}

vector<packet> vp;

int main(int argc, char *argv[]) {
  driver.init(argc, argv);
  ifstream caida(argv[argc - 1], ios::binary);
  uint8_t tmp_mac_addr[6] = {};

  struct sigaction new_sa, old_sa;

  sigemptyset(&new_sa.sa_mask);
  new_sa.sa_flags = 0;
  new_sa.sa_handler = exit_handler;
  sigaction(SIGINT, &new_sa, &old_sa);

  packet p;
  while (caida.read(p.key, sizeof(packet))) {
    //notice the big and little endian
    vp.push_back(p);
  }

  const int nb_burst = 65536;

  tb = chrono::high_resolution_clock::now();

  cout << "dataset read: " << vp.size() << endl;

  auto t1 = chrono::high_resolution_clock::now();
  // while (true) {
  for (int i = 0; i < vp.size(); i++) {
    send_pkt(tmp_mac_addr, vp[i].key, ntohs(vp[i].len));

    if (burst_send % nb_burst == 0) {
      auto t2 = chrono::high_resolution_clock::now();
      auto t3 = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
      int t4 =
          round(burst_byte * 8.0 / (40.0 * 1000 * 1000 * 1000) * 1000000.0);

      if (t4 > t3) {
        this_thread::sleep_for(chrono::microseconds(t4 - t3));
      }

      t1 = chrono::high_resolution_clock::now();
      burst_send = 0;
      burst_byte = 0;
    }
  }
  // }

  exit_handler(0);
}