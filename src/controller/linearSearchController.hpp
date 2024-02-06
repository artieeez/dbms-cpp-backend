#include <string>
#include <queue>
#include <vector>
#include "stock.hpp"
#include "stockPrice.hpp"

namespace Controller {
namespace LinearSearch {

const std::string STOCK_DB_FILE = "stockDb.db";
const std::string STOCK_PRICE_DB_FILE = "stockPriceDb.db";

void addStock(Model::Stock payload);
void deleteStock(std::string stockId);
void updateStock(Model::Stock payload);
Model::Stock getStock(std::string stockId);
std::vector<Model::Stock> getStockList(std::string prefix, int page, int pageSize, std::string orderBy);

void addStockPrice(Model::StockPrice payload);
void deleteStockPrice(std::string stockPriceId);
void updateStockPrice(Model::StockPrice payload);
std::vector<Model::StockPrice> getStockPriceList(std::string stockId, std::string minDate, std::string maxDate, std::string orderBy);

void resetDb();
// TODO: add sorting options to this function (ex: increasing, decreasing, etc)
void sortStockPriceList(std::vector<Model::StockPrice> stockPriceList);
} // namespace LinearSearch

} // namespace Controller