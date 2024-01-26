#ifndef DB_CONTEXT_HPP
#define DB_CONTEXT_HPP

#include <iostream>
#include <fstream>
#include <ios>

namespace DatabaseManagementSystem {
namespace Database {

    template <typename T>
    class DbContext {
    private:
        std::string filePath;

    public:
        DbContext(const std::string& filePath);

        T read(std::streampos position);

        std::streampos write(const T& record);

        std::streampos remove(std::streampos position);

        std::streampos getLastPosition();

        void move(std::streampos position, std::streampos newPosition);
    };

} // namespace Database
} // namespace DatabaseManagementSystem

#endif // DB_CONTEXT_HPP
