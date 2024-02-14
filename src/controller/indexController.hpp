#ifndef INDEX_SEARCH_CONTROLLER_HPP
#define INDEX_SEARCH_CONTROLLER_HPP

#include "stock.hpp"
#include "stockPrice.hpp"
#include <string>
#include <vector>

namespace Controller {
namespace IndexSearch {

void addStock(Model::Stock payload);
void deleteStock(char stockId[MAX_SIZE_STOCK]);
void updateStock(Model::Stock payload);
Model::Stock getStock(char stockId[MAX_SIZE_STOCK]);
std::vector<Model::Stock> getStockList(char prefix[MAX_SIZE_STOCK], int pageNumber, int pageSize);

void addStockPrice(Model::StockPrice payload);
void deleteStockPrice(char stockPriceId[MAX_SIZE_SP], char stockId[MAX_SIZE_STOCK]);
std::vector<Model::StockPrice> getStockPriceList(char stockId[MAX_SIZE_STOCK], int pageNumber, int pageSize);
} // namespace IndexSearch
} // namespace Controller

#endif // INDEX_SEARCH_CONTROLLER_HPP
