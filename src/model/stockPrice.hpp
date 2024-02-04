
#include <string>

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