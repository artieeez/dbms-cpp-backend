#ifndef LINEAR_SEARCH_CONTROLLER_HPP
#define LINEAR_SEARCH_CONTROLLER_HPP

#include "stock.hpp"
#include "stockPrice.hpp"
#include <string>
#include <vector>

namespace Controller {
namespace LinearSearch {

Model::Stock getStock(char stockId[MAX_SIZE_STOCK]);
std::vector<Model::Stock> getStockList(char prefix[MAX_SIZE_STOCK], int pageNumber, int pageSize);
std::vector<Model::StockPrice> getStockPriceList(char stockId[MAX_SIZE_STOCK], int pageNumber, int pageSize);
} // namespace LinearSearch
} // namespace Controller

#endif // LINEAR_SEARCH_CONTROLLER_HPP
