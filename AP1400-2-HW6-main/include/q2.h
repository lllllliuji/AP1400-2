#ifndef Q2_H
#define Q2_H
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
namespace q2 {
struct Patient {
   public:
    std::string name;
    size_t age;
    size_t smokes;
    size_t area_q;
    size_t alkhol;
    Patient() {}
    Patient(const Patient &p) {
        name = p.name;
        age = p.age;
        smokes = p.smokes;
        area_q = p.area_q;
        alkhol = p.alkhol;
    }
    int lung_cancer_weight() const { return 3 * age + 5 * smokes + 2 * area_q + 4 * alkhol; }
};

std::vector<Patient> read_file(std::string filename);

void sort(std::vector<Patient> &patients);
}  // namespace q2

#endif  // Q2_H