#include <fstream>
#include <sstream>
#include <syslog.h>

#include "gpio.hpp"

using namespace std;

namespace RPiFanController {

const char *gpio_export_path = "/sys/class/gpio/export";
const char *gpio_unexport_path = "/sys/class/gpio/unexport";
const char *gpio_direction_path = "/sys/class/gpio/gpio%d/direction";
const char *gpio_value_path = "/sys/class/gpio/gpio%d/value";

GPIO::GPIO(int pin) {
    this->pin = pin;
}

bool enable(int pin) {
    try {
        ofstream node(gpio_export_path);

        if (node) {
            node << pin;
        } else {
            syslog(LOG_CRIT, "Error writing to %s", gpio_export_path);
            return false;
        }

        node.close();

        return true;
    } catch (...) {
        syslog(LOG_CRIT, "Unhandled error on enabling pin!");
        return false;
    }
}

bool disable(int pin) {
    try {
        ofstream node(gpio_unexport_path);

        if (node) {
            node << pin;
        } else {
            syslog(LOG_CRIT, "Error writing to %s", gpio_unexport_path);
            return false;
        }

        node.close();

        return true;
    } catch (...) {
        syslog(LOG_CRIT, "Unhandled error on disabling pin!");
        return false;
    }
}

bool set_output(int pin) {
    try {
        char *path = new char[35];

        sprintf(path, gpio_direction_path, pin);

        ofstream node(path);

        if (node) {
            node << "out";
        } else {
            syslog(LOG_CRIT, "Error writing to %s", gpio_export_path);
            return false;
        }

        node.close();

        return true;
    } catch (...) {
        syslog(LOG_CRIT, "Unhandled error on setting pin to output mode!");
        return false;
    }
}

bool read(int pin) {
    try {
        char *path = new char[35];

        sprintf(path, gpio_value_path, pin);

        const char *value_str = "0";

        ifstream node(path);
        if (node.is_open()) {
            ostringstream value_stream;
            value_stream << node.rdbuf();
            value_str = value_stream.str().c_str();
            node.close();
        } else {
            syslog(LOG_CRIT, "Failed to open %s", path);
        }

        return stoi(value_str);
    } catch (...) {
        syslog(LOG_CRIT, "Failed to value of gpio pin!");
        return 0;
    }
}

bool write(int pin, int value) {
    try {
        char *path = new char[35];

        sprintf(path, gpio_value_path, pin);

        ofstream node(path);

        if (node) {
            node << value;
        } else {
            syslog(LOG_CRIT, "Error writing to %s", gpio_export_path);
            return false;
        }

        node.close();

        return true;
    } catch (...) {
        syslog(LOG_CRIT, "Unhandled error on setting pin to output mode!");
        return false;
    }
}

bool GPIO::init() {
    return enable(pin) && set_output(pin);
}

bool GPIO::is_high() {
    return read(pin) == 1;
}

bool GPIO::high() {
    return write(pin, 1);
}

bool GPIO::low() {
    return write(pin, 0);
}

bool GPIO::close() {
    return disable(pin);
}

} // namespace RPiFanController