#include <stdlib.h>
#include <syslog.h>

#include "config.hpp"
#include "gpio.hpp"
#include "main.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    // Setup logging
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON); // For more info: https://linux.die.net/man/3/syslog

    syslog(LOG_NOTICE, "RPi fan controller daemon started!");
    RPiFanController::Config *config = RPiFanController::read_config();
    RPiFanController::GPIO *gpio = new RPiFanController::GPIO(config->GPIO_PIN);

    if (!gpio->init()) {
        syslog(LOG_CRIT, "GPIO initialization failed!");
        return (EXIT_FAILURE);
    }

    RPiFanController::SignalHandler *signal_handler;
    if (!signal_handler->hook()) {
        return (EXIT_FAILURE);
    }

    while (!signal_handler->killed()) {
        sleep(config->INTERVAL);
    }

    gpio->close();

    syslog(LOG_NOTICE, "RPi fan controller daemon stopped!");

    closelog();
    return (EXIT_SUCCESS);
}
