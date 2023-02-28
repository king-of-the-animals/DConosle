//
// Created by danil on 25.02.23.
//
#include <iostream>
#include <limits>
#include "../../include/userFunc/userFunc.h"
#include "../../include/datecslib/datecslib.h"
#include "../../include/datecsException/datecsException.h"

bool connectWhenPortBusy(datecslib &d){
    cout << "Connect when the port is busy. Wait..." << endl;
    for (int i = 0; i < 50; ++i) {
        try {
            d.sendRead(90, "");
        }
        catch (datecsException &e) {
            continue;
        }
        cout << "The printer is now ours!Passed attempts " << i << endl;
        return true;
    }
    return false;
}

void workNullParam(string port, int cmd, string data, bool endless){

    datecslib datecs;

    if(!datecs.openPort(port)) {
        cerr  << "Error in open port " << port  << endl;
        return;
    }

    do{
        try{
            datecs.sendRead(cmd,data);
            cout << "===============================" << endl;
            cout << "Answer from Printer:" << endl;
            cout << datecs.getanswerFromPrinter() << endl;
            cout << "===============================" << endl;
            for(auto var : datecs.getStatusByte())
                cout << var << endl;
        }
        catch(datecsException &e) {
            cout << "Exception in send or read!!" << endl;
            cout << e.what() << endl;
        }

        if(endless) {
            cout << "Enter cmd" << endl;
            cin >> cmd;
            cout << "Enter data" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, data);
        }
    }while(endless);
}
void workWithFile(string port, string file) {
    datecslib datecs;
    int ask;
    bool isSkip = false;
    if (!datecs.openPort(port)) {
        cout  << "Error in open port" << endl;
        return;
    }
    cout << "Send command from file " << file << endl;
    if (!datecs.readFile(file)) {
        cout << "Error in open file" << endl;
        return;
    }
    for (auto var: datecs.getCommandFromFile())
        cout << "Data:\"" << var.cmd << "\" Command:\"" << var.data << "\""<< endl;
    cout << "Excute " << datecs.getCommandFromFile().size() << " commands?" << endl;
    cout << "1-Continue\n0-break" << endl; cout << "Input -->";
    cin >> ask;
    if(ask!=1){
        cout << "break and exit" << endl;
        return;
    }
    cout << "Read file successfully!" << endl;
    for (auto var: datecs.getCommandFromFile()) {
        cout << "Command:\"" << var.cmd << "\"\tData:\"" << var.data << "\"";
        try {
            datecs.sendRead(var.cmd, var.data);
        }
        catch (datecsException &ex) {
            cout << "\tStatus: bad" << endl;
            cout << "\nError in send or read command!!!!" << endl;
            cout  << ex.what() << endl;
            cout << "Продолжить?" << endl << "1-Continue\n0-break" << endl;
            cout << "Input -->";
            cin >> ask;
            if (ask == 1)
                continue;
            else{
                cout << "break and exit" << endl;
                return;
            }
        }
        cout << "\tStatus: OK" << endl;
        for (auto i: datecs.getStatusByte()) {
            cout << i << endl;
            cout << "Error in status byte!!" << endl;
            if(!isSkip) {
                cout << "Продолжить?" << endl << "2-Continue for everyone\n1-Continue\n0-break" << endl;
                cout << "Input -->";
                cin >> ask;
                if (ask == 1)
                    break;
                else if (ask == 2) {
                    isSkip=true;
                    break;
                } else
                    return;
            }
        }
        cout << "Answer from Printer:" << endl;
        cout << "--------------------------------" << endl;
        cout << datecs.getanswerFromPrinter() << endl;
        cout << "--------------------------------" << endl;
    }
    datecs.closePort();
}
void workWithFileIgnore(string port, string file) {
    datecslib datecs;
    if (!datecs.openPort(port)) {
        cout << "Error in open port" << endl;
        return;
    }
    cout << "Send command from file " << file << endl;
    if (!datecs.readFile(file)) {
        cout << "Error in open file" << endl;
        return;
    }
    cout << "Read file successfully!" << endl;
    for (auto var: datecs.getCommandFromFile()) {
        cout << "Command:\"" << var.cmd << "\"\tData:\"" << var.data << "\"";
        try {
            datecs.sendRead(var.cmd, var.data);
        }
        catch (datecsException &ex) {
            cout << "\tStatus: bad" << endl;
            cout << "\nError in send or read command!!!!" << endl;
            cout << ex.what() << endl;
        }

        cout << "\tStatus: OK" << endl;
        for (auto i: datecs.getStatusByte()) {
            cout << i << endl;
        }
        cout << "Answer from Printer:" << endl;
        cout << "--------------------------------" << endl;
        cout << datecs.getanswerFromPrinter() << endl;
        cout << "--------------------------------" << endl;
    }
    datecs.closePort();
}

