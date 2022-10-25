#include <gtest/gtest.h>
#include "clipper2/clipper.h"
#include <random>
#include "../Utils/clipper.svg.utils.h"
#include "../Utils/ChaoUtil/include/ChaoFileLoad.h"
#include "../Utils/ChaoUtil/include/ChaoFileSave.h"
// #include "ChaoFileLoad.h"
// #include "ChaoFileSave.h"

void static SystemTest(const std::string &filename) {
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
}

// TEST(Clipper2Tests, TestCoedge2107) {
//     using namespace Clipper2Lib;
//     Paths64 subject;
//     loadCsvFile("coedge_2017.csv", subject);
//     //OutPutSubject("trimmed_coedge2107.gp", subject);
//     OutPutSubject("trimmed_coedge2107.csv", subject);
//     Clipper64 cp;
//     cp.PreserveCollinear = false;
//     cp.AddSubject(subject);
//     PolyTree64 clip_solution2;
//     Paths64 clip_solution;

//     const bool ret = cp.Execute(ClipType::Union, FillRule::Positive, clip_solution);
//     const bool ret2 = cp.Execute(ClipType::Union, FillRule::Positive, clip_solution2);
//    /* Clipper64 cp2;
//     cp2.PreserveCollinear = false;
//     cp2.AddSubject(clip_solution);
//     PolyTree64 clip_solution_double;
//     cp2.Execute(ClipType::Union, FillRule::Positive, clip_solution_double);*/

//     SvgWriter svg, svg2;

//     SvgAddSolution(svg, subject, FillRule::Positive, true);
//     SvgAddSolution(svg2, clip_solution, FillRule::Positive, true);
//     SvgSaveToFile(svg, "TrimCoedge2107_origin.svg", 5450, 5450, 10);
//     SvgSaveToFile(svg2, "TrimCoedge2107_clipped_result.svg", 5450, 5450, 10);
//     SystemTest("TrimCoedge2107_origin.svg");
//     SystemTest("TrimCoedge2107_clipped_result.svg");
// }

// TEST(Clipper2Tests, TestNested2Wrong) {
//    using namespace Clipper2Lib;
//     Paths64 subject{Path64{Point64{0, 0}, Point64{2, 0}, Point64{2, 9}, Point64{0, 9}},
//                    Path64{Point64{6, 0}, Point64{8, 0}, Point64{8, 9}, Point64{6, 9}},
//                    Path64{Point64{2, 9}, Point64{2, 8}, Point64{6, 8}, Point64{6, 9}},
//                    Path64{Point64{2, 8}, Point64{6, 8}, Point64{6, 5}, Point64{2, 5}},
//                    Path64{Point64{3, 7}, Point64{3, 6}, Point64{5, 6}, Point64{5, 7}},
//                    Path64{Point64{2, 5}, Point64{2, 4}, Point64{6, 4}, Point64{6, 5}},
//                    Path64{Point64{2, 4}, Point64{6, 4}, Point64{6, 2}, Point64{2, 2}},
//                    Path64{Point64{2, 2}, Point64{2, 0}, Point64{6, 0}, Point64{6, 2}}};
//
//    Clipper64 cp;
//    cp.AddSubject(subject);
//    Paths64 clip_solution;
//    PolyTree64 polytree_clip_solution;
//    const bool ret = cp.Execute(ClipType::Union, FillRule::Positive, clip_solution);
//    cp.Execute(ClipType::Union, FillRule::Positive, polytree_clip_solution);
//
//    SvgWriter svg_origin, svg;
//
//    SvgAddSolution(svg_origin, subject, FillRule::Positive, true);
//    SvgAddSolution(svg, clip_solution, FillRule::Positive, true);
//    SvgSaveToFile(svg_origin, "solution_origin.svg", 450, 450, 10);
//    SystemTest("solution_origin.svg");
//    SvgSaveToFile(svg, "solution.svg", 450, 450, 10);
//    SystemTest("solution.svg");
// }

