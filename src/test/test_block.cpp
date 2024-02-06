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
    context.append(50);
    blockStorage.insertBlock(blockStart, context.append(900));
    context.append(51);
    blockStorage.insertBlock(blockStart, context.append(940));
    context.append(52);

    for (int i = 10; i < 20; i++) {
        context.append(i);
    }

    // interate over block and print values
    std::cout << "\nBefore remove" << std::endl;
    std::vector<std::streampos> block = blockStorage.retrieveBlock(blockStart);
    for (std::streampos pos : block) {
        std::cout << "Value: " << context.read(pos).value << " - Address: " << pos << std::endl;
    }

    // remove third block
    blockStorage.removeBlockItem(blockStart, block[2]);

    // interate over block and print values
    std::cout << "\nAfter remove" << std::endl;
    block = blockStorage.retrieveBlock(blockStart);
    for (std::streampos pos : block) {
        std::cout << "Value: " << context.read(pos).value << " - Address: " << pos << std::endl;
    }

    // remove first block
    blockStart = blockStorage.removeBlockItem(blockStart, block[0]);

    // interate over block and print values
    std::cout << "\nAfter remove" << std::endl;
    block = blockStorage.retrieveBlock(blockStart);
    for (std::streampos pos : block) {
        std::cout << "Value: " << context.read(pos).value << " - Address: " << pos << std::endl;
    }

    // remove block
    blockStorage.removeBlock(blockStart);

    // iterate over context and print values
    std::cout << "\nAfter removeBlock" << std::endl;
    blockContext.clearIterator();
    while (blockContext.next()) {
        int val = context.read(blockContext.curr.value.address).value;
        std::cout << "Adress: " << blockContext.curr.value.address << " - Value: " << val << std::endl;
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
