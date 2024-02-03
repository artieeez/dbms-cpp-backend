#include "context.hpp"

#include <filesystem>

namespace Database {

template <typename T>
Context<T>::Context(const std::string& filePath) {
    // check if file exists
    if (std::filesystem::exists(filePath)) {
        _file = std::fstream(filePath, std::ios::in | std::ios::out | std::ios::binary);
    } else {
        _file = std::fstream(filePath, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    }

    // Check if the file is open
    if (!_file.is_open()) {
        std::cerr << "Error opening or creating file: " << filePath << std::endl;
        // Handle the error as needed
    }
}

// template <typename T>
// Context<T>::~Context() {
//     _file.close();
// }

template <typename T>
Record<T> Context<T>::read(std::streampos position) {
    _file.seekg(position);
    Record<T> record;
    _file.read(reinterpret_cast<char*>(&record), sizeof(T));

    return record;
}

template <typename T>
std::streampos Context<T>::append(const T& value) {
    _file.seekp(0, std::ios::end);

    std::streampos position = _file.tellp();

    Record<T> record;
    record.value = value;
    record.position = position;
    record.deleted = false;

    _file.write(reinterpret_cast<const char*>(&record), sizeof(Record<T>));

    return position;
}

template <typename T>
void Context<T>::save(const Record<T>& record) {
    _file.seekp(record.position);

    _file.write(reinterpret_cast<const char*>(&record), sizeof(Record<T>));
}

template <typename T>
void Context<T>::remove(std::streampos position) {
    
    Record<T> record = read(position);
    record.deleted = true;
    save(record);
}

template <typename T>
std::streampos Context<T>::getLastPosition() {
    _file.seekg(0, std::ios::end);
    std::streampos endPosition = _file.tellg();

    if (endPosition == 0) {
        return 0;
    } else if (endPosition < sizeof(Record<T>)) {
        std::cerr << "File is empty or too small to contain a record." << std::endl;
        return 0;  // Return an invalid position to indicate an error
    }

    // Calculate the position of the last record
    return (int)endPosition - (int)sizeof(Record<T>);
}

template <typename T>
void Context<T>::move(std::streampos position, std::streampos newPosition) {
    // Check if the positions are valid
    if (position < 0 || newPosition < 0) {
        std::cerr << "Invalid positions for record relocation." << std::endl;
        return;
    }

    // Check if the file is large enough to contain records at the specified positions
    _file.seekg(0, std::ios::end);
    std::streampos endPosition = _file.tellg();

    if (endPosition < (int)std::max(position, newPosition) + sizeof(Record<T>)) {
        std::cerr << "File is too small to contain records at the specified positions." << std::endl;
        return;
    }

    // Move the get pointer to the position of the record to be relocated
    _file.seekg(position);

    // Read the record from the original position
    Record<T> record;
    _file.read(reinterpret_cast<char*>(&record), sizeof(Record<T>));

    // Move the put pointer to the new position
    _file.seekp(newPosition);

    // Write the record to the new position
    _file.write(reinterpret_cast<const char*>(&record), sizeof(Record<T>));
}

// Iterator
template <typename T>
bool Context<T>::next() {
    _currPos = (int)_currPos < 0
                   ? (int)static_cast<std::streampos>(0)
                   : (int)_currPos + (int)sizeof(Record<T>);

    bool positionIsValid = _currPos <= getLastPosition();

    curr = read(_currPos);

    return _currPos <= getLastPosition();
}

template <typename T>
void Context<T>::clearIterator() {
    _currPos = -1;
}

template <typename T>
std::streampos Context<T>::getCurrPosition() {
    return _currPos;
}

}  // namespace Database

template class Database::Context<int>;
