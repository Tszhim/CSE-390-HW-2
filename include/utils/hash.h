#ifndef HASH_H
#define HASH_H

#include <cstddef>
#include "coordinate.h"

/**
 * @brief An struct declaration for hashing coordinates.
 * 
 * Use this struct to provide a hashing function for the custom coordinate object.
 */
struct cHash
{       
    /**
     * @brief Hashing function for coordinates.
     * https://stackoverflow.com/questions/682438/hash-function-providing-unique-uint-from-an-integer-coordinate-pair
     * 
     * @param c The coordinate object to hash.
     * @return The hash of the coordinate as a size_t.
     */
    std::size_t operator() (const Coordinate& c) const 
    {
        return (c.x * 0x1f1f1f1f) ^ c.y;
    }
};

#endif