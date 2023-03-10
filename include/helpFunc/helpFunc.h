//
// Created by danil on 17.02.23.
//

#ifndef DCONSOLE_HELPFUNC_H
#define DCONSOLE_HELPFUNC_H
#include <algorithm>
#include "string"
#include <time.h>

int GetRandomNumber(int min, int max);
void Utf8to866(char *s);
ushort* string_to_ushort(std::string s,int *len);
unsigned char str_to_hex(const char *szstr);

std::string date_to_printer_format(tm *time);

std::string to_lower(std::string s);

#endif //DCONSOLE_HELPFUNC_H
