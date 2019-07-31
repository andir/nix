#pragma once

#include "binary-cache-store.hh"

#include <atomic>

namespace nix {

class GCSBinaryCacheStore : public BinaryCacheStore
{
protected:

    GCSBinaryCacheStore(const Params & params)
        : BinaryCacheStore(params)
    { }
};

}
