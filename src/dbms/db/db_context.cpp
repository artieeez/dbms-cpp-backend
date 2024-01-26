#include "db_context.hpp"

#include <filesystem>

namespace DatabaseManagementSystem {
namespace Database {

template <typename T>
DbContext<T>::DbContext(const std::string& filePath) {
    this->filePath = filePath;

    // Open the file in append mode, which also creates the file if it doesn't exist
    std::ofstream file(filePath, std::ios::out | std::ios::app);

    std::filesystem::path absolutePath = std::filesystem::absolute(filePath);

    std::cout << "Original path: " << filePath << std::endl;
    std::cout << "Absolute path: " << absolutePath << std::endl;

    // Check if the file is open
    if (!file) {
        std::cerr << "Error opening or creating file: " << filePath << std::endl;
        // Handle the error as needed
    }

    // Close the file
    file.close();
}

template <typename T>
T DbContext<T>::read(std::streampos position) {
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

template <typename T>
std::streampos DbContext<T>::write(const T& record) {
    std::ofstream file(filePath, std::ios::out | std::ios::binary | std::ios::app);

    if (!file) {
        std::cerr << "Error opening file for writing." << std::endl;
        return -1;
    }

    file.seekp(0, std::ios::end);
    std::streampos position = file.tellp();

    file.write(reinterpret_cast<const char*>(&record), sizeof(T));

    file.close();

    return position;
}

template <typename T>
std::streampos DbContext<T>::remove(std::streampos position) {
    // call get last pos and then move the last record to the position of the deleted record
    std::streampos lastRecordPosition = getLastPosition();

    if (lastRecordPosition < 0) {
        std::cerr << "Error getting last record position." << std::endl;
        return -1;
    }

    move(lastRecordPosition, position);

    // TODO: remove last record

    return lastRecordPosition;
}

template <typename T>
std::streampos DbContext<T>::getLastPosition() {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file for getting last record position." << std::endl;
        return -1;  // Return an invalid position to indicate an error
    }

    file.seekg(0, std::ios::end);
    std::streampos endPosition = file.tellg();

    if (endPosition < sizeof(T)) {
        std::cerr << "File is empty or too small to contain a record." << std::endl;
        return -1;  // Return an invalid position to indicate an error
    }

    // Calculate the position of the last record
    std::streampos lastRecordPosition = endPosition - sizeof(T);

    file.close();
    return lastRecordPosition;
}

template <typename T>
void DbContext<T>::move(std::streampos position, std::streampos newPosition) {
    std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::binary);

    if (!file) {
        std::cerr << "Error opening file for record relocation." << std::endl;
        return;
    }

    // Check if the positions are valid
    if (position < 0 || newPosition < 0) {
        std::cerr << "Invalid positions for record relocation." << std::endl;
        file.close();
        return;
    }

    // Check if the file is large enough to contain records at the specified positions
    file.seekg(0, std::ios::end);
    std::streampos endPosition = file.tellg();

    if (endPosition < std::max(position, newPosition) + sizeof(T)) {
        std::cerr << "File is too small to contain records at the specified positions." << std::endl;
        file.close();
        return;
    }

    // Move the get pointer to the position of the record to be relocated
    file.seekg(position);

    // Read the record from the original position
    T record;
    file.read(reinterpret_cast<char*>(&record), sizeof(T));

    // Move the put pointer to the new position
    file.seekp(newPosition);

    // Write the record to the new position
    file.write(reinterpret_cast<const char*>(&record), sizeof(T));

    file.close();
}

}  // namespace Database
}  // namespace DatabaseManagementSystem

template class DatabaseManagementSystem::Database::DbContext<int>;
