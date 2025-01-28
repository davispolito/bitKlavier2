//
// Created by Davis Polito on 1/28/25.
//

#ifndef BITKLAVIER2_TYPE_H
#define BITKLAVIER2_TYPE_H

#include <string>
#include <typeinfo>

std::string demangle(const char* name);

template <class T>
std::string type(const T& t) {

    return demangle(typeid(t).name());
}

#endif //BITKLAVIER2_TYPE_H
