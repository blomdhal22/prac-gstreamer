#include "aiddca/aiddca.manager.h"

#include "aiddca/aiddca.logger.h"

#include <thread>


int aiddca_test_main()
{
    AIDDCA_LOG_PRINT_NOTICE(
        "Begin.");

    aiddca_manager_init(1000, 10000);

    aiddca_manager_start();

    std::this_thread::sleep_for(std::chrono::milliseconds(25000));

    aiddca_manager_stop();

    aiddca_manager_deinit();

    AIDDCA_LOG_PRINT_NOTICE(
        "End.");

    return 0;
}

int main(void)
{
    return aiddca_test_main();
}