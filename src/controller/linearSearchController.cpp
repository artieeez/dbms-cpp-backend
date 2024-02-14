#include "linearSearchController.hpp"
#include "context.hpp"
#include "stock.hpp"
#include "stockPrice.hpp"
#include <array>
#include <cassert>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include "logger.hpp"

extern Logger mainLogger;
namespace Controller {

namespace LinearSearch {

Model::Stock getStock(char stockId[MAX_SIZE_STOCK]) {
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    std::vector<Database::Record<Model::Stock>> stocks = dbContext.find([stockId](Database::Record<Model::Stock> record) { return strcmp(record.value.stockId, stockId) == 0; });

    if (stocks.size() > 0) {
        return stocks[0].value;
    }

    return Model::Stock();
}

std::vector<Model::Stock> getStockList(char prefix[MAX_SIZE_STOCK], int pageNumber, int pageSize) {
    std::string prefixStr(prefix);
    Database::Context<Model::Stock> dbContext(Database::PATH::DB::STOCK);
    std::vector<Database::Record<Model::Stock>> records = dbContext.find(
        [prefixStr](Database::Record<Model::Stock> record) {
            return strncmp(record.value.stockId, prefixStr.c_str(), MAX_SIZE_STOCK) == 0;
        },
        pageSize, pageNumber);

    // map records to stocks
    std::vector<Model::Stock> result;
    for (Database::Record<Model::Stock> record : records) {
        result.push_back(record.value);
    }

    return result;
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
    mainLogger.pushScope("getStockPriceList");
    mainLogger.log("stockId: " + std::string(stockId));
    mainLogger.log("page: " + std::to_string(pageNumber));
    mainLogger.log("pageSize: " + std::to_string(pageSize));

    Database::Context<Model::StockPrice> dbContext(Database::PATH::DB::STOCK_PRICE);
    std::vector<Database::Record<Model::StockPrice>> records = dbContext.find(
        [stockId](Database::Record<Model::StockPrice> record) {
            return strcmp(record.value.stockId, stockId) == 0;
        },
        pageSize, pageNumber);

    // map records to stock prices
    std::vector<Model::StockPrice> result;
    for (Database::Record<Model::StockPrice> record : records) {
        result.push_back(record.value);
    }

    // sortStockPriceList(result);

    mainLogger.popScope();
    return result;
}
} // namespace LinearSearch
} // namespace Controller