//
// Created by danil on 14.02.23.
//
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <bitset>
#include <list>

#ifndef DFISCAL_DATECSLIB_H
#define DFISCAL_DATECSLIB_H
using namespace std;

class cmdData{
public:
    cmdData(int c, string d) : cmd(c), data(d){

    };
    int cmd;
    string data;
};
class datecslib {

public:
    datecslib();
    datecslib(string port);
    ~datecslib();
    bool openPort(string port);
    bool isOpen();
    bool sendCommand(int command, string data);
    void closePort();
    bool readAnswer();
    bool sendRead(int command, string data);
    vector<string> getStatusByte();
    list<cmdData> getCommandFromFile();
    bool readFile(string path);
    string getanswerFromPrinter();
private:
    unsigned char seq;
    int fd;
    vector<string> status;
    ifstream fileRead;
    list<cmdData> commandFromFile;
    string answerFromPrinter;
    ushort* CsCount(const vector<ushort> &array);

    bool readFirstByte(u_char &c);

    u_char * CommandFormation(ushort command, string data, int *lenght);

    void init_tty(int DriverHandle);

    void transcriptStatus(string status, int num);

};


#endif //DFISCAL_DATECSLIB_H