// TEST(Clipper2Tests, TestNested2Ok) {
//     using namespace Clipper2Lib;
//     Paths64 subject{Path64{Point64{0, 0}, Point64{8, 0}, Point64{8, 9}, Point64{0, 9}},
//                     Path64{Point64{2, 9}, Point64{2, 8}, Point64{6, 8}, Point64{6, 9}},
//                     Path64{Point64{2, 8}, Point64{6, 8}, Point64{6, 5}, Point64{2, 5}},
//                     Path64{Point64{3, 7}, Point64{3, 6}, Point64{5, 6}, Point64{5, 7}},
//                     Path64{Point64{2, 5}, Point64{2, 4}, Point64{6, 4}, Point64{6, 5}},
//                     Path64{Point64{2, 4}, Point64{6, 4}, Point64{6, 2}, Point64{2, 2}},
//                     Path64{Point64{2, 2}, Point64{2, 0}, Point64{6, 0}, Point64{6, 2}}};
//
//     Clipper64 cp;
//     cp.AddSubject(subject);
//     Paths64 clip_solution;
//     PolyTree64 polytree_clip_solution;
//     const bool ret = cp.Execute(ClipType::Union, FillRule::Positive, clip_solution);
//     cp.Execute(ClipType::Union, FillRule::Positive, polytree_clip_solution);
//
//     SvgWriter svg_origin, svg;
//
//     SvgAddSolution(svg_origin, subject, FillRule::Positive, true);
//     SvgAddSolution(svg, clip_solution, FillRule::Positive, true);
//     SvgSaveToFile(svg_origin, "solution_origin.svg", 450, 450, 10);
//     SystemTest("solution_origin.svg");
//     SvgSaveToFile(svg, "solution.svg", 450, 450, 10);
//     SystemTest("solution.svg");
// }

//TEST(Clipper2Tests, TestChildParentAssociation) {
//    using namespace Clipper2Lib;
//    Paths64 subject;
//    subject.push_back(MakePath("641680, 906000, 247280, 906000, 247280, 700000, 253680, 700000, 253680, 901280, 635280, 901280, 635280, 700000, 641680, 700000"));
//    subject.push_back(MakePath("635280, 901280, 253680, 901280, 253680, 860780, 635280, 860780"));
//    subject.push_back(MakePath("635200, 901200, 635200, 860860, 253760, 860860, 253760, 901200"));
//    subject.push_back(MakePath("635280, 860780, 253680, 860780, 253680, 844500, 635280, 844500"));
//    subject.push_back(MakePath("635280, 844500, 635200, 844500, 635200, 814060, 635280, 814060"));
//    subject.push_back(MakePath("253760, 844500, 253680, 844500, 253680, 814060, 253760, 814060"));
//    subject.push_back(MakePath("624560, 837460, 264400, 837460, 264400, 821100, 624560, 821100"));
//    subject.push_back(MakePath("635280, 814060, 253680, 814060, 253680, 797700, 635280, 797700"));
//
//    Clipper64 cp;
//    cp.PreserveCollinear = false;
//    cp.AddSubject(subject);
//    PolyTree64 clip_solution2;
//
//    const bool ret2 = cp.Execute(ClipType::Union, FillRule::NonZero, clip_solution2);
//    
//    Paths64 clip_solution;
//    const bool ret = cp.Execute(ClipType::Union, FillRule::NonZero, clip_solution);
//    SvgWriter svg, svg2;
//    SvgAddSolution(svg, subject, FillRule::Positive, true);
//    SvgAddSolution(svg2, clip_solution, FillRule::Positive, true);
//    SvgSaveToFile(svg, "Issue_origin.svg", 1280, 1280,10);
//    SvgSaveToFile(svg2, "Issue_result.svg",  1280, 1280,10);
//    SystemTest("Issue_origin.svg");
//    SystemTest("Issue_result.svg");
//}

// TEST(Clipper2Tests, TestChildParentAssociation) {
//     using namespace Clipper2Lib;
//     Paths64 subject, clip;
//     subject.push_back(MakePath("641680, 906000, 247280, 906000, 247280, 700000, 253680, 700000, 253680, 901280, 635280, 901280, 635280, 700000, 641680, 700000"));
//     subject.push_back(MakePath("635280, 901280, 253680, 901280, 253680, 860780, 635280, 860780"));
//     subject.push_back(MakePath("635200, 901200, 635200, 860860, 253760, 860860, 253760, 901200"));
//     subject.push_back(MakePath("635280, 860780, 253680, 860780, 253680, 844500, 635280, 844500"));
//     subject.push_back(MakePath("635280, 844500, 635200, 844500, 635200, 814060, 635280, 814060"));
//     subject.push_back(MakePath("253760, 844500, 253680, 844500, 253680, 814060, 253760, 814060"));
//     subject.push_back(MakePath("624560, 837460, 264400, 837460, 264400, 821100, 624560, 821100"));
//     subject.push_back(MakePath("635280, 814060, 253680, 814060, 253680, 797700, 635280, 797700"));


//     PolyTree64 pt;
//     BooleanOp(ClipType::Union, FillRule::Positive, subject, clip, pt);
//     DisplayPolyTree(pt);
// }