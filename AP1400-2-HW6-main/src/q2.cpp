#include "q2.h"
namespace q2 {

std::vector<Patient> read_file(std::string filename) {
    // std::cout << "read file"
    //           << " " << filename << std::endl;
    std::ifstream infile(filename, std::ios::in);
    std::vector<Patient> result;
    std::regex reg(",");  // 匹配split
    std::string line;
    while (infile.good()) {
        std::getline(infile, line);
        if (line == "lung_cancer" || line == "Name,Surname,Age,Smokes,AreaQ,Alkhol") {
            continue;
        }
        Patient patient;
        std::sregex_token_iterator pos(line.begin(), line.end(), reg, -1);
        decltype(pos) end;  // 自动推导类型
        for (int i = 0; pos != end; ++pos, ++i) {
            if (i == 0) {
                std::string t = pos->str();
                while (t.back() == ' ') t.pop_back();
                patient.name += t + " ";
            } else if (i == 1) {
                patient.name += pos->str();
            } else if (i == 2) {
                patient.age = stoi(pos->str());
            } else if (i == 3) {
                patient.smokes = stoi(pos->str());
            } else if (i == 4) {
                patient.area_q = stoi(pos->str());
            } else if (i == 5) {
                patient.alkhol = stoi(pos->str());
            }
        }
        // std::cout << patient.name << " " << patient.age << " " << patient.smokes << " " << patient.area_q << " "
        //           << patient.alkhol << std::endl;
        result.push_back(patient);
    }
    return result;
}

void sort(std::vector<Patient> &patients) {
    std::sort(patients.begin(), patients.end(),
              [](const Patient &pa, const Patient &pb) { return pa.lung_cancer_weight() > pb.lung_cancer_weight(); });
}
}  // namespace q2