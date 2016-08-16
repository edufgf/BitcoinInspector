#ifndef SIGNALSLOTCONTROLLER_H
#define SIGNALSLOTCONTROLLER_H

#include <QObject>
#include <Transaction.h>
#include <Block.h>
#include <QAbstractGraphicsShapeItem>

class QGraphicsTransactionItem;
class QGraphicsBlockItem;

class SignalSlotController : public QObject {
    Q_OBJECT

public:
    void emit_signalDisplayTxInfo(const Transaction& transaction){
        emit signalDisplayTxInfo(transaction);
    }

    void emit_signalDisplayBlockInfo(const Block& block){
        emit signalDisplayBlockInfo(block);
    }

    //QGraphicsTransactionItem* last_info_tx = nullptr;
    //QGraphicsBlockItem* last_info_block = nullptr;
    QAbstractGraphicsShapeItem* selected = nullptr;

signals:
    void signalDisplayTxInfo(const Transaction& transaction);
    void signalDisplayBlockInfo(const Block& block);
};

#endif // SIGNALSLOTCONTROLLER_H
