
#include <string>

namespace Model {

struct StockPrice {
    std::string stockPriceId[20];
    std::string stockId[20];
    std::string date [9];
    float adj;
    float close;
    float high;
    float low;
    float open;
    float volume;
};

} // namespace Model