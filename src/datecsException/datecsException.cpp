//
// Created by danil on 17.02.23.
//

#include "../../include/datecsException/datecsException.h"
datecsException::datecsException(const char* error) : message_error(error) {
}

const char* datecsException::what() {
    return message_error.c_str();
}

datecsException::datecsException(std::string error, string type_error) : message_error(error), type_error(type_error){

}

const char *datecsException::whatType() {
    return type_error.c_str();
}
