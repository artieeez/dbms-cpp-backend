#include <string>
#include <vector>
#include <queue>
#include <array>
#include <fstream>
#include <cassert>
#include "linearSearchController.hpp"
#include "context.hpp"
#include "stock.hpp"
#include "stockPrice.hpp"
#include "indexController.cpp"

namespace Controller {

namespace LinearSearch {

void addStock(Model::Stock payload) {
    Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
    dbContext.append(payload);
}

void deleteStock(std::string stockId) {
    Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
    std::vector<Database::Record<Model::Stock>> stocks = dbContext.find([stockId](Database::Record<Model::Stock> record) {
        return record.value.stockId == stockId;
    });

    if (stocks.size() > 0) {
        dbContext.remove(stocks[0].position);
    }
}

void updateStock(Model::Stock payload) {
    Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
    std::vector<Database::Record<Model::Stock>> stocks = dbContext.find([payload](Database::Record<Model::Stock> record) {
        return record.value.stockId == payload.stockId;
    });

    if (stocks.size() > 0) {
        Database::Record<Model::Stock> stock = stocks[0];
        stock.value = payload;
        dbContext.save(stock);
    }
}

Model::Stock getStock(std::string stockId) {
    Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
    std::vector<Database::Record<Model::Stock>> stocks = dbContext.find([stockId](Database::Record<Model::Stock> record) {
        return record.value.stockId == stockId;
    });

    if (stocks.size() > 0) {
        return stocks[0].value;
    }

    return Model::Stock();
}

std::vector<Model::Stock> getStockList(std::string prefix, int page, int pageSize, std::string orderBy) {
    Database::Context<Model::Stock> dbContext(STOCK_DB_FILE);
    std::vector<Database::Record<Model::Stock>> records = dbContext.find([prefix](Database::Record<Model::Stock> record) {
        return record.value.stockId.find(prefix) != std::string::npos;
    });

    // map records to stocks
    std::vector<Model::Stock> result;
    for (Database::Record<Model::Stock> record : records) {
        result.push_back(record.value);
    }

    return result;
}

void addStockPrice(Model::StockPrice payload) {
    Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
    dbContext.append(payload);
}

void deleteStockPrice(std::string stockPriceId) {
    Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
    std::vector<Database::Record<Model::StockPrice>> stockPrices = dbContext.find([stockPriceId](Database::Record<Model::StockPrice> record) {
        return record.value.stockPriceId == stockPriceId;
    });

    if (stockPrices.size() > 0) {
        dbContext.remove(stockPrices[0].position);
    }
}

void updateStockPrice(Model::StockPrice payload) {
    Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
    std::vector<Database::Record<Model::StockPrice>> stockPrices = dbContext.find([payload](Database::Record<Model::StockPrice> record) {
        return record.value.stockPriceId == payload.stockPriceId;
    });

    if (stockPrices.size() > 0) {
        Database::Record<Model::StockPrice> stockPrice = stockPrices[0];
        stockPrice.value = payload;
        dbContext.save(stockPrice);
    }
}

std::vector<Model::StockPrice> getStockPriceList(std::string stockId, std::string minDate, std::string maxDate, std::string orderBy) {

    Database::Context<Model::StockPrice> dbContext(STOCK_PRICE_DB_FILE);
    std::vector<Database::Record<Model::StockPrice>> records = dbContext.find([stockId](Database::Record<Model::StockPrice> record) {
        return record.value.stockId == stockId;
    });

    // map records to stock prices
    std::vector<Model::StockPrice> result;
    for (Database::Record<Model::StockPrice> record : records) {
        result.push_back(record.value);
    }

    sortStockPriceList(result);

    return result;
}

void resetDb() {
    Database::Context<Model::Stock> stockDbContext(STOCK_DB_FILE);
    Database::Context<Model::Stock> stockPriceDbContext(STOCK_PRICE_DB_FILE);
    stockDbContext.reset();
    stockPriceDbContext.reset();
}

void sortStockPriceList(std::vector<Model::StockPrice> stockPriceList) {
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
int loadDb(int pageSize) {
    std::ifstream fileStockPrice {"./../raw_data/bovespa_stocks.csv"};
    assert(fileStockPrice.is_open());

    Database::Context<std::streampos> loaderDb (Controller::LinearSearch::LOADER_DB_FILE_PATH);

    auto recPos = loaderDb.read(0);

    fileStockPrice.seekg(recPos.value);
    for (int i = 0; i < pageSize; i++) {
        Model::StockPrice sPrice;
        std::string line;

        // finished to read the file
        if (fileStockPrice.eof()) {
            recPos.value = fileStockPrice.tellg();
            loaderDb.save(recPos);
            return i;
        }

        std::getline(fileStockPrice, line);

        sPrice = getStockPriceFromLine(line);

        std::vector<Model::Stock> sList = Controller::IndexSearch::getStockList(sPrice.stockId, 1, 1);

        if (sList.size() == 0) {
            Model::Stock s;
            s.stockId = sPrice.stockId;
            Controller::IndexSearch::addStock(s);
        }

        Controller::IndexSearch::addStockPrice(sPrice);
    }

    recPos.value = fileStockPrice.tellg();

    loaderDb.save(recPos);

    return pageSize;
}

inline std::string getSymbolFromLine(std::string line) {
    int i {11};
    int j {i};
    while (line.at(j) != ',')
        j++;

    return line.substr(i, j - i);
}

inline std::string getDateFromLine(std::string line) {
    return line.substr(0, 10);
}

Model::StockPrice getStockPriceFromLine(std::string line) {
    // string position starts in 11 to skip date
    int begin {11};
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


} // namespace LinearSearch

} // namespace Controller
