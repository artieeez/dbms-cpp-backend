#include <string>

namespace Model {

struct Stock
{
    std::string stockId;
    std::string companyId;
    std::string min_date;
    std::string max_date;
};

}  // namespace Model
