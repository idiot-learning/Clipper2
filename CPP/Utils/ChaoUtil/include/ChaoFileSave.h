#include "clipper2/clipper.h"
#include <fstream>

bool OutPutSubject(std::string file_path, const Clipper2Lib::Paths64& subject) {
    std::ofstream outfile(file_path);
    auto loc = file_path.find_last_of(".");
    auto suffix = file_path.substr(loc+1);
    if (suffix == "gp") {
        outfile << subject.size() << "\n";
        int polygoncount = 0;
        for (const auto& path : subject) {
            outfile << path.size() << " " << polygoncount++ << "\n";
            for (const auto& pt : path) {
                outfile << pt.x << " " << pt.y << "\n";
            }
        }
    } else if (suffix == "csv") {
        int polygoncount = 0;
        for (const auto& path : subject) {
            outfile <<"Polygon: " << " " << polygoncount++ << "\n";
            for (const auto& pt : path) {
                outfile << pt.x << " " << pt.y << "\n";
            }
        }
    }else if(suffix == "txt"){ //subject.push_back(MakePath("635280, 814060, 253680, 814060, 253680, 797700, 635280, 797700"));
        int polygoncount =0; 
        for (const auto& path : subject) {
            outfile <<"subject.push_back(MakePath(\" " ;
            for (size_t i =0 ; i < path.size() ; ++i) {
                if(i == 0 ) {
                    outfile << path[i].x << ", " << path[i].y ;
                }
                else 
                {
                    outfile<< "," << path[i].x << ", " << path[i].y ;
                }
            }
            outfile << "\"));\n";     
        }
    }
    return true;
}