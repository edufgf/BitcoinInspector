#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T17:26:42
#
#-------------------------------------------------

QT       += core gui
            core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bitcoin-analyzer2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        easywsclient/easywsclient.cpp

HEADERS  += mainwindow.h\
            easywsclient/easywsclient.hpp \
            WebSocketThread.h \
            QGraphicsTransactionItem.h \
    TransactionGraphicsPosition.h \
    SignalSlotController.h \
    json.h \
    Transaction.h \
    TxInput.h \
    TxOutput.h \
    QGraphicsBlockItem.h \
    BlockGraphicsPosition.h \
    Block.h \
    Miner.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
