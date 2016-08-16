#include "mainwindow.h"
#include "easywsclient/easywsclient.hpp"
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string>
#include <QApplication>
#include <thread>
#include <QDebug>
#include <QObject>
#include "WebSocketThread.h"
#include "QGraphicsTransactionItem.h"
#include <chrono>
#include <ctime>

using easywsclient::WebSocket;
QString raw_tx_data, raw_block_data;

void handle_transaction_message(const std::string & message){
    raw_tx_data = QString(message.c_str());
}

void handle_block_message(const std::string & message){
    raw_block_data = QString(message.c_str());
}

void socket_transactions_connect(void* w){
    MainWindow* mw = (MainWindow*) w;
    WebSocketThread wst;
    QObject::connect(&wst, SIGNAL(signalNewGraphicsTransaction(QString)),
                     mw, SLOT(slotNewGraphicsTransaction(QString)));
    WebSocket::pointer websocket = WebSocket::from_url("ws://ws.blockchain.info/inv");
    assert(websocket);
    websocket->send("{\"op\":\"unconfirmed_sub\"}");

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

      emit wst.signalNewGraphicsTransaction(raw_tx_data);

      start_t = std::chrono::system_clock::now();

      raw_tx_data = "";
    }

    qDebug() << "Closed Transactions Socket\n";
    delete websocket;
}

void socket_blocks_connect(void* w){
    MainWindow* mw = (MainWindow*) w;
    WebSocketThread wst;
    QObject::connect(&wst, SIGNAL(signalNewGraphicsBlock(QString)),
                     mw, SLOT(slotNewGraphicsBlock(QString)));

    WebSocket::pointer websocket = WebSocket::from_url("ws://ws.blockchain.info/inv");
    assert(websocket);
    websocket->send("{\"op\":\"ping_block\"}");

    while (websocket->getReadyState() != WebSocket::CLOSED) {
      websocket->poll(-1);
      websocket->dispatch(handle_block_message);
      // No new message.
      if (raw_block_data != ""){
        emit wst.signalNewGraphicsBlock(raw_block_data);
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
          qDebug() << "ping\n";
          start_t = std::chrono::system_clock::now();
      }

      websocket->poll(30000);
      websocket->dispatch(handle_block_message);

      // No new message.
      if (raw_block_data == "") continue;

      emit wst.signalNewGraphicsBlock(raw_block_data);

      start_t = std::chrono::system_clock::now();

      raw_block_data = "";
    }

    delete websocket;
    qDebug() << "Closed Blocks Socket\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    std::thread websocket_transactions_thread(socket_transactions_connect, &w);
    std::thread websocket_blocks_thread(socket_blocks_connect, &w);

    return a.exec();
}

