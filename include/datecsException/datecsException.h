//
// Created by danil on 17.02.23.
//

#ifndef DCONSOLE_DATECSEXCEPTION_H
#define DCONSOLE_DATECSEXCEPTION_H

#include "../datecslib/datecslib.h"
#include <exception>
#include "string"
class datecsException : public std::exception{
private:
    std::string message_error;
public:
    datecsException(std::string error);
    const char* what();
};


#endif //DCONSOLE_DATECSEXCEPTION_H
