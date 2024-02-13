extern void test_loader();
// extern void test_controller_index();
#include "logger.hpp"

extern Logger mainLogger;

int main() {
    mainLogger.pushScope("main");

    mainLogger.log("calling test_loader()");
    test_loader();

    mainLogger.log("calling test_controller_index()");
    // test_controller_index();

    mainLogger.popScope();
    return 0;
}
