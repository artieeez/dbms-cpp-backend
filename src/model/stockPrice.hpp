#ifndef STOCKPRICE_HPP
#define STOCKPRICE_HPP

#include <string>

namespace Model {

struct StockPrice {
    char stockPriceId [30];
    char stockId [30];
    char date [30];
    float adj;
    float close;
    float high;
    float low;
    float open;
    float volume;
};

} // namespace Model

#endif // STOCKPRICE_HPP