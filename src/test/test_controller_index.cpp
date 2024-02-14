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

  Model::Stock stock;
  strcpy(stock.stockId, "ASMAB11");
  strcpy(stock.max_date, "2020-01-01");
  strcpy(stock.min_date, "2019-01-01");
  strcpy(stock.companyId, "asdf");
  addStock(stock);

  strcpy(stock.stockId, "AMMXM3");
  strcpy(stock.max_date, "2020-01-01");
  strcpy(stock.min_date, "2019-01-01");
  strcpy(stock.companyId, "bueno");
  addStock(stock);

  strcpy(stock.stockId, "APETR3");
  strcpy(stock.max_date, "2020-01-01");
  strcpy(stock.min_date, "2019-01-01");
  strcpy(stock.companyId, "bueno");
  addStock(stock);

  strcpy(stock.stockId, "AQWERT12");
  strcpy(stock.max_date, "2020-01-01");
  strcpy(stock.min_date, "2019-01-01");
  strcpy(stock.companyId, "bueno");
  addStock(stock);


  std::cout << "\n==============getStockList==============" << std::endl;
  char teste[MAX_SIZE_STOCK] = "a";
  std::vector<Model::Stock> stocks = getStockList(teste, 0, 3);
  for (Model::Stock s : stocks)
  {
    std::cout << "Stock: " << s.stockId << " " << std::endl;
  }

  std::cout << "\n=================add stock price================" << std::endl;

  Model::StockPrice stockPrice;

  strcpy(stockPrice.stockPriceId, "AMMXM320100104");
  strcpy(stockPrice.stockId, "AMMXM3");
  strcpy(stockPrice.date, "2010-01-04");
  stockPrice.open = 100;
  stockPrice.high = 110;
  stockPrice.low = 90;
  stockPrice.close = 105;
  stockPrice.volume = 1000000;
  addStockPrice(stockPrice);
  std::cout << "stock price added." << std::endl;


  strcpy(stockPrice.stockPriceId, "AMMXM320100105");
  strcpy(stockPrice.stockId, "AMMXM3");
  strcpy(stockPrice.date, "2010-01-05");
  stockPrice.open = 100;
  stockPrice.high = 110;
  stockPrice.low = 90;
  stockPrice.close = 105;
  stockPrice.volume = 1000000;
  addStockPrice(stockPrice);
  std::cout << "stock price added." << std::endl;

  strcpy(stockPrice.stockPriceId, "AMMXM320100106");
  strcpy(stockPrice.stockId, "AMMXM3");
  strcpy(stockPrice.date, "2010-01-06");
  stockPrice.open = 100;
  stockPrice.high = 110;
  stockPrice.low = 90;
  stockPrice.close = 105;
  stockPrice.volume = 1000000;
  addStockPrice(stockPrice);
  std::cout << "stock price added." << std::endl;


  std::cout << "\n=============get stock prices ========== " << std::endl;
  char stockId[MAX_SIZE_STOCK] = "AMMXM3";
  std::vector<Model::StockPrice> stockPrices = getStockPriceList(stockId, 3, 0);
  for (Model::StockPrice sp : stockPrices)
  {
    std::cout << "StockPrice: " << sp.stockPriceId << " " << sp.date << " " << sp.open << " " << sp.high << " " << sp.low << " " << sp.close << " " << sp.volume << std::endl;
  }

  // Test resetDb

  std::clog.rdbuf(oldClog);
  // return 0;
}
