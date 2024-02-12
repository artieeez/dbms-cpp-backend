#include "stateController.hpp"
#include "blockStorage.hpp"
#include "context.hpp"
#include "indexController.hpp"
#include "stock.hpp"
#include "trie.hpp"
#include <array>
#include <cassert>
#include <queue>
#include <string>
#include <vector>

namespace Controller {
namespace State {

Database::State getDatabaseState() {
    Database::Context<Database::State> stateContext(Database::PATH::LOADER::STATE);

    if (stateContext.isEmpty()) {
        Database::State dbStateEntity;
        dbStateEntity.isFinished = false;
        dbStateEntity.loaderPosition = 0;
        dbStateEntity.stockCount = 0;
        dbStateEntity.stockPriceCount = 0;
        stateContext.append(dbStateEntity);
    }

    auto stateRecord = stateContext.read(0);

    return stateRecord.value;
}

void resetDb() {
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
    // string position starts in 11 to skip date
    int begin{11};
    int end = {begin};
    Model::StockPrice sPrice;

    sPrice.date = getDateFromLine(line);
    sPrice.stockId = getSymbolFromLine(line);
    sPrice.stockPriceId = sPrice.stockId + sPrice.date;

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

    /*
    std::cout << "symbol: " << sPrice.stockId << std::endl;
    std::cout << "adj: " << sPrice.adj << std::endl;
    std::cout << "close: " << sPrice.close << std::endl;
    std::cout << "high: " << sPrice.high << std::endl;
    std::cout << "low: " << sPrice.low << std::endl;
    std::cout << "open: " << sPrice.open << std::endl;
    std::cout << "volume: " << sPrice.volume << std::endl;
    */

    return sPrice;
}

// return the number of lines read.
Database::State loadDb(int pageSize) {
    std::ifstream fileStockPrice{"bovespa_stocks.csv"};
    assert(fileStockPrice.is_open());

    Database::Context<Database::State> stateContext(Database::PATH::LOADER::STATE);

    if (stateContext.isEmpty()) {
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
            s.stockId = sPrice.stockId;
            stateRecord.value.stockCount++;
            Controller::IndexSearch::addStock(s);
        }

        stateRecord.value.stockPriceCount++;
        Controller::IndexSearch::addStockPrice(sPrice);
    }

    stateRecord.value.loaderPosition = fileStockPrice.tellg();

    std::cout << stateRecord.value.loaderPosition << std::endl;

    if (fileStockPrice.eof()) {
        stateRecord.value.isFinished = true;
    }

    stateContext.save(stateRecord);

    return stateRecord.value;
}

} // namespace State
} // namespace Controller