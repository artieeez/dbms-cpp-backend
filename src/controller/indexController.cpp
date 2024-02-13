#include "indexController.hpp"
#include "blockStorage.hpp"
#include "context.hpp"
#include "stock.hpp"
#include "trie.hpp"
#include <array>
#include <cassert>
#include <queue>
#include <string>
#include <vector>
#include "logger.hpp"

extern Logger mainLogger;

namespace Controller {
namespace IndexSearch {
void addStock(Model::Stock payload) {
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::streampos position = dbContext.append(payload);
    trie.insertString(payload.stockId, position);
}

void deleteStock(std::string stockId) {
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(stockId, 1, 0);
    if (positions.size() > 0) {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stocks = dbContext.read(position);
        if (stocks.value.stockId == stockId) {
            dbContext.remove(position);
            trie.deleteString(stockId);
        }
    }
}

void updateStock(Model::Stock payload) {
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(payload.stockId, 1, 0);
    if (positions.size() > 0) {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stock = dbContext.read(position);
        if (stock.value.stockId == payload.stockId) {
            stock.value = payload;
            dbContext.save(stock);
        }
    }
}

Model::Stock getStock(std::string stockId) {
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(stockId, 1, 0);
    if (positions.size() > 0) {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stock = dbContext.read(position);
        if (stock.value.stockId == stockId) {
            return stock.value;
        }
    }
    return Model::Stock();
}

std::vector<Model::Stock> getStockList(std::string prefix, int pageSize, int page) {
    mainLogger.pushScope("getStockList");
    mainLogger.log("prefix: " + prefix);
    mainLogger.log("pageSize: " + std::to_string(pageSize));
    mainLogger.log("page: " + std::to_string(page));

    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(prefix, pageSize, page);
    std::vector<Model::Stock> stocks;

    for (std::streampos position : positions) {
        Database::Record<Model::Stock> stock = dbContext.read(position);
        stocks.push_back(stock.value);
    }

    //log stocks

    mainLogger.pushScope("getStockList - stocks response");
    for (auto stock : stocks) {
        mainLogger.log("stockId: " + stock.stockId);
    }
    if (stocks.size() == 0) {
        mainLogger.log("No stocks found");
    }
    mainLogger.popScope();

    mainLogger.popScope();
    return stocks;
}

///////////////////STOCKPRICE////////////////////

void addStockPrice(Model::StockPrice payload) {
    Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
    Index::BlockStorage blockStorage(Database::PATH::BLOCK::STOCK_ID_TO_STOCK_PRICE);
    Index::Trie trieStockBlock(Database::PATH::TRIE::STOCK_ID_TO_STOCK_PRICE_BLOCK);
    Index::Trie trieStockPrices(Database::PATH::TRIE::STOCK_PRICE_ID_TO_STOCK_PRICE);
    // adiciona no banco
    std::streampos dbPosition = dbContext.append(payload);

    // procura o stockId no trie que armazena endereços de blocos
    std::vector<std::streampos> positions = trieStockBlock.searchString(payload.stockId, 1, 0);
    if (positions.size() > 0) {
        // se encontrou, já há um bloco então basta adicionar um item no bloco
        std::streampos triePosition = positions[0];
        blockStorage.insertBlock(triePosition, dbPosition);
    } else {
        std::streampos blockPosition = blockStorage.startChain(dbPosition);
        trieStockBlock.insertString(payload.stockId, blockPosition);
    }
    trieStockPrices.insertString(payload.stockPriceId, dbPosition);
}

void deleteStockPrice(std::string stockPriceId, std::string stockId) {
    Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
    Index::Trie trieStockBlock(Database::PATH::TRIE::STOCK_ID_TO_STOCK_PRICE_BLOCK);
    Index::Trie trieStockPrices(Database::PATH::TRIE::STOCK_PRICE_ID_TO_STOCK_PRICE);
    Index::BlockStorage blockStorage(Database::PATH::BLOCK::STOCK_ID_TO_STOCK_PRICE);

    // Busca na trie de stockPrices o endereço do item no db
    std::vector<std::streampos> trieStockPricesAddress = trieStockPrices.searchString(stockPriceId, 1, 0);
    // Busca na trie de stockBlock o endereço do bloco que contém o item
    std::vector<std::streampos> trieStockBlockAddress = trieStockBlock.searchString(stockId, 1, 0);

    // Se encontrou o item na trie de stockPrices
    if (trieStockPricesAddress.size() > 0) {
        std::streampos dbAddress = trieStockPricesAddress[0];
        // Se encontrou o bloco na trie de stockBlock
        if (trieStockBlockAddress.size() > 0) {
            std::streampos blockAddress = trieStockBlockAddress[0];
            // Remove o item do bloco e salva o novo endereço do bloco
            std::streampos startPositionBlock = blockStorage.removeBlockItem(blockAddress, dbAddress);
            // se o endereco mudou, atualiza na trie
            if (startPositionBlock != blockAddress) {
                trieStockBlock.insertString(stockId, startPositionBlock);
            }

            dbContext.remove(dbAddress);
            trieStockPrices.deleteString(stockPriceId);
        }
    }
}

std::vector<Model::StockPrice> getStockPriceList(std::string stockId, int pageSize, int page) {
    mainLogger.pushScope("getStockPriceList");
    mainLogger.log("stockId: " + stockId);
    mainLogger.log("pageSize: " + std::to_string(pageSize));
    mainLogger.log("page: " + std::to_string(page));

    Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
    Index::Trie trieStockBlock(Database::PATH::TRIE::STOCK_ID_TO_STOCK_PRICE_BLOCK);
    Index::BlockStorage blockStorage(Database::PATH::BLOCK::STOCK_ID_TO_STOCK_PRICE);

    // Busca na trie de stockBlock o endereço do bloco que contém o item
    std::vector<std::streampos> trieStockBlockAddress = trieStockBlock.searchString(stockId, pageSize, page);
    if (trieStockBlockAddress.size() > 0) {
        std::streampos blockAddress = trieStockBlockAddress[0];
        // Busca o bloco que contém os endereços no db
        std::vector<std::streampos> dbAddressess = blockStorage.retrieveBlock(blockAddress);
        // Busca os itens no db e salva em stockPrices
        std::vector<Model::StockPrice> stockPrices;
        for (std::streampos address : dbAddressess) {
            Database::Record<Model::StockPrice> stock = dbContext.read(address);
            stockPrices.push_back(stock.value);
        }

        mainLogger.popScope();
        return stockPrices;
    }

    mainLogger.popScope();
    return std::vector<Model::StockPrice>();
}

Model::StockPrice getStockPrice(std::string stockPriceId) {
    Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
    Index::Trie trieStockPrices(Database::PATH::TRIE::STOCK_PRICE_ID_TO_STOCK_PRICE);
    // Busca na trie de stockPrices o endereço do item no db
    std::vector<std::streampos> positions = trieStockPrices.searchString(stockPriceId, 1, 0);
    if (positions.size() > 0) {
        std::streampos position = positions[0];
        Database::Record<Model::StockPrice> stockPrice = dbContext.read(position);
        if (stockPrice.value.stockPriceId == stockPriceId) {
            return stockPrice.value;
        }
    }
    return Model::StockPrice();
}

void sortStockPriceList(std::vector<Model::StockPrice> &stockPriceList) {
    std::array<std::queue<Model::StockPrice>, 10> buckets;

    // sort by day
    for (int i = 0; i < 2; i++) {
        for (auto sPrice : stockPriceList)
            buckets.at(sPrice.date.at(9 - i) - '0').push(sPrice);

        int p = 0;
        for (int j = 0; j < 10; j++) {
            while (!buckets.at(j).empty()) {
                stockPriceList.at(p) = buckets.at(j).front();
                buckets.at(j).pop();
                p++;
            }
        }
    }

    // sort by month
    for (int i = 0; i < 2; i++) {
        for (auto sPrice : stockPriceList)
            buckets.at(sPrice.date.at(6 - i) - '0').push(sPrice);

        int p = 0; // position in vector
        for (int j = 0; j < 10; j++) {
            while (!buckets.at(j).empty()) {
                stockPriceList.at(p) = buckets.at(j).front();
                buckets.at(j).pop();
                p++;
            }
        }
    }

    // sort by year
    for (int i = 0; i < 4; i++) {
        for (auto sPrice : stockPriceList)
            buckets.at(sPrice.date.at(3 - i) - '0').push(sPrice);

        int p = 0; // position in vector
        for (int j = 0; j < 10; j++) {
            while (!buckets.at(j).empty()) {
                stockPriceList.at(p) = buckets.at(j).front();
                buckets.at(j).pop();
                p++;
            }
        }
    }
}

} // namespace IndexSearch
} // namespace Controller