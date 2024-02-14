#include "indexController.hpp"
#include "blockStorage.hpp"
#include "context.hpp"
#include "logger.hpp"
#include "stock.hpp"
#include "trie.hpp"
#include <array>
#include <cassert>
#include <queue>
#include <string>
#include <vector>

extern Logger mainLogger;

namespace Controller {
namespace IndexSearch {
void addStock(Model::Stock payload) {
    mainLogger.pushScope("Controller::IndexSearch::addStock");
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::streampos position = dbContext.append(payload);
    trie.insertString(payload.stockId, position);
    mainLogger.popScope();
}

void deleteStock(char stockId[MAX_SIZE_STOCK]) {
    mainLogger.pushScope("Controller::IndexSearch::deleteStock");
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(stockId, 1, 0);
    if (positions.size() > 0) {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stocks = dbContext.read(position);
        if (strcmp(stocks.value.stockId, stockId) == 0) {
            dbContext.remove(position);
            trie.deleteString(stockId);
            mainLogger.log("Stock " + std::string(stockId) + " deleted");
        } else {
            mainLogger.log("Stock " + std::string(stockId) + " not found", LogType::ERROR);
        }
    } else {
        mainLogger.log("Stock " + std::string(stockId) + " not found", LogType::ERROR);
    }
    mainLogger.popScope();
}

void updateStock(Model::Stock payload) {
    mainLogger.pushScope("Controller::IndexSearch::updateStock");
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(payload.stockId, 1, 0);
    if (positions.size() > 0) {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stock = dbContext.read(position);
        if (strcmp(stock.value.stockId, payload.stockId) == 0) {
            stock.value = payload;
            dbContext.save(stock);
            mainLogger.log("Stock " + std::string(payload.stockId) + " updated");
        } else {
            mainLogger.log("Stock " + std::string(payload.stockId) + " not found", LogType::ERROR);
        }
    } else {
        mainLogger.log("Stock " + std::string(payload.stockId) + " not found", LogType::ERROR);
    }
    mainLogger.popScope();
}

Model::Stock getStock(char stockId[MAX_SIZE_STOCK]) {
    mainLogger.pushScope("Controller::IndexSearch::getStock");
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(stockId, 1, 0);
    if (positions.size() > 0) {
        std::streampos position = positions[0];
        Database::Record<Model::Stock> stock = dbContext.read(position);
        if (strcmp(stock.value.stockId, stockId) == 0) {
            mainLogger.log("Found stock: " + std::string(stock.value.stockId));
            return stock.value;
        }
    }
    mainLogger.log("Stock " + std::string(stockId) + " not found", LogType::ERROR);
    mainLogger.popScope();
    return Model::Stock();
}

std::vector<Model::Stock> getStockList(char prefix[MAX_SIZE_STOCK], int pageNumber, int pageSize) {
    mainLogger.pushScope("getStockList");
    mainLogger.log("prefix: " + std::string(prefix));
    mainLogger.log("pageNumber: " + std::to_string(pageNumber));
    mainLogger.log("pageSize: " + std::to_string(pageSize));

    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    auto positions = trie.searchString(prefix, pageSize, pageNumber);
    std::vector<Model::Stock> stocks;

    for (auto position : positions) {
        auto stock = dbContext.read(position);
        stocks.push_back(stock.value);
    }

    // log stocks
    if (stocks.size() == 0) {
        mainLogger.log("No stocks found");
    } else {
        mainLogger.log("Found " + std::to_string(stocks.size()) + " stocks");
    }
    mainLogger.popScope();
    return stocks;
}

///////////////////STOCKPRICE////////////////////

void addStockPrice(Model::StockPrice payload) {
    mainLogger.pushScope("Controller::IndexSearch::addStockPrice");
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
    mainLogger.popScope();
}

void deleteStockPrice(char stockPriceId[MAX_SIZE_SP], char stockId[MAX_SIZE_STOCK]) {
    mainLogger.pushScope("Controller::IndexSearch::deleteStockPrice");
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
            mainLogger.log("StockPrice " + std::string(stockPriceId) + " deleted");
            mainLogger.popScope();
            return;
        }
    }

    mainLogger.log("StockPrice " + std::string(stockPriceId) + " not found", LogType::ERROR);
    mainLogger.popScope();
}

void sortStockPriceList(std::vector<Model::StockPrice> &stockPriceList) {
    std::array<std::queue<Model::StockPrice>, 10> buckets;

    // sort by day
    for (int i = 0; i < 2; i++) {
        for (auto sPrice : stockPriceList) {
            std::string date(sPrice.date);
            buckets.at(date.at(9 - i) - '0').push(sPrice);
        }

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
        for (auto sPrice : stockPriceList) {
            std::string date(sPrice.date);
            buckets.at(date.at(6 - i) - '0').push(sPrice);
        }

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
        for (auto sPrice : stockPriceList) {
            std::string date(sPrice.date);
            buckets.at(date.at(3 - i) - '0').push(sPrice);
        }

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

std::vector<Model::StockPrice> getStockPriceList(char stockId[MAX_SIZE_STOCK], int pageNumber, int pageSize) {
    mainLogger.pushScope("Controller::IndexSearch::getStockPriceList");
    mainLogger.log("stockId: " + std::string(stockId));
    mainLogger.log("page: " + std::to_string(pageNumber));
    mainLogger.log("pageSize: " + std::to_string(pageSize));

    Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
    Index::Trie trieStockBlock(Database::PATH::TRIE::STOCK_ID_TO_STOCK_PRICE_BLOCK);
    Index::BlockStorage blockStorage(Database::PATH::BLOCK::STOCK_ID_TO_STOCK_PRICE);

    // Busca na trie de stockBlock o endereço do bloco que contém o item
    std::vector<std::streampos> trieStockBlockAddress = trieStockBlock.searchString(stockId, pageSize, pageNumber);
    if (trieStockBlockAddress.size() > 0) {
        std::streampos blockAddress = trieStockBlockAddress[0];
        // Busca o bloco que contém os endereços no db
        std::vector<std::streampos> dbAddressess = blockStorage.retrieveBlock(blockAddress);
        // Busca os itens no db e salva em stockPrices
        std::vector<Model::StockPrice> stockPrices;
        int start = pageNumber * pageSize;
        int end = std::min((pageNumber + 1) * pageSize, static_cast<int>(dbAddressess.size()));
        for (int i = start; i < end; i++) {
            std::streampos dbAddress = dbAddressess[i];
            Database::Record<Model::StockPrice> stock = dbContext.read(dbAddress);
            stockPrices.push_back(stock.value);
        }

        // sortStockPriceList(stockPrices);

        mainLogger.log("Found " + std::to_string(stockPrices.size()) + " stock prices");
        mainLogger.popScope();
        return stockPrices;
    }

    mainLogger.log("StockPrice " + std::string(stockId) + " not found", LogType::ERROR);
    mainLogger.popScope();
    return std::vector<Model::StockPrice>();
}
} // namespace IndexSearch
} // namespace Controller