// adapted from zmq_ipc_receiver.cc https://gist.github.com/jheo4/566c9173e90657ef14d6d4a7918f0db5
#include <zmq.h>
#include <iostream>
#include <chrono>
using namespace std;

#define getTimeStampNow() ( (double)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() / 1000 )

int main() {
  void *ctx_source, *sock_source;
  ctx_source = zmq_ctx_new();
  sock_source = zmq_socket(ctx_source, ZMQ_PAIR);
  zmq_connect(sock_source, "tcp://192.17.102.20:19436");//mxre machine ip

  void *ctx_sink, *sock_sink;
  ctx_sink = zmq_ctx_new();
  sock_sink = zmq_socket(ctx_sink, ZMQ_PAIR);
  zmq_bind(sock_sink, "tcp://192.17.102.20:19435");//this machine ip

  char ack[4];
  //app source receiving
  char send_buf1[164];
  char send_buf2[721920];
  for(int idx=0;idx<10;idx++){
  auto timestamp1 = std::chrono::high_resolution_clock::now();
  zmq_send(sock_source, send_buf1, 164, 0);
  zmq_send(sock_source, send_buf2, 721920, 0);
  //zmq_recv(sock_source, ack, 4, 0);
  auto timestamp2 = std::chrono::high_resolution_clock::now();
  //app sink sending
  char recv_buf[72];
  zmq_recv(sock_sink, recv_buf,72, 0);
  //zmq_recv(sock_sink, ack, 4, 0);
  auto timestamp3 = std::chrono::high_resolution_clock::now();
  auto diff21=timestamp2-timestamp1;
  auto diff32=timestamp3-timestamp2;
  std::cout<<"ILLIXR MOCK "<<std::chrono::duration_cast<std::chrono::milliseconds>(diff21).count()<<" ms, "<<std::chrono::duration_cast<std::chrono::milliseconds>(diff32).count()<<"ms"<<std::endl;
  }
  

  zmq_close(sock_source);
  zmq_ctx_destroy(ctx_source);
  
  zmq_close(sock_sink);
  zmq_ctx_destroy(ctx_sink);
}