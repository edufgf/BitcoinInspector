#ifndef QGRAPHICSTRANSACTIONITEM_H
#define QGRAPHICSTRANSACTIONITEM_H

#include <QObject>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsEllipseItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <TransactionGraphicsPosition.h>
#include <QDebug>
#include <Transaction.h>
#include <SignalSlotController.h>

class QGraphicsTransactionItem : public QGraphicsEllipseItem{

public:
        QGraphicsTransactionItem(int px, int py, int width, int height, SignalSlotController* signal_slot_controller){
            this->setPen(QColor(0, 0, 0));
            this->setBrush(QColor(255, 140, 0));
            this->setRect(px, py, width, height);
            this->signal_slot_controller = signal_slot_controller;
        }

        QGraphicsTransactionItem(TransactionGraphicsPosition tx_pos, SignalSlotController* signal_slot_controller) :
            QGraphicsTransactionItem(tx_pos.getPx(), tx_pos.getPy(), tx_pos.getWidth(), tx_pos.getHeight(), signal_slot_controller){
        }

        void setRawData(QString raw_data){
            Transaction new_t(raw_data);
            this->transaction = new_t;
        }

        void setTransaction(Transaction transaction){
            this->transaction = transaction;
        }

        Transaction& getTransaction(){
            return this->transaction;
        }

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event){
            if (event->button() == Qt::LeftButton){
                if (signal_slot_controller->selected != nullptr){
                    signal_slot_controller->selected->setBrush(QColor(255, 140, 0));
                }

                signal_slot_controller->selected = this;
                this->setBrush(QColor(135,200,250));

                signal_slot_controller->emit_signalDisplayTxInfo(this->transaction);
            }
        }

private:
        Transaction transaction;
        SignalSlotController* signal_slot_controller;

};

#endif // QGRAPHICSTRANSACTIONITEM_H
