cmake_minimum_required(VERSION 3.0.0)
project (RPi-Fan-Controller VERSION 0.0.1)

link_libraries(stdc++fs)

file(GLOB_RECURSE FC_SOURCES "src/*.cpp")

include_directories("src/include")
add_executable(fancontrollerd ${FC_SOURCES})

target_compile_features(fancontrollerd PRIVATE cxx_std_17)

install (TARGETS fancontrollerd	RUNTIME	DESTINATION /usr/bin)
install (FILES fancontroller.service DESTINATION /lib/systemd/system)
install(CODE "execute_process(COMMAND systemctl enable fancontroller)")
