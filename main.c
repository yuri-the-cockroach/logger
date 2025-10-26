#include "logger.h"

int main() {
    CURRENT_LOG_LEVEL_CONSOLE = (enum loglevel)8;

    for (int i = 0; i < (int)CURRENT_LOG_LEVEL_CONSOLE; i++) {
        enum loglevel cur = (enum loglevel)i;
        LOG(cur, "Test");
    }
    CloseLogger();
    return 0;
}
