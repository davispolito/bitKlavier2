//
// Created by Davis Polito on 11/19/24.
//

#ifndef BITKLAVIER_FACTORY_H
#define BITKLAVIER_FACTORY_H
#include <functional>
#include <map>
#include <string>
#include <iostream>
#include <any>
template <class Base>
class Factory {
public:
    using CreateFunction = std::function<std::shared_ptr<Base>(std::any)>;

    template <typename T, typename... Args>
    void registerType(const std::string& typeName) {
        creators[typeName] = [](std::any args) -> std::shared_ptr<Base> {
            try {
                auto tupleArgs = std::any_cast<std::tuple<Args...>>(args); // Unpack std::any into tuple
                return std::apply([](auto&&... unpackedArgs) {
                    return std::make_shared<T>(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);  // Create shared_ptr with forwarded arguments
                }, tupleArgs);  // Apply the arguments
            } catch (const std::bad_any_cast& e) {
                std::cerr << "std::bad_any_cast: " << e.what() << " (expected tuple)" << std::endl;
                return nullptr;
            }
        };
    }

    // Create object with arguments wrapped in std::any
    std::shared_ptr<Base> create(const std::string& typeName, std::any args) const {
        auto it = creators.find(typeName);
        if (it != creators.end()) {
            return it->second(args);  // Call the creation function with arguments
        }
        return nullptr;  // Type not found
    }

private:
    std::map<std::string, CreateFunction> creators;
};
#endif //BITKLAVIER_FACTORY_H
