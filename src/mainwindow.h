#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdint.h>
#include <QMainWindow>
#include <QGraphicsScene>
#include <TransactionGraphicsPosition.h>
#include <BlockGraphicsPosition.h>
#include <SignalSlotController.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QGraphicsScene *MemPool_Scene;
    QGraphicsScene *Blockchain_Scene;
    TransactionGraphicsPosition TxGraphicsPos;
    BlockGraphicsPosition BxGraphicsPos;
    int transaction_count = 0;
    int block_count = 0;
    SignalSlotController signal_slot_controller;

public slots:
    void slotNewGraphicsTransaction(QString raw_tx_data);
    void slotNewGraphicsBlock(QString raw_tx_data);
    void slotDisplayTxInfo(const Transaction& transaction);
    void slotDisplayBlockInfo(const Block& block);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
