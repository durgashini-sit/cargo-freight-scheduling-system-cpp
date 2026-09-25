#include "FreightFactory.h"
#include "Freight.h"
#include <algorithm>

// converts to lowercase
static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return char(std::tolower(c)); });
    return s;
}
// sets capacity according to type
Freight FreightFactory::createFreight(const std::string& id,
    const std::string& dest,
    const Time& t,
    const std::string& label) {
    std::string L = lower(label);
    int cap = 0;
    if (L == "mm" || L == "minimover")        cap = 2;
    else if (L == "cc" || L == "cargocruiser") cap = 6;
    else if (L == "mc" || L == "megacarrier")  cap = 12;



    return Freight(id, dest, t, label, cap);   
}
