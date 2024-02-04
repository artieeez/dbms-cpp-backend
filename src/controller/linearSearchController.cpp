#include "linearSearchController.hpp";
#include "context.hpp";
#include "stock.hpp";
#include "stockPrice.hpp";
#include <string>;
#include <vector>;

namespace Controller {

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

    //map records to stocks
    std::vector<Model::Stock> result;
    for (Database::Record<Model::Stock> record : records) {
        result.push_back(record.value);
    }

    return result;
}

void addStockPrice(Model::StockPrice payload) {
    Database::Context<Model::StockPrice> dbContext(STOCK_DB_FILE);
    dbContext.append(payload);
}

void deleteStockPrice(std::string stockPriceId) {
    Database::Context<Model::StockPrice> dbContext(STOCK_DB_FILE);
    std::vector<Database::Record<Model::StockPrice>> stockPrices = dbContext.find([stockPriceId](Database::Record<Model::StockPrice> record) {
        return record.value.stockPriceId == stockPriceId;
    });

    if (stockPrices.size() > 0) {
        dbContext.remove(stockPrices[0].position);
    }
}

void updateStockPrice(Model::StockPrice payload) {
    Database::Context<Model::StockPrice> dbContext(STOCK_DB_FILE);
    std::vector<Database::Record<Model::StockPrice>> stockPrices = dbContext.find([payload](Database::Record<Model::StockPrice> record) {
        return record.value.stockPriceId == payload.stockPriceId;
    });

    if (stockPrices.size() > 0) {
        Database::Record<Model::StockPrice> stockPrice = stockPrices[0];
        stockPrice.value = payload;
        dbContext.save(stockPrice);
    }
}

// std::vector<Model::StockPrice> getStockPriceList(std::string stockId, std::string minDate, std::string maxDate) {
//     Database::Context<Model::StockPrice> dbContext(STOCK_DB_FILE);
//     std::vector<Database::Record<Model::StockPrice>> records = dbContext.find([stockId](Database::Record<Model::StockPrice> record) {
//         return record.value.stockId == stockId;
//     });

//     //map records to stock prices
//     std::vector<Model::StockPrice> result;
//     for (Database::Record<Model::StockPrice> record : records) {
//         result.push_back(record.value);
//     }

//     return result;
// }

} // namespace Controller
