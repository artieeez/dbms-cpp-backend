extern void test_loader();
extern void test_controller_linear();
// extern void test_controller_index();
#include "logger.hpp"

extern Logger mainLogger;

int main() {
    mainLogger.pushScope("main");

    test_loader();

    // test_controller_linear();

    // test_controller_index();

    mainLogger.popScope();
    return 0;
}
