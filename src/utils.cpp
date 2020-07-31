#include <fstream>
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

} // namespace RPiFanController