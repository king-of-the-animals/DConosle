#include <iostream>
#include <limits>
#include "../include/userFunc/userFunc.h"
#include <time.h>

int main(int argc, char *argv[]) {
    setlocale(LC_ALL,"ru");

    std::cout << "Питон для лохов с++ для мужиков!!!"<< endl;


    int type;
    string port;
    int cmd;
    string data;
    if(argc==6 && string(argv[1]) == "-s" && string(argv[3]) == "-f" && string(argv[4]) == "-i"){
        workWithFileSockIgnore(argv[2],argv[5]);
    }
    else if(argc==5 && string(argv[1]) == "-s" && string(argv[3]) == "-f"){
        workWithFileSock(argv[2],argv[4]);
    }
    else if(argc==5 && string(argv[1])=="-s"){
        WorkWithSocket(argv[2], atoi(argv[3]), argv[4],false);
    }
    else if(argc==4 && string(argv[1])=="-s"){
        WorkWithSocket(argv[2], atoi(argv[3]), "",false);
    }
    else if(argc==5 && string(argv[2])=="-f" && string(argv[3]) == "-i"){
        workWithFileIgnore(argv[1],argv[4]);
    }
    else if(argc==4 && string(argv[2])=="-f"){
        workWithFile(argv[1],argv[3]);
    }
    else if(argc==4) {
        workNullParam(argv[1], atoi(argv[2]), argv[3], false);
    }
    else if(argc==3){
        workNullParam(argv[1], atoi(argv[2]), "" , false);
    }
    else{
        cout << "Enter type connection(1 - serial; 2-socket)" << endl;
        cin >> type;
        if(type==1)
            cout << "Enter port" << endl;
        else if(type==2)
            cout << "Enter ip" << endl;
        else {
            cout << "Error type connection " << endl;
            return -1;
        }
        cin >> port;
        cout << "Enter cmd" << endl;
        cin >> cmd;
        cout << "Enter data" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, data);
        if(type==1)
            workNullParam(port,cmd,data,true);
        else if(type==2)
            WorkWithSocket(port,cmd,data,true);
    }
}

