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
    
    /**
     * @brief Constructs a "Coordinate" object with default values.
     */
    Coordinate() : x(0), y(0) {}

    /**
     * @brief Constructs a "Coordinate" object with specified values.
     * @param x The x value for the coordinate.
     * @param y The y value for the coordinate.
     */
    Coordinate(int x, int y) : x(x), y(y) {}


     /**
     * @brief Destroys a "Coordinate" object.
     */
    ~Coordinate() {}
};

/**
 * @brief Overrides "==" operator for "Coordinate" and specifies how to check for equality.
 * @param c1 Coordinate #1 to check.
 * @param c2 Coordinate #2 to check.
 */
inline bool operator==(const Coordinate& c1, const Coordinate& c2) {
    return (c1.x == c2.x) && (c1.y == c2.y);
}

/**
 * @brief Overrides "!=" operator for "Coordinate" and specifies how to check for inequality.
 * @param c1 Coordinate #1 to check.
 * @param c2 Coordinate #2 to check.
 */
inline bool operator!=(const Coordinate& c1, const Coordinate& c2) {
    return !(c1 == c2);
}

#endif