#ifndef COMPANY_HPP
#define COMPANY_HPP

#include <string>
const std::string COMPANY_DB_FILE = "companyDb.db";
namespace Model
{
  struct Company
  {
    std::string companyId;
    std::string name;
  };
} // namespace Model

#endif // COMPANY_HPP