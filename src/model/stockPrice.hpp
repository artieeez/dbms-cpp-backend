#ifndef STOCKPRICE_HPP
#define STOCKPRICE_HPP

#include <string>

const std::string STOCK_PRICE_DB_FILE = "stockPriceDb.db";
namespace Model {

struct StockPrice {
    std::string stockPriceId;
    std::string stockId;
    std::string date;
    float adj;
    float close;
    float high;
    float low;
    float open;
    float volume;
};

} // namespace Model

#endif // STOCKPRICE_HPP