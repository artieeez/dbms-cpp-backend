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
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    Index::Trie trie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    std::vector<std::streampos> positions = trie.searchString(prefix, pageSize, page);
    std::vector<Model::Stock> stocks;
    for (std::streampos position : positions) {
        Database::Record<Model::Stock> stock = dbContext.read(position);
        stocks.push_back(stock.value);
    }
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
        return stockPrices;
    }
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

void resetDb() {
    Database::Context<Model::StockPrice> dbContextStock(Database::PATH::DB::STOCK);
    Database::Context<Model::Stock> dbContextStockPrice(Database::PATH::DB::STOCK_PRICE);
    Database::Context<Model::Stock> dbContextStockTrie(Database::PATH::TRIE::STOCK_ID_TO_STOCK);
    Database::Context<Model::Stock> dbContextStockPriceTrie(Database::PATH::TRIE::STOCK_PRICE_ID_TO_STOCK_PRICE);
    Database::Context<Model::Stock> dbContextStockPriceTrieBlock(Database::PATH::TRIE::STOCK_ID_TO_STOCK_PRICE_BLOCK);
    Database::Context<Model::Stock> dbContextStockBlock(Database::PATH::BLOCK::STOCK_ID_TO_STOCK_PRICE);
    Database::Context<std::streampos> loaderDb(Database::PATH::LOADER::POSITION);
    Database::Context<int> stockCountDb(Database::PATH::LOADER::STOCK);
    Database::Context<int> stockPriceCountDb(Database::PATH::LOADER::STOCK_PRICE);

    dbContextStock.reset();
    dbContextStockPrice.reset();
    dbContextStockTrie.reset();
    dbContextStockPriceTrie.reset();
    dbContextStockPriceTrieBlock.reset();
    dbContextStockBlock.reset();
    loaderDb.reset();
    stockCountDb.reset();
    stockPriceCountDb.reset();
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

// return the number of lines read.
std::vector<int> loadDb(int pageSize) {
    std::ifstream fileStockPrice{"bovespa_stocks.csv"};
    assert(fileStockPrice.is_open());

    Database::Context<std::streampos> loaderDb(Database::PATH::LOADER::POSITION);
    Database::Context<int> stockCountDb(Database::PATH::LOADER::STOCK);
    Database::Context<int> stockPriceCountDb(Database::PATH::LOADER::STOCK_PRICE);

    if (loaderDb.isEmpty())
        loaderDb.append(0);

    if (stockCountDb.isEmpty())
        stockCountDb.append(0);

    if (stockPriceCountDb.isEmpty())
        stockPriceCountDb.append(0);

    auto recPos = loaderDb.read(0);
    auto recStockCount = stockCountDb.read(0);
    auto recStockPriceCount = stockPriceCountDb.read(0);

    std::cout << recPos.value << std::endl;

    fileStockPrice.seekg(recPos.value);
    for (int i = 0; i < pageSize; i++) {
        Model::StockPrice sPrice;
        std::string line;

        // finished to read the file
        if (fileStockPrice.eof()) {
            recPos.value = fileStockPrice.tellg();
            loaderDb.save(recPos);
            return {0, 0};
        }

        std::getline(fileStockPrice, line);

        sPrice = getStockPriceFromLine(line);

        std::vector<Model::Stock> sList = Controller::IndexSearch::getStockList(sPrice.stockId, 1, 0);

        if (sList.size() == 0) {
            Model::Stock s;
            s.stockId = sPrice.stockId;
            recStockCount.value++;
            Controller::IndexSearch::addStock(s);
        }

        recStockPriceCount.value++;
        Controller::IndexSearch::addStockPrice(sPrice);
    }

    recPos.value = fileStockPrice.tellg();

    std::cout << recPos.value << std::endl;

    loaderDb.save(recPos);
    stockCountDb.save(recStockCount);
    stockPriceCountDb.save(recStockPriceCount);

    // check if is end of file
    if (fileStockPrice.eof())
        return {0, 0};

    return {recStockCount.value, recStockPriceCount.value};
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

} // namespace IndexSearch
} // namespace Controller