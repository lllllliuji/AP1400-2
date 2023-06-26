#include "q3.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

namespace q3 {
size_t parse_time(char duration[]) {
    // printf("%s\n", duration);
    size_t ans = 0;
    int part = 0;
    for (int i = 0; i < 255 && duration[i] != '\0'; i++) {
        if (duration[i] >= '0' && duration[i] <= '9') {
            part = 10 * part + (duration[i] - '0');
        } else if (duration[i] == 'h') {
            ans += 60 * part;
            part = 0;
        } else if (duration[i] == 'm') {
            ans += part;
            part = 0;
        } else {
            break;
        }
        // std::cout << ans << " " << part << std::endl;
    }
    // ans += part;
    return ans;
}
Flight construct(std::string &s) {
    int index, connection_count, price;
    char flight_number[255];
    char duration[255];
    char connection_times[255];
    sscanf(s.c_str(), "%d- flight_number:%s - duration:%s - connections:%d - connection_times:%s - price:%d", &index,
           flight_number, duration, &connection_count, connection_times, &price);
    // printf("index: %d, price: %d\n", index, price);
    Flight flight;
    for (int i = 0; flight_number[i] != '\0'; i++) {
        flight.flight_number += flight_number[i];
    }
    flight.duration = parse_time(duration);
    flight.connections = connection_count;
    size_t total_time = 0;
    for (int i = 0, j = 0; i < connection_count; i++) {
        char tmp[255];
        int k = 0;
        while (j < 255 && connection_times[j] != '\0') {
            if (connection_times[j] == ',') {
                tmp[k++] = '\0';
                total_time += parse_time(tmp);
                k = 0;
                j++;
                break;
            }
            tmp[k++] = connection_times[j++];
        }
        if (k != 0) {
            total_time += parse_time(tmp);
        }
    }
    flight.connection_times = total_time;
    flight.price = price;
    // std::cout << flight.flight_number << " " << flight.duration << " " << flight.connections << " "
    //           << flight.connection_times << " " << flight.price << " " << flight.get_weight() << std::endl;
    return flight;
}
std::priority_queue<Flight, std::vector<Flight>, cmp> gather_flights(std::string fileName) {
    std::ifstream infile(fileName);
    std::string line;
    std::priority_queue<Flight, std::vector<Flight>, cmp> q(cmp{});
    while (infile.good()) {
        std::getline(infile, line);
        // std::cout << line << std::endl;
        auto t = construct(line);
        q.push(t);
    }
    return q;
}

}  // namespace q3