#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <cmath>

#include <boost/json.hpp>
#include <boost/json/src.hpp>

using namespace boost::json;

class VisDataProvider
{

public:

    std::string provide(std::string in)
    {
        auto size{ value_to<int>(parse( in ).at("size")) };
        std::vector<value> vals(size + 1);
        vals[0] = {"time","level1"};

        int idx{1};
        while(size --> 0)
        {
            vals[idx++] = {
                idx, (float)(std::sin(x) / x) 
            };
            x++;
        }

        return serialize(value{
            {"data", value_from(vals)}
        });
    }

private:

    float x{ 1.0f };


};