#include "stateController.hpp"
#include "context.hpp"
#include "logger.hpp"
#include <string>

void test_loader() {

    Logger logger("test_loader");
    logger.pushScope("MAIN");

    logger.pushScope("getDatabaseState()");
    Database::State dbState = Controller::State::getDatabaseState();
    logger.log("stockCount: " + std::to_string(dbState.stockCount));
    logger.log("stockPriceCount: " + std::to_string(dbState.stockPriceCount));
    logger.log("isFinished: " + std::to_string(dbState.isFinished));
    logger.log("loaderPosition: " + std::to_string(dbState.loaderPosition));
    logger.popScope();

    // loadDb
    logger.pushScope("loadDb()");
    logger.log("loading 100 records");
    Controller::State::loadDb(100);
    logger.popScope();

    logger.pushScope("getDatabaseState()");
    dbState = Controller::State::getDatabaseState();
    logger.log("stockCount: " + std::to_string(dbState.stockCount));
    logger.log("stockPriceCount: " + std::to_string(dbState.stockPriceCount));
    logger.log("isFinished: " + std::to_string(dbState.isFinished));
    logger.log("loaderPosition: " + std::to_string(dbState.loaderPosition));
    logger.popScope();
}