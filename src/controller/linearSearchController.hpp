#include <string>
#include <vector>
#include "stock.hpp"


namespace Controller {

const std::string STOCK_DB_FILE = "stockDb.db";

void addStock(Model::Stock payload);
// void deleteStock(std::string stockId);
// void updateStock(Model::Stock payload);
Model::Stock getStock(std::string stockId);
// std::vector<Model::Stock> getStockList(std::string prefix, int page, int pageSize, std::string orderBy);

// void addStockPrice(Model::StockPrice payload);
// void deleteStockPrice(std::string stockPriceId);
// void updateStockPrice(Model::StockPrice payload);
// std::vector<Model::StockPrice> getStockPriceList(std::string stockId, std::string minDate, std::string maxDate);

} // namespace Controller