#include <string>
#include <queue>
#include <vector>
#include "stock.hpp"
#include "stockPrice.hpp"

namespace Controller {
namespace LinearSearch {

void addStock(Model::Stock payload);
void deleteStock(std::string stockId);
void updateStock(Model::Stock payload);
Model::Stock getStock(std::string stockId);
std::vector<Model::Stock> getStockList(std::string prefix, int page, int pageSize, std::string orderBy = "name_asc");

void addStockPrice(Model::StockPrice payload);
void deleteStockPrice(std::string stockPriceId);
void updateStockPrice(Model::StockPrice payload);
std::vector<Model::StockPrice> getStockPriceList(std::string stockId, std::string minDate, std::string maxDate, std::string orderBy);

// sort a stockPrice list by date
void sortStockPriceList(std::vector<Model::StockPrice>& stockPriceList);

} // namespace LinearSearch

} // namespace Controller