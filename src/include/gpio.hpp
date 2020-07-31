#ifndef GPIO_H
#define GPIO_H

namespace RPiFanController {

class GPIO {
  public:
    GPIO(int);
    bool init();
    bool is_high();
    bool high();
    bool low();
    bool close();

  private:
    int pin;
};

} // namespace RPiFanController
#endif // GPIO_H
