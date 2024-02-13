#ifndef STOCKPRICE_HPP
#define STOCKPRICE_HPP
constexpr int MAX_SIZE_SP = 20;
constexpr int MAX_SIZE_STOCK = 10;
constexpr int MAX_SIZE_DATE = 15;

namespace Model {

struct StockPrice {
    char stockPriceId [MAX_SIZE_SP];
    char stockId [MAX_SIZE_STOCK];
    char date [MAX_SIZE_DATE];
    float adj;
    float close;
    float high;
    float low;
    float open;
    float volume;
};

} // namespace Model

#endif // STOCKPRICE_HPP