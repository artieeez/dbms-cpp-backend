#include <string>
#include <queue>
#include <vector>
#include "stock.hpp"
#include "stockPrice.hpp"

namespace Controller {
namespace LinearSearch {

void addStock(Model::Stock payload);
void deleteStock(char stockId[MAX_SIZE_STOCK]);
void updateStock(Model::Stock payload);
Model::Stock getStock(char stockId[MAX_SIZE_STOCK]);
std::vector<Model::Stock> getStockList(char prefix[MAX_SIZE_STOCK], int page, int pageSize, std::string orderBy = "name_asc");

void addStockPrice(Model::StockPrice payload);
void deleteStockPrice(char stockPriceId[MAX_SIZE_SP]);
void updateStockPrice(Model::StockPrice payload);
std::vector<Model::StockPrice> getStockPriceList(char stockId[MAX_SIZE_STOCK], char minDate[MAX_SIZE_DATE], char maxDate[MAX_SIZE_DATE], std::string orderBy);

// sort a stockPrice list by date
void sortStockPriceList(std::vector<Model::StockPrice>& stockPriceList);

} // namespace LinearSearch

} // namespace Controller