void WorkWithSocket(string ip, int command, string data, bool endles){
    datecslib datecs;
    int trying = 5;
    if(!datecs.connetSocket(ip)){
        cout << "Error in open socket" << endl;
        return;
    }
    if(!connectWhenPortBusy(datecs)){
        cerr << "Error port busy! Try again" << endl;
        return;
    }
    do {
        back:
        try {
            datecs.sendRead(command, data);
            cout << "Answer from Printer:" << endl;
            cout << "--------------------------------" << endl;
            cout << datecs.getanswerFromPrinter() << endl;
            cout << "--------------------------------" << endl;
        }
        catch (datecsException &ex) {
            cout << "Error in socket:" << endl;
            cerr << ex.what() << endl;
            if(string(ex.whatType()) == "timeout"){
                if(trying<0){
                    cout << "Timeout. The attempts are over.\n break and exit" << endl;
                    break;
                }
                else {
                    cout << "Attempting to retry a command due to a timeout\n" << trying-- << " attempts left" << endl;
                    goto back;
                }
            }

        }
        if(endles) {
            cout << "Enter cmd" << endl;
            cin >> command;
            cout << "Enter data" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, data);
        }
    }while(endles);
    datecs.closePort();
}
void workWithFileSock(string ip, string file) {
    int trying = 5;
    datecslib datecs;
    int ask;
    bool isSkip = false;
    if (!datecs.connetSocket(ip)) {
        cout  << "Error in open socket" << endl;
        return;
    }
    if(!connectWhenPortBusy(datecs)){
        cerr  << "Error port busy! Try again" << endl;
        return;
    }
    cout << "Send command from file " << file << endl;
    if (!datecs.readFile(file)) {
        cout  << "Error in open file" << endl;
        return;
    }
    for (auto var: datecs.getCommandFromFile())
        cout << "Data:\"" << var.cmd << "\" Command:\"" << var.data << "\""<< endl;
    cout << "Excute " << datecs.getCommandFromFile().size() << " commands?" << endl;
    cout << "1-Continue\n0-break" << endl; cout << "Input -->";
    cin >> ask;
    if(ask!=1){
        cout << "break and exit" << endl;
        return;
    }
    cout << "Read file successfully!" << endl;
    for (auto var: datecs.getCommandFromFile()) {
        cout << "Command:\"" << var.cmd << "\"\tData:\"" << var.data << "\"";
        back:
        try {
            datecs.sendRead(var.cmd, var.data);
        }
        catch (datecsException &ex) {
            cout << "\tStatus: bad" << endl;
            cout << "\nError in send or read command!!!!" << endl;
            if(string(ex.whatType()) == "timeout"){
                if(trying<0){
                    cout  << "Timeout. The attempts are over.\n break and exit" << endl;
                    break;
                }
                else {
                    cout << "Attempting to retry a command due to a timeout\n" << trying-- << " attempts left" << endl;
                    goto back;
                }
            }
            cout << "Продолжить?" << endl << "1-Continue\n0-break" << endl;
            cout << "Input -->";
            cin >> ask;
            if (ask == 1)
                continue;
            else{
                cout << "break and exit" << endl;
                return;
            }
        }
        cout << "\tStatus: OK" << endl;
        for (auto i: datecs.getStatusByte()) {
            cout  << i << endl;
            cout  << "Error in status byte!!" << endl;
            if(!isSkip) {
                cout << "Продолжить?" << endl << "2-Continue for everyone\n1-Continue\n0-break" << endl;
                cout << "Input -->";
                cin >> ask;
                if (ask == 1)
                    break;
                else if (ask == 2) {
                    isSkip=true;
                    break;
                } else
                    return;
            }
        }
        cout << "Answer from Printer:" << endl;
        cout << "--------------------------------" << endl;
        cout << datecs.getanswerFromPrinter() << endl;
        cout << "--------------------------------" << endl;
        trying=5;
    }
    datecs.closePort();
}
void workWithFileSockIgnore(string ip, string file) {
    datecslib datecs;
    int trying = 5;
    if (!datecs.connetSocket(ip)) {
        cout  << "Error in open socket" << endl;
        return;
    }
    if(!connectWhenPortBusy(datecs)){
        cerr << "Error port busy! Try again" << endl;
        return;
    }
    cout << "Send command from file " << file << endl;
    if (!datecs.readFile(file)) {
        cout  << "Error in open file" << endl;
        return;
    }
    cout << "Read file successfully!" << endl;
    for (auto var: datecs.getCommandFromFile()) {
        back:
        cout << "Command:\"" << var.cmd << "\"\tData:\"" << var.data << "\"";
        try {
            datecs.sendRead(var.cmd, var.data);
        }
        catch (datecsException &ex) {
            if(string(ex.whatType()) == "timeout"){
                if(trying<0){
                    cout << "Timeout. The attempts are over.\n break and exit" << endl;
                    break;
                }
                else {
                    cout << "Attempting to retry a command due to a timeout\n" << trying-- << " attempts left" << endl;
                    goto back;
                }
            }
            cout << "\tStatus: bad" << endl;
            cout << "\nError in send or read command!!!!" << endl;
            cout << ex.what() << endl;
        }

        cout  << "\tStatus: OK" << endl;
        for (auto i: datecs.getStatusByte()) {
            cout << i << endl;
        }
        cout  << "Answer from Printer:" << endl;
        cout  << "--------------------------------" << endl;
        cout  << datecs.getanswerFromPrinter() << endl;
        cout  << "--------------------------------" << endl;
    }
    datecs.closePort();
}