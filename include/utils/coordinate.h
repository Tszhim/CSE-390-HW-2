#ifndef COORDINATE_H
#define COORDINATE_H

/**
 * @brief An struct declaration for coordinates.
 * 
 * Use this struct when expressing location relative to the charging dock (origin).
 * *Preferred over Pair<T, T> due to more specificity in attributes (x and y). 
 */
struct Coordinate {
    int x;
    int y;
};

#endif