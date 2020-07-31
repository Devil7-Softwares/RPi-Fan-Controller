#include <stdlib.h>
#include <syslog.h>

#include "config.hpp"

#define DAEMON_NAME "fcdaemon"

using namespace std;

int main(int argc, char *argv[]) {
    // Setup logging
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON); // For more info: https://linux.die.net/man/3/syslog

    syslog(LOG_NOTICE, "RPi fan controller daemon started!");
    RPiFanController::read_config();

    syslog(LOG_NOTICE, "RPi fan controller daemon stopped!");

    closelog();
    return (EXIT_SUCCESS);
}
