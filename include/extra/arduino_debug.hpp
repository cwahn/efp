#ifndef ARDUINO_DEBUG_HPP_
#define ARDUINO_DEBUG_HPP_

#include "efp/prelude.hpp"

template <typename A> void sprintln(A&& x) {
    Serial.print(x);
    Serial.println();
}

template <typename A, typename... Args> void sprintln(A&& h, Args&&... t) {
    Serial.print(h);
    Serial.print(" ");
    sprintln(t...);
}

template <typename A> void sprintln_seq(A&& seq) {
    auto sprintln_element = [](Element_t<A> x) {
        Serial.print(x);
        Serial.print(" ");
    };

    for_each(sprintln_element, seq);
    Serial.println();
}

#endif