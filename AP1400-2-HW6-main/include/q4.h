#ifndef Q4_H
#define Q4_H
#include <vector>
namespace q4 {
struct Vector2D {
    double x{};
    double y{};
};

struct Sensor {
    Vector2D pos;
    double accuracy;
};
Vector2D kalman_filter(std::vector<Sensor> sensors);
}  // namespace q4
#endif  // Q4_H