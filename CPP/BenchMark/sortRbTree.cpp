#include <boost/intrusive/rbtree.hpp>
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <cassert>
#include <iostream>

struct Active {
  // Point64 bot;
  // Point64 top;
  int64_t curr_x = 0;     // current (updated at every new scanline)
  int64_t sel_curr_x = 0; // current (updated at every new scanline)
  double dx = 0.0;
  int wind_dx = 1; // 1 or -1 depending on winding direction
  int wind_cnt = 0;
  int wind_cnt2 = 0; // winding count of the opposite polytype
  // OutRec* outrec = nullptr;
  // AEL: 'active edge list' (Vatti's AET - active edge table)
  //     a linked list of all edges (from left to right) that are present
  //     (or 'active') within the current scanbeam (a horizontal 'beam' that
  //     sweeps from bottom to top over the paths in the clipping operation).
  Active *prev_in_ael = nullptr;
  Active *next_in_ael = nullptr;
  // SEL: 'sorted edge list' (Vatti's ST - sorted table)
  //     linked list used when sorting edges into their new positions at the
  //     top of scanbeams, but also (re)used to process horizontals.
  Active *prev_in_sel = nullptr;
  Active *next_in_sel = nullptr;
  Active *jump = nullptr;
  /*
      Vertex* vertex_top = nullptr;
      LocalMinima* local_min = nullptr;  // the bottom of an edge 'bound' (also
     Vatti) bool is_left_bound = false; JoinWith join_with = JoinWith::None;
  */
  boost::intrusive::set_member_hook<> member_hook;
  Active(int curr_x_) : curr_x(curr_x_){};
};

struct ActiveEdgeCompare {
  bool operator()(const Active &lhs, const Active &rhs) const {
    return lhs.curr_x < rhs.curr_x;
  }
};
using ActiveRbTree = boost::intrusive::rbtree<
    Active,
    boost::intrusive::member_hook<Active, boost::intrusive::set_member_hook<>,
                                  &Active::member_hook>,
    boost::intrusive::compare<ActiveEdgeCompare>>;

void ConstructSortedEdgeList(Active *& sel, ActiveRbTree &active_rb_tree) {
  sel = &*active_rb_tree.begin();
  int val = 4;
  for (auto iter = active_rb_tree.begin();
       iter != active_rb_tree.end(); iter++) {

    iter->prev_in_sel = iter == active_rb_tree.begin() ? nullptr :&*std::prev(iter);
    iter->next_in_sel = std::next(iter) == active_rb_tree.end() ? nullptr: &*std::next(iter);
    iter->jump = iter->next_in_sel;
    // if (e->join_with == JoinWith::Left)
    //   e->curr_x = e->prev_in_ael->curr_x; // also avoids complications
    // else
    iter->sel_curr_x = val--;
  }
  
  return;
}

inline Active *ExtractFromSEL(Active *ae) {
  Active *res = ae->next_in_sel;
  if (res)
    res->prev_in_sel = ae->prev_in_sel;
  ae->prev_in_sel->next_in_sel = res;
  return res;
}

inline void Insert1Before2InSEL(Active *ae1, Active *ae2) {
  ae1->prev_in_sel = ae2->prev_in_sel;
  if (ae1->prev_in_sel)
    ae1->prev_in_sel->next_in_sel = ae1;
  ae1->next_in_sel = ae2;
  ae2->prev_in_sel = ae1;
}

std::vector<std::pair<Active *, Active *>> MergeSort(Active *&sel_) {
  if (!sel_ || !sel_->next_in_sel)
    return {};

  Active *left = sel_, *right, *l_end, *r_end, *curr_base, *tmp;
  std::vector<std::pair<Active *, Active *>> intersection_pair_list;

  while (left && left->jump) {
    Active *prev_base = nullptr;
    while (left && left->jump) {
      curr_base = left;
      right = left->jump;
      l_end = right;
      r_end = right->jump;
      left->jump = r_end;
      while (left != l_end && right != r_end) {
        if (right->sel_curr_x < left->sel_curr_x) {
          tmp = right->prev_in_sel;
          for (;;) {
            //   AddNewIntersectNode(*tmp, *right, top_y);
            intersection_pair_list.push_back({tmp, right});
            std::cout << "here is a intersection" << std::endl;
            if (tmp == left)
              break;
            tmp = tmp->prev_in_sel;
          }

          tmp = right;
          right = ExtractFromSEL(tmp);
          l_end = right;
          Insert1Before2InSEL(tmp, left);
          if (left == curr_base) {
            curr_base = tmp;
            curr_base->jump = r_end;
            if (!prev_base)
              sel_ = curr_base;
            else
              prev_base->jump = curr_base;
          }
        } else
          left = left->next_in_sel;
      }
      prev_base = curr_base;
      left = r_end;
    }
    left = sel_;
  }
  return intersection_pair_list;
}

int main() {
  ActiveRbTree active_rb_tree;
  Active *sel = nullptr; // use this to construct linked list for mergesort
  // this is difficult to use

  Active data1{1}; // Example data element
  Active data2{2}; // Another example data element
  Active data3{3};
  Active data4{4};

  active_rb_tree.insert_unique(data1);
  active_rb_tree.insert_unique(data2);
  active_rb_tree.insert_unique(data3);
  active_rb_tree.insert_unique(data4);

  ConstructSortedEdgeList(sel, active_rb_tree);
  auto intersection_list = MergeSort(sel);
  active_rb_tree.clear(); // avoid trigger the assertion
  return 0;
}