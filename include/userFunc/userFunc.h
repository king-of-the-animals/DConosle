//
// Created by danil on 25.02.23.
//

#ifndef DCONOSLE_USERFUNC_H
#define DCONOSLE_USERFUNC_H
#include "../../include/datecslib/datecslib.h"
#include "../../include/datecsException/datecsException.h"
bool connectWhenPortBusy(datecslib &d);

void workNullParam(string port, int cmd, string data, bool endless);

void workWithFile(string port, string file);

void workWithFileIgnore(string port, string file);

void WorkWithSocket(string ip, int command, string data, bool endles);

void workWithFileSock(string ip, string file);

void workWithFileSockIgnore(string ip, string file);



#endif //DCONOSLE_USERFUNC_H
