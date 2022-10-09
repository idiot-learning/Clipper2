#include <gtest/gtest.h>
#include "clipper2/clipper.h"
#include <fstream>
#include <random>
#include "../Utils/clipper.svg.utils.h"
void SystemTest(const std::string &filename) {
#ifdef _WIN32
    system(filename.c_str());
#else
    system(("firefox " + filename).c_str());
#endif
}
//TEST(Clipper2Tests, TestLearningPaper) {
//    Clipper2Lib::Paths64 subject;
//    std::vector<std::pair<int, int>> path_data{{3, 9}, {0, 8}, {1, 4}, {2, 0}, {3, 2}, {6, 5}, {7, 4}, {8, 1}, {9, 6}};
//
//    Clipper2Lib::Path64 path;
//    for (auto each_pair : path_data) {
//        path.emplace_back(Clipper2Lib::Point64(each_pair.first, each_pair.second));
//    }
//    subject.push_back(path);
//    path.clear();
//
//    Clipper2Lib::Clipper64 cp;
//    cp.AddSubject(subject);
//
//    Clipper2Lib::Paths64 clip_solution;
//    const bool ret = cp.Execute(Clipper2Lib::ClipType::Union, Clipper2Lib::FillRule::Positive, clip_solution);
//
//    Clipper2Lib::SvgWriter svg;
//    SvgAddSolution(svg, clip_solution, Clipper2Lib::FillRule::Positive, true);
//    //  bool is_positive = Clipper2Lib::IsPositive(clip_solution[0]);
//    SvgAddSolution(svg, subject, Clipper2Lib::FillRule::Positive, true);
//
//    SvgSaveToFile(svg, "solution.svg", 450, 450, 10);
//    SystemTest("solution.svg");
//}

//
//TEST(Clipper2Tests, TestLearningSquare) {
//    Clipper2Lib::Paths64 subject;
//    std::vector<std::pair<int, int>> path_data{{0, 1}, {2, 0}, {3, 2}, {1, 3}};
//
//    Clipper2Lib::Path64 path;
//    for (auto each_pair : path_data) {
//        path.emplace_back(Clipper2Lib::Point64(each_pair.first, each_pair.second));
//    }
//    subject.push_back(path);
//    path.clear();
//
//    Clipper2Lib::Clipper64 cp;
//    cp.AddSubject(subject);
//
//    Clipper2Lib::Paths64 clip_solution;
//    const bool ret = cp.Execute(Clipper2Lib::ClipType::Union, Clipper2Lib::FillRule::Positive, clip_solution);
//
//    Clipper2Lib::SvgWriter svg;
//    SvgAddSolution(svg, clip_solution, Clipper2Lib::FillRule::Positive, true);
//    //  bool is_positive = Clipper2Lib::IsPositive(clip_solution[0]);
//    SvgAddSolution(svg, subject, Clipper2Lib::FillRule::Positive, true);
//
//    SvgSaveToFile(svg, "solution.svg", 450, 450, 10);
//    SystemTest("solution.svg");
//}

TEST(Clipper2Tests, TestLearningHorizontalTriangle) {
    Clipper2Lib::Paths64 subject;
    std::vector<std::pair<int, int>> path_data{{2, 1}, {5, 1}, {3, 4} };

    Clipper2Lib::Path64 path;
    for (auto each_pair : path_data) {
        path.emplace_back(Clipper2Lib::Point64(each_pair.first, each_pair.second));
    }
    subject.push_back(path);
    path.clear();

    Clipper2Lib::Clipper64 cp;
    cp.AddSubject(subject);

    Clipper2Lib::Paths64 clip_solution;
    const bool ret = cp.Execute(Clipper2Lib::ClipType::Union, Clipper2Lib::FillRule::Positive, clip_solution);

    Clipper2Lib::SvgWriter svg;
    SvgAddSolution(svg, clip_solution, Clipper2Lib::FillRule::Positive, true);
    //  bool is_positive = Clipper2Lib::IsPositive(clip_solution[0]);
    SvgAddSolution(svg, subject, Clipper2Lib::FillRule::Positive, true);

    SvgSaveToFile(svg, "solution.svg", 450, 450, 10);
    SystemTest("solution.svg");
}