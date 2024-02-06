#include "blockStorage.hpp"
#include "context.hpp"
#include <fstream>
#include <string>
#include <vector>

int main() {
    Database::Context<int> context("blockStorageTest.db1");
    Index::BlockStorage blockStorage("blockStorageTest.db2");
    Database::Context<Index::Block> blockContext("blockStorageTest.db2");

    // iterate 10 with random values
    for (int i = 0; i < 10; i++) {
        context.append(i);
    }

    std::streampos pos = context.append(40);
    std::streampos blockStart = blockStorage.startChain(pos);
    std::cout << "\nblockStart: " << blockStart << std::endl;
    blockStorage.insertBlock(blockStart, context.append(360));

    for (int i = 10; i < 20; i++) {
        context.append(i);
    }

    // interate over block and print values
    std::cout << "\nblockStart: " << blockStart << std::endl;
    std::vector<std::streampos> block = blockStorage.retrieveBlock(blockStart);
    std::cout << "\nblock.size(): " << block.size() << std::endl;
    for (std::streampos pos : block) {
        std::cout << context.read(pos).value << std::endl;
    }

    // iterate over block
    blockContext.clearIterator();
    while (blockContext.next()) {
        int value = context.read(blockContext.curr.value.address).value;
        std::cout << "\nvalue: " << value << std::endl;
        std::cout << "blockContext.curr.value.address: " << blockContext.curr.value.address << std::endl;
        std::cout << "blockContext.curr.value.nextBlock: " << blockContext.curr.value.nextBlock << std::endl;
        std::cout << "blockContext.curr.position: " << blockContext.curr.position << std::endl;
    }

    context.reset();
    blockContext.reset();

    return 0;
}

// StockPrice Controller example

// Add stock price
// verificar se trie já foi indexada com o stockId desta stockPricePayload
//  se não foi, criar um novo block e adicionar o stockId + blockStartPos na trie
//  se foi, adicionar o stockPricePayload no bloco já existente
