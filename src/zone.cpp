#include "zone.h"

std::istream& operator>>(std::istream &is, State &s)
{
    char c; is >> c;

    switch (c)
    {
        case static_cast<char>(State::Start):
            s = State::Start;
            break;
        case static_cast<char>(State::Free):
            s = State::Free;
            break;
        case static_cast<char>(State::Wall):
            s = State::Wall;
            break;
        case static_cast<char>(State::Stop):
            s = State::Stop;
            break;
        default:
            throw std::runtime_error(std::format("{} cannot be mapped to State!", c));
    }

    return is;
}

std::ostream& operator<<(std::ostream &os, const State &s)
{
    os << static_cast<char>(s);
    return os;
}
