#ifndef BLOCKSTORAGE_HPP
#define BLOCKSTORAGE_HPP

#include <fstream>
#include <string>
#include <vector>
#include "context.hpp"

namespace Index {

struct Block {
    std::streampos address;
    std::streampos nextBlock;
};

class BlockStorage {
public:
    Database::Context<Block> _context;
    BlockStorage(const std::string &filePath);

    std::vector<std::streampos> retrieveBlock(std::streampos blockStart);
    void insertBlock(std::streampos blockStart, std::streampos address);
    std::streampos startChain(std::streampos address);
    void removeBlock(std::streampos blockStart);
    std::streampos removeBlockItem(std::streampos blockStart, std::streampos address);
};
}; // namespace Index

// { stockId: "petr4", companyId: 1 }
// { stockId: "amzn", companyId: 2 }

// { stockId: "petr4", date: "2021-01-01", value: 100.0, position: 40 }
// { stockId: "petr4", date: "2021-01-02", value: 500.0, position: 360 }
// { stockId: "petr4", date: "2021-01-03", value: 700.0, position: 900 }
// { stockId: "petr4", date: "2021-01-04", value: 300.0, position: 940 }

//  trie.get("petr4") >> retorna blockStart position

// blockStorage.retrieveBlock(blockStart) >> retorna vector de positions
// [40, 360, 900, 940]

#endif