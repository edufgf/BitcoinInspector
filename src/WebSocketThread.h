#ifndef WEBSOCKETTHREAD_H
#define WEBSOCKETTHREAD_H

#include <QObject>
#include "QGraphicsTransactionItem.h"

class WebSocketThread : public QObject {
    Q_OBJECT

signals:
    void signalNewGraphicsTransaction(QString raw_tx_data);
    void signalNewGraphicsBlock(QString raw_tx_data);
};

#endif // WEBSOCKETTHREAD_H
