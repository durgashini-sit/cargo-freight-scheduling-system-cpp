#include "Cargo.h"
#include <sstream>

Cargo::Cargo(const std::string& id,
    const std::string& dest,
    const Time& t,
    int g)
    : Record(id, dest, t), groupSize(g) {
}

std::string Cargo::toString() const {
    std::ostringstream oss;
    oss << getId() << ',' << getDestination() << ',' << getTime().toString()
        << ',' << groupSize;
    return oss.str();
}
