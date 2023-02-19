//
// Created by danil on 17.02.23.
//

#include "../../include/datecsException/datecsException.h"
datecsException::datecsException(std::string error) : message_error(error) {
}

const char* datecsException::what() {
    return message_error.c_str();
}