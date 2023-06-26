#include "q4.h"

namespace q4 {
#include <vector>

Vector2D kalman_filter(std::vector<Sensor> sensors) {
    Vector2D predicate;
    double total = 0.0;
    for (const auto &sensor : sensors) {
        total += sensor.accuracy;
    }
    for (const auto &sensor : sensors) {
        predicate.x += sensor.pos.x * sensor.accuracy / total;
        predicate.y += sensor.pos.y * sensor.accuracy / total;
    }
    return predicate;
}

}  // namespace q4