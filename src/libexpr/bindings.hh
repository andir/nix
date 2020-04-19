#pragma once
#include <map>
#include <gc/gc_allocator.h>

#include "symbol-table.hh"

typedef std::map<Symbol, Attr, std::less<Symbol>, gc_allocator<std::pair<const Symbol, Attr>>> Bindings;

