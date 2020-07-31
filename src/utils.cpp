#include <errno.h>
#include <fstream>
#include <signal.h>
#include <sstream>
#include <syslog.h>

#include "utils.hpp"

using namespace std;

namespace RPiFanController {

const char *cpu_temp_path = "/sys/class/thermal/thermal_zone0/temp";

int get_cpu_temp() {
    try {
        const char *value_str;

        ifstream node(cpu_temp_path);
        if (node.is_open()) {
            ostringstream value_stream;
            value_stream << node.rdbuf();
            value_str = value_stream.str().c_str();
            node.close();
        } else {
            syslog(LOG_CRIT, "Failed to open %s", cpu_temp_path);
        }

        return stoi(value_str) / 1000;
    } catch (...) {
        syslog(LOG_CRIT, "Failed to get CPU temperature!");
        return -1;
    }
}

bool SignalHandler::_killed = false;

bool SignalHandler::hook() {
    if (signal((int)SIGINT, SignalHandler::signal_handler) == SIG_ERR) {
        syslog(LOG_CRIT, "Failed to setup signal handler!");
        return false;
    } else {
        return true;
    }
}

bool SignalHandler::killed() {
    return _killed;
}

void SignalHandler::signal_handler(int _ignored) {
    _killed = true;
}

} // namespace RPiFanController