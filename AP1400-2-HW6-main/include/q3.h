#ifndef Q3_H
#define Q3_H
#include <queue>
#include <string>
#include <vector>
namespace q3 {
struct Flight {
    std::string flight_number;
    size_t duration;
    size_t connections;
    size_t connection_times;
    size_t price;
    size_t get_weight() const { return duration + connection_times + 3 * price; }
};

struct cmp {
    bool operator()(const Flight &f1, const Flight &f2) { return f1.get_weight() > f2.get_weight(); }
};
std::priority_queue<Flight, std::vector<Flight>, cmp> gather_flights(std::string fileName);
}  // namespace q3

#endif  // Q3_H