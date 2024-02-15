#include <fstream>
#include <string>
#include <vector>
#include "context.hpp"
#include "blockStorage.hpp"
#include "logger.hpp"

extern Logger mainLogger;

namespace Index
{

    BlockStorage::BlockStorage(const std::string &filePath) : _context(filePath) {}

    std::vector<std::streampos> BlockStorage::retrieveBlock(std::streampos blockStart)
    {
        mainLogger.pushScope("BlockStorage::retrieveBlock");

        std::vector<std::streampos> block;
        Database::Record<Block> currRecord = _context.read(blockStart);

        while (!currRecord.error && currRecord.value.nextBlock != -1)
        {
            block.push_back(currRecord.value.address);
            currRecord = _context.read(currRecord.value.nextBlock);
        }
        if (!currRecord.error)
        {
            block.push_back(currRecord.value.address);
        }

        mainLogger.log("Block: " + std::to_string(blockStart) + " " + std::to_string(block.size()));
        mainLogger.popScope();
        return block;
    }

    void BlockStorage::insertBlock(std::streampos blockStart, std::streampos address)
    {
        mainLogger.pushScope("BlockStorage::insertBlock");
        Database::Record<Block> currRecord = _context.read(blockStart);
        while (!currRecord.error && currRecord.value.nextBlock != -1)
        {
            currRecord = _context.read(currRecord.value.nextBlock);
        }

        if (!currRecord.error)
        {
            Block newBlock;
            newBlock.address = address;
            newBlock.nextBlock = -1;
            std::streampos newBlockPosition = _context.append(newBlock);
            currRecord.value.nextBlock = newBlockPosition;
            std::cout << "insertBlock - Blocks - " << currRecord.value.address << "-" << currRecord.value.nextBlock << std::endl;
            _context.save(currRecord);
        }

        mainLogger.popScope();
    }

    std::streampos BlockStorage::startChain(std::streampos address)
    {
        mainLogger.pushScope("BlockStorage::startChain");
        Block newBlock;
        newBlock.address = address;
        newBlock.nextBlock = -1;
        mainLogger.popScope();
        return _context.append(newBlock);
    }

    void BlockStorage::removeBlock(std::streampos blockStart)
    {
        mainLogger.pushScope("BlockStorage::removeBlock");
        Database::Record<Block> currRecord = _context.read(blockStart);
        while (!currRecord.error && currRecord.value.nextBlock != -1)
        {
            Database::Record<Block> nextRecord = _context.read(currRecord.value.nextBlock);
            _context.remove(currRecord.position);
            currRecord = nextRecord;
        }
        if (!currRecord.error)
        {
            _context.remove(currRecord.position);
        }
        mainLogger.popScope();
    }

    // Removes a block item from the block chain. Returns the new block start position.
    std::streampos BlockStorage::removeBlockItem(std::streampos blockStart, std::streampos address)
    {
        mainLogger.pushScope("BlockStorage::removeBlockItem");
        Database::Record<Block> currRecord = _context.read(blockStart);
        if (currRecord.error)
        {
            mainLogger.log("Error reading block start", true);
            mainLogger.popScope();
            return -1;
        }
        if (currRecord.value.address == address)
        {
            std::streampos newBlockStart = currRecord.value.nextBlock;
            _context.remove(blockStart);
            mainLogger.popScope();
            return newBlockStart;
        }
        while (!currRecord.error && currRecord.value.nextBlock != -1)
        {
            Database::Record<Block> nextRecord = _context.read(currRecord.value.nextBlock);
            if (nextRecord.error)
            {
                mainLogger.log("Error reading block in chain", true);
                mainLogger.popScope();
                return -1;
            }
            if (nextRecord.value.address == address)
            {
                currRecord.value.nextBlock = nextRecord.value.nextBlock;
                _context.save(currRecord);
                _context.remove(nextRecord.position);
                mainLogger.popScope();
                return blockStart;
            }
            currRecord = nextRecord;
        }
        mainLogger.log("Block item not found", true);
        mainLogger.popScope();
        return -1;
    }

} // namespace Index