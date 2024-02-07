#include "context.hpp"
#include "indexController.hpp"
#include "stock.hpp"
#include "stockPrice.hpp"
#include <iomanip> // std::setw
#include <iostream>
#include <string>
#include <vector>

using namespace Controller::IndexSearch;

void test_controller_index()
{

  // Test addStock
  Model::Stock stock;
  stock.stockId = "aapl";
  stock.max_date = "2020-01-01";
  stock.min_date = "2019-01-01";
  stock.companyId = "asdf";
  addStock(stock);

  // Test addStock
  Model::Stock stock2;
  stock2.stockId = "apple";
  stock2.max_date = "2020-01-01";
  stock2.min_date = "2019-01-01";
  stock2.companyId = "bueno";
  addStock(stock2);

  // Test getStock
  std::cout << "\n==========getStock==============" << std::endl;
  Model::Stock stock3 = getStock("aapl");
  std::cout << "Stock: " << stock3.stockId << " " << std::endl;
  Model::Stock stock4 = getStock("apple");
  std::cout << "Stock: " << stock4.stockId << " " << std::endl;

  // Test getStockList
  std::cout << "\n==============getStockList==============" << std::endl;
  std::vector<Model::Stock> stocks = getStockList("a", 1, 10);
  for (Model::Stock s : stocks)
  {
    std::cout << "Stock: " << s.stockId << " " << std::endl;
  }

  // Test updateStock
  std::cout << "\n=============updateStock================= " << std::endl;
  // initial max_date
  std::cout << "Stock: " << stock3.stockId << " " << stock3.max_date << std::endl;
  std::cout << "update max_date to 2021-01-01 ..." << std::endl;
  stock3.max_date = "2021-01-01";
  updateStock(stock3);
  std::cout << "stock updated." << std::endl;
  // updated max_date
  stock3 = getStock(stock3.stockId);
  std::cout << "Stock: " << stock3.stockId << " " << stock3.max_date << std::endl;

  // Test deleteStock
  std::cout << "\n=================deleteStock================" << std::endl;
  deleteStock("apple");
  std::cout << "stock deleted." << std::endl;
  std::cout << "getStockList " << std::endl;
  stocks = getStockList("a", 0, 10);
  for (Model::Stock s : stocks)
  {
    std::cout << "Stock: " << s.stockId << " " << std::endl;
  }

  std::cout << "\n=================add stock price================" << std::endl;
  // Test addStockPrice
  Model::StockPrice stockPrice;
  stockPrice.stockPriceId = "qwerty";
  stockPrice.stockId = "aapl";
  stockPrice.date = "2020-01-01";
  stockPrice.open = 100;
  stockPrice.high = 110;
  stockPrice.low = 90;
  stockPrice.close = 105;
  stockPrice.volume = 1000000;
  addStockPrice(stockPrice);
  std::cout << "stock price added." << std::endl;

  // Test addStockPrice
  Model::StockPrice stockPrice2;
  stockPrice2.stockPriceId = "isaac";
  stockPrice2.stockId = "aapl";
  stockPrice2.date = "2020-01-01";
  stockPrice2.open = 100;
  stockPrice2.high = 110;
  stockPrice2.low = 90;
  stockPrice2.close = 105;
  stockPrice2.volume = 1000000;
  addStockPrice(stockPrice2);
  std::cout << "stock price added." << std::endl;

  std::cout << "\n=============get stock prices ========== " << std::endl;
  std::vector<Model::StockPrice> stockPrices = getStockPriceList("aapl", 1, 1);
  for (Model::StockPrice stockPrice : stockPrices)
  {
    std::cout << "StockPrice: " << stockPrice.stockPriceId << " " << stockPrice.date << " " << stockPrice.open << " " << stockPrice.high << " " << stockPrice.low << " " << stockPrice.close << " " << stockPrice.volume << std::endl;
  }

  std::cout << "\n=============get stock price========== " << std::endl;
  Model::StockPrice stockPrice4 = getStockPrice("qwerty");
  std::cout << "get stock price: " << stockPrice4.stockPriceId << " " << stockPrice4.date << " " << stockPrice4.open << " " << stockPrice4.high << " " << stockPrice4.low << " " << stockPrice4.close << " " << stockPrice4.volume << std::endl;
  // Test deleteStockPrice
  std::cout << "\n==========deleteStockPrice==============" << std::endl;
  deleteStockPrice("qwerty", "aapl");
  std::cout << "stockPrice deleted." << std::endl;
  std::cout << "getStockPriceList " << std::endl;
  stockPrices = getStockPriceList("aapl", 0, 10);
  for (Model::StockPrice stockPrice : stockPrices)
  {
    std::cout << "StockPrice: " << stockPrice.stockPriceId << " " << stockPrice.date << " " << stockPrice.open << " " << stockPrice.high << " " << stockPrice.low << " " << stockPrice.close << " " << stockPrice.volume << std::endl;
  }

  // Test resetDb
  resetDb();
}
