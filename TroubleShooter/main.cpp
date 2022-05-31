#include <iostream>
#include "MultiPing.h"

int main()
{
    int timeout = 1000;

    int num_adresses = 7;
    string* adresses = new string[]{ 
        "yandex.ru", 
        "google.com",
        "ru.wikipedia.com",
        "192.168.0.6",
        "rutracker.org",
        "hhhh",
        "iafisher.com"
    };
    PingRes* res_a = new PingRes[num_adresses];
    MultiPing(res_a, adresses, num_adresses, timeout);
    for (int i = 0; i < num_adresses; ++i) { // Вывод результатов
        std::cout << res_a[i].adress << " time = " << res_a[i].time << std::endl;
    }

    PingRes* res_b = new PingRes[256];
    MultiPing256(res_b, "192.168.0.", timeout);
    for (int i = 0; i < 256; ++i) { // Вывод результатов
        if (res_b[i].time >= 0)
            std::cout << res_b[i].adress << " is responding, time = " << res_b[i].time << std::endl;
    }
}