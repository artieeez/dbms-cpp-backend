#include "context.hpp"
#include "indexController.hpp"
#include "linearSearchController.hpp"
#include "logger.hpp"
#include "stock.hpp"
#include "stockPrice.hpp"
#include <iomanip> // std::setw
#include <iostream>
#include <string>
#include <vector>

extern Logger mainLogger;

void test_controller_linear() {
    mainLogger.pushScope("test_controller_linear");

    std::vector<Model::Stock> stocks;

    char stockId[MAX_SIZE_STOCK] = "MMXM3";
    stocks = Controller::LinearSearch::getStockList(stockId, 0, 10);
    Database::Context<Model::Stock> stockContext(Database::PATH::DB::STOCK);
    Model::Stock stock;
    strcpy(stock.stockId, stockId);
    strcpy(stock.max_date, "2026-01-01");

    stockContext.clearIterator();
    mainLogger.pushScope("Stocks search 1");
    while(stockContext.next()) {
        auto position = stockContext.curr.position;
        auto stockId = std::string(stockContext.curr.value.stockId);
        auto stockMaxDate = std::string(stockContext.curr.value.max_date);
        auto isDeleted = stockContext.curr.deleted;
        mainLogger.log("Stock: " + stockId + " - MaxDate: " + stockMaxDate + " - Deleted: " + std::to_string(isDeleted) + " - Position: " + std::to_string(position));
    };
    mainLogger.popScope();

    if (stocks.size() == 0) {
        mainLogger.log("Stock not found: " + std::string(stockId), true);
    } else {
        stock = stocks[0];
        mainLogger.log("Stock found: " + std::string(stock.stockId));
        Controller::IndexSearch::deleteStock(stockId);
        mainLogger.log("Stock deleted: " + std::string(stock.stockId));
    }

    // mainLogger.log("Update stock after delete, so it get inserted again");
    // Controller::IndexSearch::addStock(stock);

    stocks = Controller::LinearSearch::getStockList(stockId, 0, 10);
    mainLogger.log("Stocks after delete:");
    for (auto stock : stocks) {
        mainLogger.log("Stock: " + std::string(stock.stockId));
        mainLogger.log("Stock MaxDate: " + std::string(stock.max_date));
    }


    mainLogger.popScope();
}
