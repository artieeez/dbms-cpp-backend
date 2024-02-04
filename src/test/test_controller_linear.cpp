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
    Controller::addStock(stock);

    // Test getStock
    Model::Stock stock2 = Controller::getStock("AAPL");
    std::cout << "Stock: " << stock2.stockId << " " << std::endl;

    return 0;
}

