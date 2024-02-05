#include "stock.hpp"
#include "stockPrice.hpp"
#include <string>
#include <vector>

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
std::vector<Model::StockPrice> getStockPriceList(std::string stockId, std::string minDate, std::string maxDate);

void resetDb();

} // namespace LinearSearch

} // namespace Controller