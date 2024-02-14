#include "stateController.hpp"
#include "blockStorage.hpp"
#include "context.hpp"
#include "indexController.hpp"
#include "logger.hpp"
#include "stock.hpp"
#include "trie.hpp"
#include <array>
#include <cassert>
#include <filesystem>
#include <queue>
#include <string>
#include <vector>

extern Logger mainLogger;
namespace Controller {
namespace State {

Database::State getDatabaseState() {
    mainLogger.pushScope("getDatabaseState");
    Database::Context<Database::State> stateContext(Database::PATH::LOADER::STATE);

    if (stateContext.isEmpty()) {
        mainLogger.log("stateContext is empty. Creating a new one.");
        Database::State dbStateEntity;
        dbStateEntity.isFinished = false;
        dbStateEntity.loaderPosition = 0;
        dbStateEntity.stockCount = 0;
        dbStateEntity.stockPriceCount = 0;
        stateContext.append(dbStateEntity);
    }

    auto stateRecord = stateContext.read(0);

    mainLogger.log("stockCount: " + std::to_string(stateRecord.value.stockCount));
    mainLogger.log("stockPriceCount: " + std::to_string(stateRecord.value.stockPriceCount));
    mainLogger.log("isFinished: " + std::to_string(stateRecord.value.isFinished));
    mainLogger.log("loaderPosition: " + std::to_string(stateRecord.value.loaderPosition));

    mainLogger.popScope();
    return stateRecord.value;
}

void resetDb() {
    mainLogger.pushScope("resetDb");

    Database::Context<Model::StockPrice> dbContextStock(Database::PATH::DB::STOCK);
    Database::Context<Model::Stock> dbContextStockPrice(Database::PATH::DB::STOCK_PRICE);
    Database::Context<Model::Stock> dbContextStockTrie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    Database::Context<Model::Stock> dbContextStockPriceTrie(Database::PATH::TRIE::STOCK_PRICE_ID_TO_STOCK_PRICE);
    Database::Context<Model::Stock> dbContextStockPriceTrieBlock(Database::PATH::TRIE::STOCK_ID_TO_STOCK_PRICE_BLOCK);
    Database::Context<Model::Stock> dbContextStockBlock(Database::PATH::BLOCK::STOCK_ID_TO_STOCK_PRICE);
    Database::Context<std::streampos> dbState(Database::PATH::LOADER::STATE);

    dbContextStock.reset();
    dbContextStockPrice.reset();
    dbContextStockTrie.reset();
    dbContextStockPriceTrie.reset();
    dbContextStockPriceTrieBlock.reset();
    dbContextStockBlock.reset();
    dbState.reset();

    mainLogger.log("Database reseted successfully");
    mainLogger.popScope();
}

inline std::string getSymbolFromLine(std::string line) {
    int i{11};
    int j{i};
    while (line.at(j) != ',')
        j++;

    return line.substr(i, j - i);
}

inline std::string getDateFromLine(std::string line) {
    return line.substr(0, 10);
}

Model::StockPrice getStockPriceFromLine(std::string line) {
    mainLogger.pushScope("getStockPriceFromLine");
    mainLogger.log("line: " + line);

    // string position starts in 11 to skip date
    int begin{11};
    int end = {begin};
    Model::StockPrice sPrice;

    strcpy(sPrice.date, getDateFromLine(line).c_str());
    strcpy(sPrice.stockId, getSymbolFromLine(line).c_str());
    strcpy(sPrice.stockPriceId, sPrice.stockId);
    strcat(sPrice.stockPriceId, sPrice.date);

    // skip symbol
    while (line.at(end) != ',')
        end++;

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.adj = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.close = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.high = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.low = std::stof(line.substr(begin, end - begin));

    end++;
    begin = end;
    while (line.at(end) != ',')
        end++;
    sPrice.open = std::stof(line.substr(begin, end - begin));

    sPrice.volume = std::stof(line.substr(end + 1, line.size() - end - 1));

    std::string stockIdStr(sPrice.stockId);
    std::string stockDateStr(sPrice.date);
    std::string stockPriceIdStr(sPrice.stockPriceId);
    mainLogger.log("stockId: " + stockIdStr);
    mainLogger.log("stockDate: " + stockDateStr);
    mainLogger.log("stockPriceId: " + stockPriceIdStr);
    mainLogger.log("adj: " + std::to_string(sPrice.adj));
    mainLogger.log("close: " + std::to_string(sPrice.close));
    mainLogger.log("high: " + std::to_string(sPrice.high));
    mainLogger.log("low: " + std::to_string(sPrice.low));
    mainLogger.log("open: " + std::to_string(sPrice.open));
    mainLogger.log("volume: " + std::to_string(sPrice.volume));

    mainLogger.popScope();

    return sPrice;
}

// return the number of lines read.
Database::State loadDb(int pageSize) {
    mainLogger.pushScope("loadDb");
    mainLogger.log("loading " + std::to_string(pageSize) + " records");
    auto path = std::filesystem::absolute("bovespa_stocks.csv");
    mainLogger.log("file path: " + path.string());

    std::ifstream fileStockPrice{"bovespa_stocks.csv"};
    assert(fileStockPrice.is_open());

    Database::Context<Database::State> stateContext(Database::PATH::LOADER::STATE);

    if (stateContext.isEmpty()) {
        mainLogger.log("stateContext is empty. Creating a new one.");
        Database::State dbStateEntity;
        dbStateEntity.isFinished = false;
        dbStateEntity.loaderPosition = 0;
        dbStateEntity.stockCount = 0;
        dbStateEntity.stockPriceCount = 0;
        stateContext.append(dbStateEntity);
    }

    auto stateRecord = stateContext.read(0);

    fileStockPrice.seekg(stateRecord.value.loaderPosition);
    for (int i = 0; i < pageSize; i++) {
        Model::StockPrice sPrice;
        std::string line;

        // finished to read the file
        if (fileStockPrice.eof()) {
            stateRecord.value.loaderPosition = fileStockPrice.tellg();
            stateRecord.value.isFinished = true;
            stateContext.save(stateRecord);
            return stateRecord.value;
        }

        std::getline(fileStockPrice, line);

        sPrice = getStockPriceFromLine(line);

        std::vector<Model::Stock> sList = Controller::IndexSearch::getStockList(sPrice.stockId, 1, 0);

        if (sList.size() == 0) {
            Model::Stock s;
            strcpy(s.stockId, sPrice.stockId);
            stateRecord.value.stockCount++;
            Controller::IndexSearch::addStock(s);
        }

        stateRecord.value.stockPriceCount++;
        Controller::IndexSearch::addStockPrice(sPrice);
    }

    stateRecord.value.loaderPosition = fileStockPrice.tellg();

    mainLogger.log("loaderPosition: " + std::to_string(stateRecord.value.loaderPosition));

    if (fileStockPrice.eof()) {
        stateRecord.value.isFinished = true;
    }

    stateContext.save(stateRecord);

    mainLogger.popScope();

    return stateRecord.value;
}

} // namespace State
} // namespace Controller