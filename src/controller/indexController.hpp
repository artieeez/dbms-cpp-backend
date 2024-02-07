#ifndef INDEX_SEARCH_CONTROLLER_HPP
#define INDEX_SEARCH_CONTROLLER_HPP

#include <string>
#include <vector>
#include "stock.hpp"
#include "stockPrice.hpp"

const std::string STOCK_TRIE_FILE = "stockId_trie.db";
const std::string STOCK_BLOCK_TRIE_FILE = "stockId_block_trie.db";
const std::string STOCK_PRICE_TRIE_FILE = "stock_price_trie.db";
const std::string STOCK_PRICE_BLOCK_FILE = "stock_price_block.db";

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

    void resetDb();
  }
}

#endif // INDEX_SEARCH_CONTROLLER_HPP
