#include "context.hpp"
#include "linearSearchController.hpp"
#include "indexController.hpp"
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
    std::vector<Model::StockPrice> stockPrices;

    char stockId[MAX_SIZE_STOCK] = "MMXM3";
    stocks = Controller::LinearSearch::getStockList(stockId, 0, 10);

    for (auto stock : stocks) {
        std::cout << stock.stockId << std::endl;
    }

    if (stocks.size() == 0) {
        std::cout << "No stock found\n" << std::endl;
    } else {
        std::cout << "Stock found\n" << std::endl;
    }

    stockPrices = Controller::LinearSearch::getStockPriceList(stockId, 0, 10);
    // stockPrices = Controller::IndexSearch::getStockPriceList(stockId, 0, 10);

    std::cout << "page 1" << std::endl;
    for (auto stockPrice : stockPrices) {
        std::cout << stockPrice.stockId << " - date: " << stockPrice.date << std::endl;
    }

    std::cout << "page 2" << std::endl;
    stockPrices = Controller::LinearSearch::getStockPriceList(stockId, 1, 10);
    // stockPrices = Controller::IndexSearch::getStockPriceList(stockId, 1, 10);
    for (auto stockPrice : stockPrices) {
        std::cout << stockPrice.stockId << " - date: " << stockPrice.date << std::endl;
    }

    if (stockPrices.size() == 0) {
        std::cout << "No stock price found\n" << std::endl;
    } else {
        std::cout << "Stock price found\n" << std::endl;
    }

    mainLogger.popScope();
}
