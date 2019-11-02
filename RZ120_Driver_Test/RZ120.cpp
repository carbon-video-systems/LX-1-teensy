
#include <Arduino.h>

#include "RZ120.h"

// Print with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

RZ120Class::RZ120Class(Stream& serial)
    : serial_(serial) {}

// RZ120 Commands
void RZ120Class::PowerOn(void)
{
    serial_ << ".ADZZ;PON.";
}

void RZ120Class::PowerOn1(void)
{
    serial_ << 0x02 << "ADZZ;PON" << 0x03;
}

void RZ120Class::PowerOn2(void)
{
    serial_ << 0x02 << 0x41 << 0x44 << 0x5a << 0x5a << 0x3b << 0x50 << 0x4f << 0x4e << 0x03;
}

void RZ120Class::PowerOff(void)
{
    serial_ << ".ADZZ;POF.";
}

void RZ120Class::PowerOff1(void)
{
    serial_ << 0x02 << "ADZZ;POF" << 0x03;
}

void RZ120Class::PowerOff2(void)
{
    serial_ << 0x02 << 0x41 << 0x44 << 0x5a << 0x5a << 0x3b << 0x50 << 0x4f << 0x46 << 0x03;
}

String RZ120Class::readString() {
    String str = "";
    static const unsigned long timeout = 1000;
    unsigned long timeout_start = millis();
    for (;;) {
        while (!serial_.available()) {
            if (millis() - timeout_start >= timeout) {
                return str;
            }
        }
        char c = serial_.read();
        if (c == '\n')
            break;
        str += c;
    }
    return str;
}
