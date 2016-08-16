#ifndef BLOCKGRAPHICSPOSITION_H
#define BLOCKGRAPHICSPOSITION_H

const int BLOCK_GRAPHICS_WIDTH = 125;
const int BLOCK_GRAPHICS_HEIGHT = 100;
const int BLOCK_GRAPHICS_PX_INIT = BLOCK_GRAPHICS_WIDTH * 0.70;
const int BLOCK_GRAPHICS_PY_INIT = 205 - BLOCK_GRAPHICS_HEIGHT/2;

class BlockGraphicsPosition{

public:
    BlockGraphicsPosition() : block_width(BLOCK_GRAPHICS_WIDTH),
                                    block_height(BLOCK_GRAPHICS_HEIGHT),
                                    block_px(BLOCK_GRAPHICS_PX_INIT),
                                    block_py(BLOCK_GRAPHICS_PY_INIT) {}

    BlockGraphicsPosition(int width, int height, int px, int py) :
                                    block_width(width),
                                    block_height(height),
                                    block_px(px),
                                    block_py(py) {}

    BlockGraphicsPosition getNextBlockPosition(){
        int block_px = this->block_px;
        int block_py = this->block_py;
        int block_height = this->block_height;
        int block_width = this->block_width;

        block_px += block_width * 1.70;
        BlockGraphicsPosition nextBlockPos (block_width, block_height, block_px, block_py);
        return nextBlockPos;
    }

    int getPx(){
        return this->block_px;
    }

    int getPy(){
        return this->block_py;
    }

    int getHeight(){
        return this->block_height;
    }

    int getWidth(){
        return this->block_width;
    }

private:
    int block_width;
    int block_height;
    int block_px;
    int block_py;
};

#endif // BLOCKGRAPHICSPOSITION_H
