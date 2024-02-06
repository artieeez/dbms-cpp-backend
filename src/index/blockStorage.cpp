#include <fstream>
#include <string>
#include <vector>
#include "context.hpp"
#include "blockStorage.hpp"

namespace Index {


BlockStorage::BlockStorage(const std::string &filePath) : _context(filePath) {}

std::vector<std::streampos> BlockStorage::retrieveBlock(std::streampos blockStart) {
    std::vector<std::streampos> block;
    Database::Record<Block> currRecord = _context.read(blockStart);

    while (!currRecord.error && currRecord.value.nextBlock != -1) {
        block.push_back(currRecord.value.address);
        currRecord = _context.read(currRecord.value.nextBlock);
    }
    if (!currRecord.error) {
        block.push_back(currRecord.value.address);
    }
    return block;
}


void BlockStorage::insertBlock(std::streampos blockStart, std::streampos address) {
    Database::Record<Block> currRecord = _context.read(blockStart);
    while (!currRecord.error && currRecord.value.nextBlock != -1) {
        currRecord = _context.read(currRecord.value.nextBlock);
    }

    if (!currRecord.error) {
        Block newBlock;
        newBlock.address = address;
        newBlock.nextBlock = -1;
        std::streampos newBlockPosition = _context.append(newBlock);
        currRecord.value.nextBlock = newBlockPosition;

        _context.save(currRecord);
    }
}


std::streampos BlockStorage::startChain(std::streampos address) {
    Block newBlock;
    newBlock.address = address;
    newBlock.nextBlock = -1;
    return _context.append(newBlock);
}


void BlockStorage::removeBlock(std::streampos blockStart) {
    Database::Record<Block> currRecord = _context.read(blockStart);
    while (!currRecord.error && currRecord.value.nextBlock != -1) {
        Database::Record<Block> nextRecord = _context.read(currRecord.value.nextBlock);
        _context.remove(currRecord.position);
        currRecord = nextRecord;
    }
    if (!currRecord.error) {
        _context.remove(currRecord.position);
    }
}


// Removes a block item from the block chain. Returns the new block start position.
std::streampos BlockStorage::removeBlockItem(std::streampos blockStart, std::streampos address) {
    Database::Record<Block> currRecord = _context.read(blockStart);
    if (currRecord.error) {
        return -1;
    }
    if (currRecord.value.address == address) {
        std::streampos newBlockStart = currRecord.value.nextBlock;
        _context.remove(blockStart);
        return newBlockStart;
    }
    while (!currRecord.error && currRecord.value.nextBlock != -1) {
        Database::Record<Block> nextRecord = _context.read(currRecord.value.nextBlock);
        if (nextRecord.error) {
            return -1;
        }
        if (nextRecord.value.address == address) {
            currRecord.value.nextBlock = nextRecord.value.nextBlock;
            _context.save(currRecord);
            _context.remove(nextRecord.position);
            return blockStart;
        }
        currRecord = nextRecord;
    }
    return -1;
}

} // namespace Index