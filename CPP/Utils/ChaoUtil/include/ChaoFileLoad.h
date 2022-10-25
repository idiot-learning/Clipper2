#include "clipper2/clipper.h"
#include <fstream>
#include<boost/algorithm/string.hpp>
#include<boost/algorithm/string/split.hpp>

constexpr char GP_HEADER_VERSION[]="version 3";
constexpr double GPFILE_SCALE = 1000.0;
constexpr int FLAG_GLOBAL = 1 << 1;

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

bool loadGpFile(const std::string& file_path, Clipper2Lib::Paths64& subject)
{
    using namespace Clipper2Lib;
    std::ifstream input(file_path);
    std::string line;
    // version line
    std::getline(input, line);
    if (line.find(GP_HEADER_VERSION) == std::string::npos) {
        return false;
    }

    // gp header info line
    std::getline(input, line);
    std::vector<std::string> words;
    boost::algorithm::trim(line);
    boost::algorithm::split(words, line, boost::is_any_of(" \t"), boost::token_compress_on);
    if (words.size() < 4) {
        return false;
    }

    auto parse_point = [](const std::string& x, const std::string& y){
        return Point64(std::round(std::atof(x.c_str())), std::round(std::atof(y.c_str())));
    };
    // parse ring and hole part
    int ring_num = std::atoi(words[0].c_str());
    while (ring_num > 0) {
        --ring_num;
        words.clear();
        std::getline(input, line);
        boost::algorithm::trim(line);
        boost::algorithm::split(words, line, boost::is_any_of(" \t"), boost::token_compress_on);
        assert(words.size() == 2 && "words should have total point number and ring index");
        const int ring_pt_num = std::atoi(words[0].c_str());
        // get ring type
        std::getline(input, line);
        const bool is_outer_ring = std::atoi(line.c_str()) == 1 ? false : true;

        int cnt = 0;
        Path64 ring;
        while (cnt < ring_pt_num) {
            std::getline(input, line);

            words.clear();
            boost::algorithm::trim(line);
            boost::algorithm::split(words, line, boost::is_any_of(" \t"), boost::token_compress_on);
            assert(words.size() >= 2 && "points line should have at least two points.");
            ring.emplace_back(parse_point(words[0], words[1]));
            cnt++;
        }
        if(!ring.empty())        subject.push_back(ring);
    }
    return true;
}