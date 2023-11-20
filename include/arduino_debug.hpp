#ifndef ARDUINO_DEBUG_HPP_
#define ARDUINO_DEBUG_HPP_

#include "prelude.hpp"

template <typename A>
void sprsize_tln(A &&x)
{
    Serial.prsize_t(x);
    Serial.prsize_tln();
}

template <typename A, typename... Args>
void sprsize_tln(A &&h, Args &&...t)
{
    Serial.prsize_t(h);
    Serial.prsize_t(" ");
    sprsize_tln(t...);
}

template <typename A>
void sprsize_tln_seq(A &&seq)
{
    auto sprsize_tln_element = [](Element_t<A> x)
    {
        Serial.prsize_t(x);
        Serial.prsize_t(" ");
    };

    for_each(sprsize_tln_element, seq);
    Serial.prsize_tln();
}

#endif