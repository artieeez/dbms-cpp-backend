#include <iomanip> // std::setw
#include <iostream>
#include <string>
#include <vector>
#include "linearSearchController.hpp"
#include "context.hpp"
#include "stock.hpp"
#include "stockPrice.hpp"

int main() {
    // Test addStock
    Model::Stock stock;
    stock.stockId = "AAPL";
    stock.max_date = "2020-01-01";
    stock.min_date = "2019-01-01";
    stock.companyId = "1";
    Controller::addStock(stock);

    // Test getStock
    std::cout << "\ngetStock " << std::endl;
    Model::Stock stock2 = Controller::getStock("AAPL");
    std::cout << "Stock: " << stock2.stockId << " " << std::endl;

    // Test getStockList
    std::cout << "\ngetStockList " << std::endl;
    std::vector<Model::Stock> stocks = Controller::getStockList("A", 0, 10, "stockId");
    for (Model::Stock stock : stocks) {
        std::cout << "Stock: " << stock.stockId << " " << std::endl;
    }

    // Test updateStock
    std::cout << "\nupdateStock " << std::endl;
    // initial max_date
    std::cout << "Stock: " << stock2.stockId << " " << stock2.max_date << std::endl;
    std::cout << "update max_date to 2021-01-01 ..." << std::endl;
    stock2.max_date = "2021-01-01";
    Controller::updateStock(stock2);
    std::cout << "stock updated." << std::endl;
    // updated max_date
    stock2 = Controller::getStock("AAPL");
    std::cout << "Stock: " << stock2.stockId << " " << stock2.max_date << std::endl;

    // Test deleteStock
    std::cout << "\ndeleteStock " << std::endl;
    Controller::deleteStock("AAPL");
    std::cout << "stock deleted." << std::endl;
    std::cout << "getStockList " << std::endl;
    stocks = Controller::getStockList("A", 0, 10, "stockId");
    for (Model::Stock stock : stocks) {
        std::cout << "Stock: " << stock.stockId << " " << std::endl;
    }

    // Test addStockPrice
    Model::StockPrice stockPrice;
    stockPrice.stockPriceId = "AAPL_2020-01-01";
    stockPrice.stockId = "AAPL";
    stockPrice.date = "2020-01-01";
    stockPrice.open = 100;
    stockPrice.high = 110;
    stockPrice.low = 90;
    stockPrice.close = 105;
    stockPrice.volume = 1000000;
    Controller::addStockPrice(stockPrice);

    // Test getStockPriceList
    std::cout << "\ngetStockPriceList " << std::endl;
    std::vector<Model::StockPrice> stockPrices = Controller::getStockPriceList("AAPL", "2020-01-01", "2020-01-01");
    for (Model::StockPrice stockPrice : stockPrices) {
        std::cout << "StockPrice: " << stockPrice.stockPriceId << " " << stockPrice.date << " " << stockPrice.open << " " << stockPrice.high << " " << stockPrice.low << " " << stockPrice.close << " " << stockPrice.volume << std::endl;
    }

    // Test updateStockPrice
    std::cout << "\nupdateStockPrice " << std::endl;
    // initial open
    std::cout << "StockPrice: " << stockPrice.stockPriceId << " " << stockPrice.open << std::endl;
    std::cout << "update open to 200 ..." << std::endl;
    stockPrice.open = 200;
    Controller::updateStockPrice(stockPrice);
    std::cout << "stockPrice updated." << std::endl;
    // updated open
    stockPrices = Controller::getStockPriceList("AAPL", "2020-01-01", "2020-01-01");
    for (Model::StockPrice stockPrice : stockPrices) {
        std::cout << "StockPrice: " << stockPrice.stockPriceId << " " << stockPrice.date << " " << stockPrice.open << " " << stockPrice.high << " " << stockPrice.low << " " << stockPrice.close << " " << stockPrice.volume << std::endl;
    }

    // Test deleteStockPrice
    std::cout << "\ndeleteStockPrice " << std::endl;
    Controller::deleteStockPrice("AAPL_2020-01-01");
    std::cout << "stockPrice deleted." << std::endl;
    std::cout << "getStockPriceList " << std::endl;
    stockPrices = Controller::getStockPriceList("AAPL", "2020-01-01", "2020-01-01");
    for (Model::StockPrice stockPrice : stockPrices) {
        std::cout << "StockPrice: " << stockPrice.stockPriceId << " " << stockPrice.date << " " << stockPrice.open << " " << stockPrice.high << " " << stockPrice.low << " " << stockPrice.close << " " << stockPrice.volume << std::endl;
    }

    // Test resetDb
    Controller::resetDb();

    return 0;
}

