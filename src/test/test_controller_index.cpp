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
// int main()
{

  std::ofstream logFile("log.txt");
  std::streambuf *oldClog = std::clog.rdbuf(logFile.rdbuf());
  std::clog << "Testando o log\n";

  // Test addStock
  Model::Stock stock;
  stock.stockId = "ASMAB11";
  stock.max_date = "2020-01-01";
  stock.min_date = "2019-01-01";
  stock.companyId = "asdf";
  addStock(stock);

  // Test addStock
  stock.stockId = "AMMXM3";
  stock.max_date = "2020-01-01";
  stock.min_date = "2019-01-01";
  stock.companyId = "bueno";
  addStock(stock);

  stock.stockId = "APETR3";
  stock.max_date = "2020-01-01";
  stock.min_date = "2019-01-01";
  stock.companyId = "bueno";
  addStock(stock);

  stock.stockId = "AQWERT12";
  stock.max_date = "2020-01-01";
  stock.min_date = "2019-01-01";
  stock.companyId = "bueno";
  addStock(stock);

  // Test getStock
  // std::cout << "\n==========getStock==============" << std::endl;
  // Model::Stock stock7 = getStock("ASMAB11");
  // std::cout << "Stock: " << stock7.stockId << " " << std::endl;
  // Model::Stock stock4 = getStock("AMMXM3");
  // std::cout << "Stock: " << stock4.stockId << " " << std::endl;
  // Model::Stock stock8 = getStock("APETR3");
  // std::cout << "Stock: " << stock4.stockId << " " << std::endl;

  // Test getStockList
  std::cout << "\n==============getStockList==============" << std::endl;
  std::vector<Model::Stock> stocks = getStockList("a", 2, 0);
  for (Model::Stock s : stocks)
  {
    std::cout << "Stock: " << s.stockId << " " << std::endl;
  }

  // // Test updateStock
  // std::cout << "\n=============updateStock================= " << std::endl;
  // // initial max_date
  // std::cout << "Stock: " << stock3.stockId << " " << stock3.max_date << std::endl;
  // std::cout << "update max_date to 2021-01-01 ..." << std::endl;
  // stock3.max_date = "2021-01-01";
  // updateStock(stock3);
  // std::cout << "stock updated." << std::endl;
  // // updated max_date
  // stock3 = getStock(stock3.stockId);
  // std::cout << "Stock: " << stock3.stockId << " " << stock3.max_date << std::endl;

  // // Test deleteStock
  // std::cout << "\n=================deleteStock================" << std::endl;
  // deleteStock("apple");
  // std::cout << "stock deleted." << std::endl;
  // std::cout << "getStockList " << std::endl;
  // stocks = getStockList("a", 10, 0);
  // for (Model::Stock s : stocks)
  // {
  //   std::cout << "Stock: " << s.stockId << " " << std::endl;
  // }

  std::cout << "\n=================add stock price================" << std::endl;
  // Test addStockPrice
   Model::StockPrice stockPrice;
  // stockPrice.stockPriceId = "ASMAB1120200101";
  // stockPrice.stockId = "ASMAB11";
  // stockPrice.date = "2020-01-01";
  // stockPrice.open = 100;
  // stockPrice.high = 110;
  // stockPrice.low = 90;
  // stockPrice.close = 105;
  // stockPrice.volume = 1000000;
  // addStockPrice(stockPrice);
  // std::cout << "stock price added." << std::endl;

  // Test addStockPrice
  stockPrice.stockPriceId = "AMMXM320100104";
  stockPrice.stockId = "AMMXM3";
  stockPrice.date = "2010-01-04";
  stockPrice.open = 100;
  stockPrice.high = 110;
  stockPrice.low = 90;
  stockPrice.close = 105;
  stockPrice.volume = 1000000;
  addStockPrice(stockPrice);
  std::cout << "stock price added." << std::endl;

  // Test addStockPrice
  stockPrice.stockPriceId = "AMMXM320100105";
  stockPrice.stockId = "AMMXM3";
  stockPrice.date = "2010-01-05";
  stockPrice.open = 100;
  stockPrice.high = 110;
  stockPrice.low = 90;
  stockPrice.close = 105;
  stockPrice.volume = 1000000;
  addStockPrice(stockPrice);
  std::cout << "stock price added." << std::endl;

  stockPrice.stockPriceId = "AMMXM320100106";
  stockPrice.stockId = "AMMXM3";
  stockPrice.date = "2010-01-06";
  stockPrice.open = 100;
  stockPrice.high = 110;
  stockPrice.low = 90;
  stockPrice.close = 105;
  stockPrice.volume = 1000000;
  addStockPrice(stockPrice);
  std::cout << "stock price added." << std::endl;
  
  
  // // Test addStockPrice
  // stockPrice.stockPriceId = "APETR320171227";
  // stockPrice.stockId = "APETR3";
  // stockPrice.date = "2010-01-05";
  // stockPrice.open = 100;
  // stockPrice.high = 110;
  // stockPrice.low = 90;
  // stockPrice.close = 105;
  // stockPrice.volume = 1000000;
  // addStockPrice(stockPrice);
  // std::cout << "stock price added." << std::endl;

  std::cout << "\n=============get stock prices ========== " << std::endl;
  std::vector<Model::StockPrice> stockPrices = getStockPriceList("AMMXM3", 2, 1);
  for (Model::StockPrice sp : stockPrices)
  {
    std::cout << "StockPrice: " << sp.stockPriceId << " " << sp.date << " " << sp.open << " " << sp.high << " " << sp.low << " " << sp.close << " " << sp.volume << std::endl;
  }

  // std::cout << "\n=============get stock price========== " << std::endl;
  // Model::StockPrice stockPrice4 = getStockPrice("ASMAB1120200101");
  // std::cout << "get stock price: " << stockPrice4.stockPriceId << " " << stockPrice4.date << " " << stockPrice4.open << " " << stockPrice4.high << " " << stockPrice4.low << " " << stockPrice4.close << " " << stockPrice4.volume << std::endl;
  // // Test deleteStockPrice
  // // std::cout
  // //     << "\n==========deleteStockPrice==============" << std::endl;
  // // deleteStockPrice("qwerty", "aapl");
  // // std::cout << "stockPrice deleted." << std::endl;
  // // std::cout << "getStockPriceList " << std::endl;
  // // stockPrices = getStockPriceList("aapl", 0, 10);
  // // for (Model::StockPrice s : stockPrices)
  // // {
  // //   std::cout << "StockPrice: " << s.stockPriceId << " " << s.date << " " << s.open << " " << s.high << " " << s.low << " " << s.close << " " << s.volume << std::endl;
  // // }

  // Test resetDb

  std::clog.rdbuf(oldClog);
  // return 0;
}
