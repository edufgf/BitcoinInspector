#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "QGraphicsTransactionItem.h"
#include "QGraphicsBlockItem.h"
#include "Transaction.h"

#include <ctime>
#include <chrono>

int bpx, bpy;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MemPool_Scene = new QGraphicsScene(this);
    ui->MemPool_GraphicsView->setScene(MemPool_Scene);
    MemPool_Scene->setSceneRect(ui->MemPool_GraphicsView->rect().x(),
                                ui->MemPool_GraphicsView->rect().y(),
                                ui->MemPool_GraphicsView->rect().width() - ui->MemPool_GraphicsView->rect().width()*0.02,
                                ui->MemPool_GraphicsView->rect().height() - ui->MemPool_GraphicsView->rect().height()*0.02);

    Blockchain_Scene = new QGraphicsScene(this);
    ui->Blockchain_GraphicsView->setScene(Blockchain_Scene);
    Blockchain_Scene->setSceneRect(ui->Blockchain_GraphicsView->rect().x(),
                                ui->Blockchain_GraphicsView->rect().y(),
                                ui->Blockchain_GraphicsView->rect().width() - ui->Blockchain_GraphicsView->rect().width()*0.02,
                                ui->Blockchain_GraphicsView->rect().height() - ui->Blockchain_GraphicsView->rect().height()*0.02);

    // speedup
    ui->Info_PlainText->setContextMenuPolicy(Qt::NoContextMenu);
    ui->Info_PlainText->setReadOnly(true);
    ui->Info_PlainText->setUndoRedoEnabled(false);
}



void MainWindow::slotDisplayTxInfo(const Transaction& transaction){
    ui->Info_PlainText->setUpdatesEnabled(false);
    ui->Info_PlainText->setPlainText(transaction.getRawData());
    ui->Info_PlainText->setUpdatesEnabled(true);

}

void MainWindow::slotDisplayBlockInfo(const Block& block){
    ui->Info_PlainText->setUpdatesEnabled(false);
    ui->Info_PlainText->setPlainText(block.getRawData());
    ui->Info_PlainText->setUpdatesEnabled(true);
}

void MainWindow::slotNewGraphicsTransaction(QString raw_tx_data){
    QGraphicsTransactionItem * new_tx = new QGraphicsTransactionItem(TxGraphicsPos, &signal_slot_controller);
    new_tx->setRawData(raw_tx_data);
    new_tx->getTransaction().setTimeStamp(std::chrono::system_clock::now());


    QObject::connect(&signal_slot_controller, SIGNAL(signalDisplayTxInfo(const Transaction&)),
                     this, SLOT(slotDisplayTxInfo(const Transaction&)));

    MemPool_Scene->addItem(new_tx);
    QGraphicsSimpleTextItem * tx_label = MemPool_Scene->addSimpleText("1", QFont("Comic Sans", 9, -1, false));
    tx_label->setParentItem(new_tx);
    tx_label->setPos(tx_label->parentItem()->boundingRect().center() - QPointF(tx_label->boundingRect().width()/2, tx_label->boundingRect().height()/2));

    TxGraphicsPos = TxGraphicsPos.getNextTxPosition(MemPool_Scene->sceneRect().y() +  MemPool_Scene->sceneRect().height());
    transaction_count++;

    QRectF cur_rect = MemPool_Scene->itemsBoundingRect();
    if (cur_rect.width() > MemPool_Scene->sceneRect().width())
        ui->MemPool_GraphicsView->setSceneRect(MemPool_Scene->itemsBoundingRect());

    QString current_tx_count = ui->MemPoolCounter_label->text();
    long tx_count = current_tx_count.toLong() + 1;
    ui->MemPoolCounter_label->setText(QString::number(tx_count));
}

void MainWindow::slotNewGraphicsBlock(QString raw_bx_data){
    QGraphicsBlockItem * new_bx = new QGraphicsBlockItem(BxGraphicsPos, &signal_slot_controller);
    new_bx->setRawData(raw_bx_data);
    new_bx->getBlock().setTimeStamp(std::chrono::system_clock::now());

    new_bx->getBlock().dump_log();

    QObject::connect(&signal_slot_controller, SIGNAL(signalDisplayBlockInfo(const Block&)),
                     this, SLOT(slotDisplayBlockInfo(const Block&)));

    std::string block_text;
    block_text = "Height: ";
    block_text += std::to_string(new_bx->getBlock().height) + "\n";
    block_text += "BTC: ";
    auto aux_value = new_bx->getBlock().totalBTCSent/1000000;
    auto whole_part = aux_value/100;
    auto fraction = aux_value%100;
    block_text += std::to_string(whole_part) + ".";
    if (fraction <= 9) block_text += "0";
    block_text += std::to_string(fraction) + "\n";
    block_text += "Tx_cnt: ";
    block_text += std::to_string(new_bx->getBlock().nTx) + "\n";

    block_text += "Size: " + std::to_string(new_bx->getBlock().size/1000) + " kB\n\n";

    time_t timestamp = new_bx->getBlock().time;
    struct tm * ts = localtime(&timestamp);

    std::string time_str;
    time_str += std::to_string(ts->tm_year + 1900) + "-" + std::to_string(ts->tm_mon + 1) + "-" + std::to_string(ts->tm_mday) + " ";
    time_str += std::to_string(ts->tm_hour) + ":" + std::to_string(ts->tm_min) + ":" + std::to_string(ts->tm_sec);

    block_text += time_str;

    QGraphicsSimpleTextItem * bx_label = Blockchain_Scene->addSimpleText(QString::fromStdString(block_text), QFont("Comic Sans", 9, -1, false));
    bx_label->setParentItem(new_bx);
    bx_label->setPos(bx_label->parentItem()->boundingRect().center() - QPointF(bx_label->boundingRect().width()/2, bx_label->boundingRect().height()/2));

    Blockchain_Scene->addItem(new_bx);
    BxGraphicsPos = BxGraphicsPos.getNextBlockPosition();

    block_count++;

    QRectF cur_rect = Blockchain_Scene->itemsBoundingRect();
    if (cur_rect.width() > Blockchain_Scene->sceneRect().width())
        ui->Blockchain_GraphicsView->setSceneRect(Blockchain_Scene->itemsBoundingRect());

    QString current_tx_count = ui->BlockchainCounter_label->text();
    long tx_count = current_tx_count.toLong() + 1;
    ui->BlockchainCounter_label->setText(QString::number(tx_count));

}

MainWindow::~MainWindow()
{
    delete ui;
}
