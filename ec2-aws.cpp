//#include "easywsclient/easywsclient.hpp"
#include "easywsclient/easywsclient.cpp" // <-- include only if you don't want compile separately
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <fstream>
#include <iostream>

//524288 next log2
#define MAX_TX 345600
#define MAX_BX 1440

int tx_ind = 0;
int bx_ind = 0;
int file_cnt = 0;

using easywsclient::WebSocket;
std::string raw_tx_data, raw_block_data;

std::string tx_arr[MAX_TX+10];
std::string tx_time_stamp[MAX_TX+10];
std::string bx_arr[MAX_BX+10];
std::string bx_time_stamp[MAX_BX+10];

std::string time_stamp_to_string(std::chrono::time_point<std::chrono::system_clock> time_stamp){
  time_t t = std::chrono::system_clock::to_time_t(time_stamp);
  struct tm * ts = localtime(&t);

  std::string time_str;
  time_str += std::to_string(ts->tm_year + 1900) + "-" + std::to_string(ts->tm_mon + 1) + "-" + std::to_string(ts->tm_mday) + " ";
  time_str += std::to_string(ts->tm_hour) + ":" + std::to_string(ts->tm_min) + ":" + std::to_string(ts->tm_sec) + " ";
  // Daylight Saving Time
  time_str += std::to_string(ts->tm_isdst) + "\n";

  return time_str;
}

void handle_transaction_message(const std::string & message){
    raw_tx_data = message;
}

void handle_block_message(const std::string & message){
    raw_block_data = message;
}

void new_tx(const std::string & message){
  tx_arr[tx_ind] = message;
  tx_time_stamp[tx_ind] = time_stamp_to_string(std::chrono::system_clock::now());
  tx_ind++;
  
  if (tx_ind == MAX_TX){
    // dump!
    std::ofstream dump_file;
    std::string filename("dump_tx_");
    filename += std::to_string(file_cnt);
    filename += ".txt";
    dump_file.open(filename);

    for (int i = 0; i < tx_ind; i++){
      dump_file << "### TRANSACTION LOG START ###\n";
      dump_file << tx_arr[i] + "\n";
      dump_file << "### TRANSACTION LOG TS ###\n";
      dump_file << tx_time_stamp[i];
      dump_file << "### TRANSACTION LOG END ###\n";
    }

    dump_file.close();

    std::ofstream dump_file2;
    std::string filename2("dump_bx_");
    filename2 += std::to_string(file_cnt);
    filename2 += ".txt";
    dump_file2.open(filename2);

    int local_bx_ind = bx_ind;
    for (int i = 0; i < local_bx_ind; i++){
      dump_file2 << "### BLOCK LOG START ###\n";
      dump_file2 << bx_arr[i] + "\n";
      dump_file2 << "### BLOCK LOG TS ###\n";
      dump_file2 << bx_time_stamp[i];
      dump_file2 << "### BLOCK LOG END ###\n";
    }

    dump_file2.close();

    file_cnt++;
    printf("File dump %d\n", file_cnt);
    tx_ind = 0;

    // get mutex
    bx_ind = 0;
  }
}

void new_block(const std::string & message){
  bx_arr[bx_ind] = message;
  bx_time_stamp[bx_ind] = time_stamp_to_string(std::chrono::system_clock::now());
  bx_ind++;
}

void socket_transactions_connect(){
    WebSocket::pointer websocket = WebSocket::from_url("ws://ws.blockchain.info/inv");
    assert(websocket);
    websocket->send("{\"op\":\"unconfirmed_sub\"}");
    int tx_count = 0;

    std::chrono::time_point<std::chrono::system_clock> start_t, end_t;
    start_t = std::chrono::system_clock::now();

    while (websocket->getReadyState() != WebSocket::CLOSED) {
      end_t = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end_t-start_t;

      if (elapsed_seconds.count() > 30) {
          websocket->sendPing();
          start_t = std::chrono::system_clock::now();
      }

      websocket->poll(-1);
      websocket->dispatch(handle_transaction_message);

      // No new message.
      if (raw_tx_data == "") continue;

      // New message.
      new_tx(raw_tx_data);
      tx_count++;

      start_t = std::chrono::system_clock::now();
      raw_tx_data = "";
    }

    printf("Closed Transactions Socket\n");
    delete websocket;
}

void socket_blocks_connect(){
    WebSocket::pointer websocket = WebSocket::from_url("ws://ws.blockchain.info/inv");
    assert(websocket);
    websocket->send("{\"op\":\"ping_block\"}");
    int block_count = 0;

    while (websocket->getReadyState() != WebSocket::CLOSED) {
      websocket->poll(-1);
      websocket->dispatch(handle_block_message);
      // No new message.
      if (raw_block_data != ""){
        // New message.
        new_block(raw_block_data);
        block_count++;

        raw_block_data = "";
        break;
      } else {
          continue;
      }
    }

    websocket->send("{\"op\":\"blocks_sub\"}");

    std::chrono::time_point<std::chrono::system_clock> start_t, end_t;
    start_t = std::chrono::system_clock::now();

    while (websocket->getReadyState() != WebSocket::CLOSED) {
      end_t = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end_t-start_t;

      if (elapsed_seconds.count() > 30) {
          websocket->sendPing();
          //printf("ping\n");
          start_t = std::chrono::system_clock::now();
      }

      websocket->poll(30000);
      websocket->dispatch(handle_block_message);

      // No new message.
      if (raw_block_data == "") continue;

      // New message.
      new_block(raw_block_data);
      block_count++;

      start_t = std::chrono::system_clock::now();

      raw_block_data = "";
    }

    delete websocket;
    printf("Closed Blocks Socket\n");
}

int main(int argc, char *argv[]){
    std::thread websocket_transactions_thread(socket_transactions_connect);
    std::thread websocket_blocks_thread(socket_blocks_connect);

    websocket_transactions_thread.join();
    websocket_blocks_thread.join();
    return 0;
}

