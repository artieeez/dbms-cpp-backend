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
    blockStorage.insertBlock(blockStart, context.append(360));
    blockStorage.insertBlock(blockStart, context.append(900));
    blockStorage.insertBlock(blockStart, context.append(940));

    for (int i = 10; i < 20; i++) {
        context.append(i);
    }

    // interate over block and print values
    std::vector<std::streampos> block = blockStorage.retrieveBlock(blockStart);
    for (std::streampos pos : block) {
        std::cout << context.read(pos).value << std::endl;
    }

    // iterate over block
    blockContext.clearIterator();
    while (blockContext.next()) {
        int value = context.read(blockContext.curr.value.address).value;
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
