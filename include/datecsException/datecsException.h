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
    std::string type_error;
public:
    datecsException(const char* error);
    datecsException(std::string error, string type_error="none");
    const char* what();
    const char* whatType();
};


#endif //DCONSOLE_DATECSEXCEPTION_H
