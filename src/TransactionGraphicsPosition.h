#ifndef TRANSACTIONGRAPHICSPOSITION_H
#define TRANSACTIONGRAPHICSPOSITION_H

const int TX_GRAPHICS_WIDTH = 15;
const int TX_GRAPHICS_HEIGHT = 15;
const int TX_GRAPHICS_PX_INIT = TX_GRAPHICS_WIDTH * 0.20;
const int TX_GRAPHICS_PY_INIT = TX_GRAPHICS_HEIGHT * 0.20;

class TransactionGraphicsPosition{

public:
    TransactionGraphicsPosition() : transaction_width(TX_GRAPHICS_WIDTH),
                                    transaction_height(TX_GRAPHICS_HEIGHT),
                                    transaction_px(TX_GRAPHICS_PX_INIT),
                                    transaction_py(TX_GRAPHICS_PY_INIT) {}

    TransactionGraphicsPosition(int width, int height, int px, int py) :
                                    transaction_width(width),
                                    transaction_height(height),
                                    transaction_px(px),
                                    transaction_py(py) {}

    TransactionGraphicsPosition getNextTxPosition(qreal max_height_y){
        int transaction_px = this->transaction_px;
        int transaction_py = this->transaction_py;
        int transaction_height = this->transaction_height;
        int transaction_width = this->transaction_width;

        transaction_py += transaction_height * 1.20;
        if (transaction_py + transaction_height * 1.20 > max_height_y){
            transaction_py = TX_GRAPHICS_PY_INIT;
            transaction_px += transaction_width * 1.20;
        }
        TransactionGraphicsPosition nextTxPos (transaction_width, transaction_height, transaction_px, transaction_py);
        return nextTxPos;
    }

    int getPx(){
        return this->transaction_px;
    }

    int getPy(){
        return this->transaction_py;
    }

    int getHeight(){
        return this->transaction_height;
    }

    int getWidth(){
        return this->transaction_width;
    }

private:
    int transaction_width;
    int transaction_height;
    int transaction_px;
    int transaction_py;
};

#endif // TRANSACTIONGRAPHICSPOSITION_H
