//
// Created by Davis Polito on 1/28/25.
//

#ifndef BITKLAVIER2_FACTORY_H
#define BITKLAVIER2_FACTORY_H

#include <functional>
#include <map>
#include <string>
#include <iostream>
#include <any>
#include <utility>
#include "Type.h"
// implemented from https://www.nirfriedman.com/2018/04/29/unforgettable-factory/
template<class Base, class... Args>
class Factory {
public:
    template<class ... T>
    static std::shared_ptr<Base> create(const std::string &typeName, T &&... args) {
        return data().at(typeName)(std::forward<T>(args)...);
    }

    template<class T>
    struct Registrar : Base {
        friend T;


        static bool registerT() {
            //demangle assumes we have acces to run time type info (see -fno-rtti or -frtti) but we dont want to use that since it
            const auto name =demangle(typeid(T).name()); ; //T::name; //demangle(typeid(T).name());
            Factory::data()[name] =
                    [](Args... args) -> std::shared_ptr<Base> {
                        return std::make_unique<T>(std::forward<Args>(args)...);
                    };
            return true;
        }

        static bool registered;

    private:
        Registrar() :Base(Key{}) {(void)registered;}
    };



    friend Base;
private:

    using CreateFunction = std::shared_ptr<Base> (*)(Args...);

    static auto &data() {
        static std::unordered_map<std::string, CreateFunction> s;
        return s;
    }

    class Key {
    public:
        Key(){};
        template <class T> friend struct Registrar;
    };
};

template <class Base, class... Args>
template <class T>
bool Factory<Base, Args...>::Registrar<T>::registered =
        Factory<Base, Args...>::Registrar<T>::registerT();

#endif //BITKLAVIER2_FACTORY_H
