#include <string>
#include <string.h>

namespace Model {

struct Stock
{
    char stockId[20];
    char companyId[20];
    std::string min_date;
    std::string max_date;
}

}  // namespace Model
