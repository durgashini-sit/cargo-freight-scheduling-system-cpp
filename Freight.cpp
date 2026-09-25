#include "Freight.h"
#include <sstream>

Freight::Freight(const std::string& id,
    const std::string& dest,
    const Time& t,
    const std::string& label,
    int capacity)
    : Record(id, dest, t), label_(label), capacity_(capacity) {
}

std::string Freight::toString() const {
    std::ostringstream oss;
    oss << getId() << ',' << getDestination() << ',' << getTime().toString()
        << ',' << label_ << ',' << capacity_;
    return oss.str();
}
