
namespace Model {

struct StockPrice {
    char stockPriceId[20];
    char stockId[20];
    char date [9];
    float adj;
    float close;
    float high;
    float low;
    float open;
    float volume;
};

} // namespace Model