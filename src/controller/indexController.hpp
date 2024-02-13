#ifndef INDEX_SEARCH_CONTROLLER_HPP
#define INDEX_SEARCH_CONTROLLER_HPP

#include <string>
#include <vector>
#include "stock.hpp"
#include "stockPrice.hpp"

namespace Controller
{
  namespace IndexSearch
  {
    void addStock(Model::Stock payload);
    void deleteStock(char stockId[MAX_SIZE_STOCK]);
    void updateStock(Model::Stock payload);
    Model::Stock getStock(char stockId[MAX_SIZE_STOCK]);
    std::vector<Model::Stock> getStockList(char prefix[MAX_SIZE_SP], int pageSize, int page);

    void addStockPrice(Model::StockPrice payload);
    void deleteStockPrice(char stockPriceId[MAX_SIZE_SP], char stockId[MAX_SIZE_STOCK]);
    std::vector<Model::StockPrice> getStockPriceList(char stockId[MAX_SIZE_STOCK], int pageSize, int page);
    Model::StockPrice getStockPrice(char stockPriceId[MAX_SIZE_SP]);

    // sort a stockPrice list by date
    void sortStockPriceList(std::vector<Model::StockPrice> &stockPriceList);
  }
}

#endif // INDEX_SEARCH_CONTROLLER_HPP
