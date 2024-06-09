#include <stdio.h>
#include <stdint.h>

#include <aiddca/foo.h>
#include <aiddca/aiddca.logger.h>

int main(void)
{
    uint64_t a = 0;
    int64_t b = 0;

    AIDDCA_LOG_PRINT_CRIT("hello world");
    AIDDCA_LOG_PRINT_CRIT("a=%ld b=%ld", a, b);

    foo();

    DI("hello!!!");

    return 0;
}