#ifndef UTILS_H
#define UTILS_H

namespace RPiFanController {

int get_cpu_temp();

void init_daemon();

class SignalHandler {
  public:
    static bool hook();
    static bool killed();
    static void signal_handler(int _ignored);

  private:
    static bool _killed;
};

} // namespace RPiFanController

#endif // UTILS_H