#ifndef QGRAPHICSBLOCKITEM_H
#define QGRAPHICSBLOCKITEM_H

#include <QObject>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsEllipseItem>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <BlockGraphicsPosition.h>
#include <QDebug>
#include <Transaction.h>
#include <SignalSlotController.h>

class QGraphicsBlockItem : public QGraphicsRectItem{

public:
        QGraphicsBlockItem(int px, int py, int width, int height, SignalSlotController* signal_slot_controller){
            new QGraphicsLineItem(px-0.7*width, py + height/2, px, py + height/2, this);
            this->setPen(QColor(0, 0, 0));
            this->setBrush(QColor(255, 140, 0));
            this->setRect(px, py, width, height);
            this->signal_slot_controller = signal_slot_controller;
        }

        QGraphicsBlockItem(BlockGraphicsPosition block_pos, SignalSlotController* signal_slot_controller) :
            QGraphicsBlockItem(block_pos.getPx(), block_pos.getPy(), block_pos.getWidth(), block_pos.getHeight(), signal_slot_controller){
        }

        void setRawData(QString raw_data){
            Block new_b(raw_data);
            this->block = new_b;
        }

        void setBlock(Block block){
            this->block = block;
        }

        Block& getBlock() {
            return this->block;
        }

protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event){
            if (event->button() == Qt::LeftButton){
                if (signal_slot_controller->selected != nullptr){
                    signal_slot_controller->selected->setBrush(QColor(255, 140, 0));
                }

                signal_slot_controller->selected = this;
                this->setBrush(QColor(135,200,250));

                signal_slot_controller->emit_signalDisplayBlockInfo(this->block);
            }
        }

private:
        Block block;
        SignalSlotController* signal_slot_controller;

};

#endif // QGRAPHICSBLOCKITEM_H
