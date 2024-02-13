#include "stateController.hpp"
#include "context.hpp"
#include "logger.hpp"
#include <string>

extern Logger mainLogger;

void test_loader() {

    mainLogger.pushScope("test_loader");

    Database::State dbState = Controller::State::getDatabaseState();

    // loadDb
    Controller::State::loadDb(100);

    dbState = Controller::State::getDatabaseState();

    mainLogger.popScope();
}