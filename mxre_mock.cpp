// adapted from zmq_ipc_sender.cc https://gist.github.com/jheo4/566c9173e90657ef14d6d4a7918f0db5#include <zmq.h>
#include <zmq.h>
#include <iostream>
#include <chrono>
using namespace std;

#define getTimeStampNow() ( (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() / 1000 )

int main() {
  void *ctx_appsource, *sock_appsource;
  ctx_appsource = zmq_ctx_new();
  sock_appsource = zmq_socket(ctx_appsource, ZMQ_PAIR);
  zmq_bind(sock_appsource, "tcp://192.17.102.20:19436");//this machine ip

  void *ctx_appsink, *sock_appsink;
  ctx_appsink = zmq_ctx_new();
  sock_appsink = zmq_socket(ctx_appsink, ZMQ_PAIR);
  zmq_connect(sock_appsink, "tcp://192.17.102.20:19435");//mxre machine ip

  char ack[4];
  //app source receiving
  char recv_buf1[72];
  char recv_buf2[360960];
  char recv_buf3[72];
  char recv_buf4[360960];
  char recv_buf5[8];
  char recv_buf6[4];
  char recv_buf7[8];
  for(int idx=0;idx<10;idx++){
  auto timestamp1 = std::chrono::high_resolution_clock::now();
  zmq_recv(sock_appsource, recv_buf1, 72, 0);
  zmq_recv(sock_appsource, recv_buf2, 360960, 0);
  zmq_recv(sock_appsource, recv_buf3, 72, 0);
  zmq_recv(sock_appsource, recv_buf4, 360960, 0);
  zmq_recv(sock_appsource, recv_buf5, 8, 0);
  zmq_recv(sock_appsource, recv_buf6, 4, 0);
  zmq_recv(sock_appsource, recv_buf7, 8, 0);
  zmq_send(sock_appsource, "ack", 4, 0);
  auto timestamp2 = std::chrono::high_resolution_clock::now();
  //app sink sending
  char send_buf[72];
  zmq_send(sock_appsink, send_buf,72, 0);
  //zmq_recv(sock_appsink, ack, 4, 0);
  auto timestamp3 = std::chrono::high_resolution_clock::now();
  auto diff21=timestamp2-timestamp1;
  auto diff32=timestamp3-timestamp2;
  std::cout<<"ILLIXR MOCK "<<std::chrono::duration_cast<std::chrono::milliseconds>(diff21).count()<<" ms, "<<std::chrono::duration_cast<std::chrono::milliseconds>(diff32).count()<<"ms"<<std::endl;
}

  

  zmq_close(sock_appsource);
  zmq_ctx_destroy(ctx_appsource);
  
  zmq_close(sock_appsink);
  zmq_ctx_destroy(ctx_appsink);
}