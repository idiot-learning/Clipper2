#include "clipper2/clipper.h"
#include <fstream>
bool isInRoi(const Clipper2Lib::Path64& path) {
    for (const auto& pt : path) {
       // if (pt.x < 160000 || pt.x > 730000)
        if (pt.x < 247200 || pt.x > 641700)
             return false;
        if (pt.x > 264000 &&pt.x < 625000  &&pt.y< 253000  ) return false;
    }
    return true;
}
bool isInRoi(const Clipper2Lib::Path64& path, bool is_minimal_input) {
    for (const auto& pt : path) {
        // if (pt.x < 160000 || pt.x > 730000)
        if (pt.x < 247200 || pt.x > 641700)
            return false;
        if (pt.x > 264000 && pt.x < 625000 && pt.y < 253000) return false;
        if (pt.x > 275000 && pt.x < 615300 && pt.y < 320000 && pt.y > 253500) return false;
        if (pt.x > 258000 && pt.x < 631000 && pt.y < 797000 && pt.y > 253500) return false;
        if (pt.x > 253600 && pt.x < 636000 && pt.y < 797000 && pt.y > 253500) return false;
    }
    return true;
}

bool loadCsvFile(const std::string& file_path, Clipper2Lib::Paths64& subject)
{
    using namespace Clipper2Lib;
    subject.clear();
    std::ifstream infile(file_path);
    std::string line;
    Path64 path;
    while (std::getline(infile, line)) {
        std::string firstword = line.substr(0, line.find(" "));
        if (firstword.find("Polygon") != std::string::npos) {
            if (!path.empty() && isInRoi(path, true))
                subject.push_back(path);
            path.clear();
        } else {
            std::stringstream ss(line);
            double x, y;
            ss >> x >> y;
            path.push_back(Point64{x, y});
        }
    }
    if (!path.empty() && isInRoi(path, true))  subject.push_back(path);
    return true;
}