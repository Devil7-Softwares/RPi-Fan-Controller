#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <syslog.h>

#include "config.hpp"

using namespace std;

namespace RPiFanController {

string config_path = "/etc/rpifc.cfg";
string delimiter = "=";

Config::Config() {
    this->ON_TEMP = 50;
    this->OFF_TEMP = 40;
    this->INTERVAL = 5;
    this->GPIO_PIN = 14;
}

Config *read_config() {
    Config *config = new Config();

    try {
        if (filesystem::exists(config_path)) {
            syslog(LOG_INFO, "Reading Configuration...");

            string line;
            ifstream config_file(config_path);
            if (config_file.is_open()) {
                while (getline(config_file, line)) {
                    if (line.rfind("#", 0) != 0 && line.find(delimiter) != string::npos) {
                        string key = line.substr(0, line.find(delimiter));
                        string value = line.substr(key.length() + 1);

                        try {
                            int parsed_value = stoi(value);
                            if (key == "ON_TEMP") {
                                config->ON_TEMP = parsed_value;
                            } else if (key == "OFF_TEMP") {
                                config->OFF_TEMP = parsed_value;
                            } else if (key == "INTERVAL") {
                                config->INTERVAL = parsed_value;
                            } else if (key == "GPIO_PIN") {
                                config->GPIO_PIN = parsed_value;
                            }
                            syslog(LOG_INFO, "Loaded value from config file: %s = %d", key.c_str(), parsed_value);
                        } catch (...) {
                            syslog(LOG_ERR, "Failed to parse config value for '%s'!", key.c_str());
                        }
                    }
                }
            } else {
                syslog(LOG_ERR, "Failed to open configuration file!");
            }

            syslog(LOG_INFO, "Configuration loaded!");
        } else {
            syslog(LOG_NOTICE, "Configuration file doesn't exist! Creating one with default values...");

            ofstream config_file(config_path);
            config_file << "ON_TEMP=" << config->ON_TEMP << endl;
            config_file << "OFF_TEMP=" << config->OFF_TEMP << endl;
            config_file << "INTERVAL=" << config->INTERVAL << endl;
            config_file << "GPIO_PIN=" << config->GPIO_PIN << endl;
            config_file.close();
        }
    } catch (...) {
        syslog(LOG_ERR, "Failed to read configuration file!");
    }

    return config;
}

} // namespace RPiFanController