#ifndef CONFIG_H
#define CONFIG_H

namespace RPiFanController {

class Config {
  public:
    int ON_TEMP;
    int OFF_TEMP;
    int INTERVAL;
    int GPIO_PIN;
    Config();
};

Config *read_config();

} // namespace RPiFanController

#endif /* CONFIG_H */