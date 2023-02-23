#include <iostream>
#include <limits>
#include "../include/datecslib/datecslib.h"
#include "../include/datecsException/datecsException.h"
void workNullParam(string port, int cmd, string data, bool endless){

    datecslib datecs;

    if(!datecs.openPort(port)) {
        cout << "Error in open port " << port  << endl;
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
        cout << "Error in open port" << endl;
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
            cout << ex.what() << endl;
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

void WorkWithSocket(string ip, int command, string data){
    datecslib datecs;
    try {
        datecs.connetSocket(ip);
        datecs.sendCommandSock(command,data);
        datecs.readAnswer();
        cout << "Answer from Printer:" << endl;
        cout << "--------------------------------" << endl;
        cout << datecs.getanswerFromPrinter() << endl;
        cout << "--------------------------------" << endl;
    }
    catch(datecsException &ex){
        cout << "Error in socket:"<< endl;
        cout << ex.what() << endl;
    }
    datecs.closePort();
}
void workWithFileSock(string ip, string file) {
    int trying = 5;
    datecslib datecs;
    int ask;
    bool isSkip = false;
    if (!datecs.connetSocket(ip)) {
        cout << "Error in open socket" << endl;
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
        back:
        try {
                datecs.sendRead(var.cmd, var.data);
        }
        catch (datecsException &ex) {
            cout << "\tStatus: bad" << endl;
            cout << "\nError in send or read command!!!!" << endl;
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
        trying=5;
    }
    datecs.closePort();
}


int main(int argc, char *argv[]) {
    setlocale(LC_ALL,"ru");
    string port;
    int cmd;
    string data;
    if(argc==5 && string(argv[2])=="-f" && string(argv[3]) == "-i"){
        workWithFileIgnore(argv[1],argv[4]);
    }
    else if(argc==4 && string(argv[2])=="-f"){
        workWithFile(argv[1],argv[3]);
    }
    else if(argc==5 && string(argv[1]) == "-s" && string(argv[3]) == "-f"){
        workWithFileSock(argv[2],argv[4]);
    }
    else if(argc==5 && string(argv[1])=="-s"){
        WorkWithSocket(argv[2], atoi(argv[3]), argv[4]);
    }
    else if(argc==4 && string(argv[1])=="-s"){
        WorkWithSocket(argv[2], atoi(argv[3]), "");
    }
    else if(argc==4) {
        workNullParam(argv[1], atoi(argv[2]), argv[3], false);
    }
    else if(argc==3){
        workNullParam(argv[1], atoi(argv[2]), "" , false);
    }
    else{
        cout << "Enter port" << endl;
        cin >> port;
        cout << "Enter cmd" << endl;
        cin >> cmd;
        cout << "Enter data" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, data);
        workNullParam(port,cmd,data,true);
    }
}

