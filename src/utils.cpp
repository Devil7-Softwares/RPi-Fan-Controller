#include <errno.h>
#include <fstream>
#include <signal.h>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

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

void init_daemon() {
    pid_t pid = 0;
    int fd;

    // Fork off the parent process
    pid = fork();

    // Failed: Terminate the process
    if (pid < 0) {
        syslog(LOG_CRIT, "Failed to fork child process!");
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // On success: The child process becomes session leader
    if (setsid() < 0) {
        syslog(LOG_CRIT, "Failed to set session id for child process!");
        exit(EXIT_FAILURE);
    }

    // Ignore signal sent from child to parent process
    signal(SIGCHLD, SIG_IGN);

    // Fork off for the second time
    pid = fork();
    if (pid < 0) {
        syslog(LOG_CRIT, "Failed to fork for second time!");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Update file permissions for new child process
    umask(0);

    chdir("/");

    // Close any open file descriptors
    for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
        close(fd);
    }

    // Daemon process don't need standard I/O
    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");
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