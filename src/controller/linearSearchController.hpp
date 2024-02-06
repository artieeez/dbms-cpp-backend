#include <string>
#include <queue>
#include <vector>
#include "stock.hpp"
#include "stockPrice.hpp"

namespace Controller {
namespace LinearSearch {

const std::string STOCK_DB_FILE = "stockDb.db";
const std::string STOCK_PRICE_DB_FILE = "stockPriceDb.db";
const std::string LOADER_DB_FILE_PATH = "loader.db";

void addStock(Model::Stock payload);
void deleteStock(std::string stockId);
void updateStock(Model::Stock payload);
Model::Stock getStock(std::string stockId);
std::vector<Model::Stock> getStockList(std::string prefix, int page, int pageSize, std::string orderBy = "name_asc");

void addStockPrice(Model::StockPrice payload);
void deleteStockPrice(std::string stockPriceId);
void updateStockPrice(Model::StockPrice payload);
std::vector<Model::StockPrice> getStockPriceList(std::string stockId, std::string minDate, std::string maxDate, std::string orderBy);

void resetDb();

// sort a stockPrice list by date
void sortStockPriceList(std::vector<Model::StockPrice> stockPriceList);

int loadDb(int pageSize);

inline std::string getSymbolFromLine(std::string line);
inline std::string getDateFromLine(std::string line);
Model::StockPrice getStockPriceFromLine(std::string line);

} // namespace LinearSearch

} // namespace Controller