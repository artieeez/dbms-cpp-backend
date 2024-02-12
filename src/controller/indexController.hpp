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
    void deleteStock(std::string stockId);
    void updateStock(Model::Stock payload);
    Model::Stock getStock(std::string stockId);
    std::vector<Model::Stock> getStockList(std::string prefix, int pageSize, int page);

    void addStockPrice(Model::StockPrice payload);
    void deleteStockPrice(std::string stockPriceId, std::string stockId);
    std::vector<Model::StockPrice> getStockPriceList(std::string stockId, int pageSize, int page);
    Model::StockPrice getStockPrice(std::string stockPriceId);

    // sort a stockPrice list by date
    void sortStockPriceList(std::vector<Model::StockPrice> &stockPriceList);
  }
}

#endif // INDEX_SEARCH_CONTROLLER_HPP
