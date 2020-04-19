#pragma once

#include "nixexpr.hh"
#include "symbol-table.hh"

#include <algorithm>
#include <optional>

namespace nix {


class EvalState;
struct Value;

/* Map one attribute name to its value. */
struct Attr
{
    Symbol name;
    Value * value;
    Pos * pos;
    Attr(Symbol name, Value * value, Pos * pos = &noPos)
        : name(name), value(value), pos(pos) { };
    Attr() : pos(&noPos) { };
    bool operator < (const Attr & a) const
    {
        return name < a.name;
    }
};

/* Bindings contains all the attributes of an attribute set. It is defined
   by its size and its capacity, the capacity being the number of Attr
   elements allocated after this structure, while the size corresponds to
   the number of elements already inserted in this structure. */
#include "bindings.hh"
}
