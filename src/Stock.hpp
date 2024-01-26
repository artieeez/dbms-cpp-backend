#include <string>

class Stock {
public:

    Stock()
        : name ("\0"), min_date {}, max_date{} {
    }

    Stock(const Stock& stck) {
        std::strcpy(name, stck.name);
        min_date = stck.min_date;
        max_date = stck.max_date;
    }

    char name [20];
    std::string min_date;
    std::string max_date;
};
