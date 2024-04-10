//
// Created by Davis Polito on 12/13/23.
//

#ifndef BITKLAVIER2_CONNECTION_H
#define BITKLAVIER2_CONNECTION_H
#include <juce_data_structures/juce_data_structures.h>
#include <iostream>

#include <variant>
#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
struct string_compare
{
public:
    bool operator()(const std::string x, const std::string y)
    {
        return x.compare(y)<0;
    }
};

template <typename Key, typename Value>
std::map<Key,std::pair<Value,Value> >
    merge_maps( std::map<Key,Value> const & lhs, std::map<Key,Value> const & rhs )
{
    typedef typename std::map<Key,Value>::const_iterator input_iterator;
    std::map<Key, std::pair<Value,Value> > result;
    for ( input_iterator it1 = lhs.begin(), it2 = rhs.begin(),
                        end1 = lhs.end(), end2 = rhs.end();
         it1 != end1 && it2 != end2; )
    {
        if ( it1->first == it2->first )
        {
            result[it1->first] = std::make_pair( it1->second, it2->second );
            ++it1; ++it2;
        }
        else
        {
            if ( it1->first < it2->first )
                ++it1;
            else
                ++it2;
        }
    }
    return result;
}
class Connection
{
public:
    Connection(std::map<std::string, std::variant<std::shared_ptr<bool>,std::shared_ptr<int>,std::shared_ptr<float>>> &from,
        std::map<std::string, std::variant<std::shared_ptr<bool>,std::shared_ptr<int>,std::shared_ptr<float>>> &to ) :
                                                                                                                        _from(from), _to(to)

    {
        auto intersection = merge_maps (_from, _to);
        std::cout << intersection.size();
        std::for_each(intersection.begin(), intersection.end(), [this](auto values )
            {

                this->_to.at(values.first) = this->_from.at(values.first);



            });

    }

    std::map<std::string, std::variant<std::shared_ptr<bool>,std::shared_ptr<int>,std::shared_ptr<float>>> &_to;
    std::map<std::string, std::variant<std::shared_ptr<bool>,std::shared_ptr<int>,std::shared_ptr<float>>> &_from;


};

#endif //BITKLAVIER2_CONNECTION_H
