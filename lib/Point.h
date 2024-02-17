#ifndef TERMINALFILEEXPLORER_LIB_POINT_H_
#define TERMINALFILEEXPLORER_LIB_POINT_H_

class Point {
private:
    int x_;
    int y_;

public:
    Point(int, int);
    [[nodiscard]] int x() const noexcept;
    [[nodiscard]] int y() const noexcept;
    void add_to_x(int v);
    void add_to_y(int v);
    [[nodiscard]] bool operator==(Point const& other) const;
};

#endif  // TERMINALFILEEXPLORER_LIB_POINT_H_
