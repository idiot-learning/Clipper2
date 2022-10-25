#include <gtest/gtest.h>
#include "clipper2/clipper.h"
#include <random>
#include "../Utils/clipper.svg.utils.h"
#include "../Utils/ChaoUtil/include/ChaoFileLoad.h"
#include "../Utils/ChaoUtil/include/ChaoFileSave.h"
// #include "ChaoFileLoad.h"
// #include "ChaoFileSave.h"

void static SystemTest(const std::string& filename) {
#ifdef _WIN32
    system(filename.c_str());
#else
    system(("firefox " + filename).c_str());
#endif
}

namespace {
using namespace Clipper2Lib;
std::string MakeSpaces(const size_t cnt) {
    std::string s;
    if (cnt) s.insert(0, cnt, ' ');
    return s;
}

void DisplayPolyPath(const PolyPath64& pt, unsigned level) {
    std::string level_spaces = MakeSpaces(level * 4);
    std::string caption = pt.IsHole() ? "Hole " : "Outer Polygon ";
    std::string childs = pt.Count() == 1 ? " child" : " children";
    std::cout << level_spaces.c_str() << caption << "with " << pt.Count() << childs << std::endl;
    std::cout << level_spaces.c_str() << pt.Polygon();
    for (auto child : pt)
        DisplayPolyPath(*child, level + 1);
}

void DisplayPolyTree(const PolyTree64& pt) {
    std::cout << std::endl;
    for (auto child : pt)
        DisplayPolyPath(*child, 0);
}
}  // namespace

TEST(Clipper2Tests, TestOverlap) {
    using namespace Clipper2Lib;
    Paths64 subject;

    subject.push_back(MakePath(" -20, 30,-40, 30,-40, -50,-20, -50"));
    subject.push_back(MakePath(" -20, 20,-40, 20,-40, -20,-20, -20"));
    subject.push_back(MakePath(" -20, 10,-40, 10,-40, -10,-20, -10"));
    subject.push_back(MakePath(" 40, 30,20, 30,20, -50,40, -50"));
    subject.push_back(MakePath(" 40, 10,20, 10,20, -10,40, -10"));
    subject.push_back(MakePath(" 40, 20,20, 20,20, -20,40, -20"));
    subject.push_back(MakePath(" 40, 40,-40, 40,-40, 30,40, 30"));
    subject.push_back(MakePath(" 30, -30,-30, -30,-30, -40,30, -40"));
    subject.push_back(MakePath(" 10, 10,-10, 10,-10, -20,10, -20"));
    subject.push_back(MakePath(" -20, 10,-40, 10,-40, 0,-20, 0"));

    Clipper64 cp;
    cp.PreserveCollinear = false;
    cp.AddSubject(subject);
    Paths64 clip_solution;

    const bool ret = cp.Execute(ClipType::Union, FillRule::Positive, clip_solution);
    

    SvgWriter svg, svg2;
    SvgAddSolution(svg, subject, FillRule::Positive, true);
    SvgAddSolution(svg2, clip_solution, FillRule::Positive, true);
    SvgSaveToFile(svg, "TestOverlap_in.svg", 1280, 1280, 5);
    SvgSaveToFile(svg2, "TestOverlap_out.svg", 1280, 1280, 5);
    SystemTest("TestOverlap_in.svg");
    SystemTest("TestOverlap_out.svg");
}



// TEST(Clipper2Tests, TestEliminateOverlap) {
//     using namespace Clipper2Lib;
//     Paths64 subject;

//     subject.push_back(MakePath(" 20,0,40,0,40,20,20,20"));
//     subject.push_back(MakePath(" 0,20,60,20,60,40,0,40"));
//     subject.push_back(MakePath(" 10,40,30,40,30,60,10,60"));

//     Clipper64 cp;
//     cp.PreserveCollinear = false;
//     cp.AddSubject(subject);
//     Paths64 clip_solution;

//     const bool ret = cp.Execute(ClipType::Union, FillRule::Positive, clip_solution);
    

//     SvgWriter svg, svg2;
//     SvgAddSolution(svg, subject, FillRule::Positive, true);
//     SvgAddSolution(svg2, clip_solution, FillRule::Positive, true);
//     SvgSaveToFile(svg, "EliminateOverlap_in.svg", 1280, 1280, 5);
//     SvgSaveToFile(svg2, "EliminateOverlap_out.svg", 1280, 1280, 5);
//     SystemTest("EliminateOverlap_in.svg");
//     SystemTest("EliminateOverlap_out.svg");
// }