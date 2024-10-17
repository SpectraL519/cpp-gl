#pragma once

#include <iostream>
#include <iomanip>
#include <locale>

struct thousend_sepparator : std::numpunct<char> {
protected:
    virtual char do_thousands_sep() const override { return '_'; }
    virtual std::string do_grouping() const override { return "\3"; }
};
