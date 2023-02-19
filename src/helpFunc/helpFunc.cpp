//
// Created by danil on 17.02.23.
//

#include <ctime>
#include "../../include/helpFunc/helpFunc.h"


int GetRandomNumber(int min, int max) {
    // Установить генератор случайных чисел
    srand(static_cast<unsigned int>(time(0)));

    // Получить случайное число - формула
    int num = min + rand() % (max - min + 1);

    return num;
}

ushort* string_to_ushort(std::string s) {
    ushort *ushorts = new ushort[s.size()];
    for (size_t i = 0; i < s.size(); i++) {
        ushorts[i] = ushort(s[i]);
    }
    return ushorts;
}