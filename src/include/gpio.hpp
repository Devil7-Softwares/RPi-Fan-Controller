#ifndef GPIO_H
#define GPIO_H

namespace RPiFanController {

class GPIO {
  public:
    GPIO(int);
    bool init();
    bool high();
    bool low();

  private:
    int pin;
};

} // namespace RPiFanController
#endif // GPIO_H
