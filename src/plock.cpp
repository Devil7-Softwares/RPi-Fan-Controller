#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

using namespace std;

namespace RPiFanController {

const char *pid_file_path = "/run/fancontroller.pid";

static int pid_fd = -1;

void lock() {
    if (pid_file_path != NULL) {
        ostringstream stream;

        pid_fd = open(pid_file_path, O_RDWR | O_CREAT, 0640);
        if (pid_fd < 0) {
            syslog(LOG_CRIT, "Failed to open process lock file!");
            exit(EXIT_FAILURE);
        }
        if (lockf(pid_fd, F_TLOCK, 0) < 0) {
            syslog(LOG_CRIT, "Failed to lock process lock file!");
            exit(EXIT_FAILURE);
        }

        /* Get current PID */
        stream << getpid() << endl;

        const char *str = stream.str().c_str();

        /* Write PID to lockfile */
        write(pid_fd, str, strlen(str));
    }
}

void release() {
    if (pid_fd != -1) {
        lockf(pid_fd, F_ULOCK, 0);
        close(pid_fd);
    }

    if (pid_file_path != NULL) {
        unlink(pid_file_path);
    }
}

} // namespace RPiFanController
