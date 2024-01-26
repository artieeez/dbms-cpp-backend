#include "dbms/db/db_context.hpp"

namespace DatabaseManagementSystem {
namespace Database {

template <typename T>
class Db {
   private:
    std::string filePath;

   public:
    Db(const std::string& filePath) : filePath(filePath) {}

    T readByStreamPosition(std::streampos position) {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        if (!file) {
            std::cerr << "Error opening file for reading." << std::endl;
            return T();
        }

        file.seekg(position);
        T record;
        file.read(reinterpret_cast<char*>(&record), sizeof(T));

        file.close();
        return record;
    }

    std::streampos write(const T& record) {
        std::ofstream file(filePath, std::ios::out | std::ios::binary | std::ios::app);

        if (!file) {
            std::cerr << "Error opening file for writing." << std::endl;
            return -1;
        }

        std::streampos position = file.tellp();
        file.write(reinterpret_cast<const char*>(&record), sizeof(T));

        file.close();
        return position;
    }

    void deleteRecord(std::streampos position) {
        std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::binary);

        if (!file) {
            std::cerr << "Error opening file for deletion." << std::endl;
            return;
        }

        file.seekg(0, std::ios::end);
        std::streampos endPosition = file.tellg();

        // Move the last record to the deleted position
        T lastRecord;
        file.seekg(endPosition - sizeof(T));
        file.read(reinterpret_cast<char*>(&lastRecord), sizeof(T));

        file.seekp(position);
        file.write(reinterpret_cast<const char*>(&lastRecord), sizeof(T));

        // Truncate the file to remove the last record
        file.trunc(endPosition - sizeof(T));

        file.close();
    }
};

}  // namespace db
}  // namespace dbms

