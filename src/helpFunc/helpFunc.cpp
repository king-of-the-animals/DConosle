//
// Created by danil on 17.02.23.
//

#include <ctime>
#include <iostream>
#include <vector>
#include "../../include/helpFunc/helpFunc.h"


int GetRandomNumber(int min, int max) {
    // Установить генератор случайных чисел
    srand(static_cast<unsigned int>(time(0)));

    // Получить случайное число - формула
    int num = min + rand() % (max - min + 1);

    return num;
}
void Utf8to866(char *s)
{  int j=0, i; char cc, b;
    for(i=0; s[i]; i++) {
        cc = s[i];
        if (cc==208 || cc==209) {
            i++;
            if (s[i]==0) break;
            b = s[i];
            if (cc==208) {
                if (b>= 144 && b<192) b -= 16;
            }
            else {
                if (b>= 128 && b<144) b += 96;
            }
            s[j++] = b;
        }
        else s[j++] = cc;
    }
    s[j] = 0;
}
unsigned char str_to_hex(const char *szstr)
{
    return ((szstr[0] > '9' ? szstr[0]-'a' : szstr[0]-'0')*16 + (szstr[1] > '9' ? szstr[1]-'a' : szstr[1]-'0'));
}
ushort* string_to_ushort(std::string s, int *len) {
    std::vector<ushort> ushorts;
    for (int i = 0; i < s.length(); ++i) {
/*        if (int(s[i]) == -48) {
            ushorts.push_back(int(s[i + 1])+304);
            i += 1;
        } else*/
        if((unsigned char)s[i] == 208) {
            //std::cout << "i = " << i << " " <<  int((unsigned char)s[i+1])+48 << std::endl;
            if((unsigned char)s[i+1] == 132)
                ushorts.push_back((unsigned char) 170);
            else if((unsigned char)s[i+1] == 134)
                ushorts.push_back((unsigned char) 178);
            else if((unsigned char)s[i+1] == 135)
                ushorts.push_back((unsigned char) 175);
            else if((unsigned char)s[i+1] == 161)
                ushorts.push_back((unsigned char) 209);
            else if((unsigned char)s[i+1] == 172)
                ushorts.push_back((unsigned char) 220);
            else
                ushorts.push_back((unsigned char) s[i+1]+48);
            i++;
        }

        else if((unsigned char)s[i] == 209){

            switch ((unsigned char) s[i+1]) {
                case 151:
                    ushorts.push_back((unsigned char)191);
                    break;
                case 150:
                    ushorts.push_back((unsigned char)179);
                    break;
                case 131:
                    ushorts.push_back((unsigned char)243);
                    break;
                case 148:
                    ushorts.push_back((unsigned char)186);
                    break;
                case 129:
                    ushorts.push_back((unsigned char)241);
                    break;
                case 140:
                    ushorts.push_back((unsigned char)252);
                    break;
            }
            i++;
        }
        else if((unsigned char)s[i] == 210){
            switch ((unsigned char) s[i+1]+29) {
                case 145:
                    ushorts.push_back((unsigned char)180);
                    break;
            }
            i++;

        }

        else {
           // std::cout << "i = " << i << " " <<  int((unsigned char)s[i]) << std::endl;
            ushorts.push_back((unsigned char) s[i]);
        }
    }
    *len = ushorts.size();
    ushort *arr = new ushort[ushorts.size()];
    std::copy(ushorts.begin(), ushorts.end(), arr);
    return arr;
}