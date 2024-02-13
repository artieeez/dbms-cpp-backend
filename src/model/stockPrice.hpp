#ifndef STOCKPRICE_HPP
#define STOCKPRICE_HPP
constexpr int MAX_SIZE_SP = 20;

namespace Model {

struct StockPrice {
    char stockPriceId [MAX_SIZE_SP];
    char stockId [10];
    char date [15];
    float adj;
    float close;
    float high;
    float low;
    float open;
    float volume;
};

} // namespace Model

#endif // STOCKPRICE_HPP