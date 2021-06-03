#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <boost/json.hpp>
#include <boost/json/src.hpp>

using namespace boost::json;

TEST(json_play_ground, test_1)
{
    object obj;
    obj["pi"] = 3.141;
    obj["happy"] = true;
    obj["name"] = "Boost";
    obj["nothing"] = nullptr;
    obj["answer"].emplace_object()["everything"] = 42;
    obj["list"] = {1, 0, 2};
    obj["object"] = {{"currency", "USD"}, {"value", 42.99}};

    value js={
        {"pi", 3.141},
        {"happy", true},
        {"name", "Boost"},
        {"nothing", nullptr},
        {"answer", {
            {"everything", 42} } },
        {"list", {1, 0, 2}},
        {"object", {
            {"currency", "USD"},
            {"value", 42.99}
        }}
    };

    array arr;
    arr = {1,2,3};
    value jv1(arr);
    value jv2(std::move(arr));
    
}