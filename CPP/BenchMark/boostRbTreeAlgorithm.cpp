/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2006-2013
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
//[doc_rbtree_algorithms_code
#include <boost/intrusive/rbtree_algorithms.hpp>
#include <cassert>

struct my_node
{
   my_node(int i = 0)
      :  int_(i)
   {}
   my_node *parent_, *left_, *right_;
   int      color_;
   //other members
   int      int_;
};

struct Active {
	//Point64 bot;
	//Point64 top;
	int64_t curr_x = 0;		//current (updated at every new scanline)
	double dx = 0.0;
	int wind_dx = 1;			//1 or -1 depending on winding direction
	int wind_cnt = 0;
	int wind_cnt2 = 0;		//winding count of the opposite polytype
	//OutRec* outrec = nullptr;
	//AEL: 'active edge list' (Vatti's AET - active edge table)
	//     a linked list of all edges (from left to right) that are present
	//     (or 'active') within the current scanbeam (a horizontal 'beam' that
	//     sweeps from bottom to top over the paths in the clipping operation).
	Active* prev_in_ael = nullptr;
	Active* next_in_ael = nullptr;
	//SEL: 'sorted edge list' (Vatti's ST - sorted table)
	//     linked list used when sorting edges into their new positions at the
	//     top of scanbeams, but also (re)used to process horizontals.
	Active* prev_in_sel = nullptr;
	Active* next_in_sel = nullptr;
	Active* jump = nullptr;
    /*
	Vertex* vertex_top = nullptr;
	LocalMinima* local_min = nullptr;  // the bottom of an edge 'bound' (also Vatti)
	bool is_left_bound = false;
	JoinWith join_with = JoinWith::None;
    */
   Active *parent_, *left_, *right_;
   int      color_;
   //other members
   Active(int curr_x_):curr_x(curr_x_) {};
};

//Define our own rbtree_node_traits
struct Active_rbtree_node_traits
{
   typedef Active                                    node;
   typedef Active *                                  node_ptr;
   typedef const Active *                            const_node_ptr;
   typedef int                                        color;
   static node_ptr get_parent(const_node_ptr n)       {  return n->parent_;   }
   static void set_parent(node_ptr n, node_ptr parent){  n->parent_ = parent; }
   static node_ptr get_left(const_node_ptr n)         {  return n->left_;     }
   static void set_left(node_ptr n, node_ptr left)    {  n->left_ = left;     }
   static node_ptr get_right(const_node_ptr n)        {  return n->right_;    }
   static void set_right(node_ptr n, node_ptr right)  {  n->right_ = right;   }
   static color get_color(const_node_ptr n)           {  return n->color_;    }
   static void set_color(node_ptr n, color c)         {  n->color_ = c;       }
   static color black()                               {  return color(0);     }
   static color red()                                 {  return color(1);     }
};

struct node_ptr_compare
{
   bool operator()(const Active *a, const Active *b)
   {  return a->curr_x < b->curr_x;  }
};

int main()
{
   typedef boost::intrusive::rbtree_algorithms<Active_rbtree_node_traits> algo;
   Active header(0), two(2), three(3);

   //Create an empty rbtree container:
   //"header" will be the header node of the tree
   algo::init_header(&header);

   //Now insert node "two" in the tree using the sorting functor
   algo::insert_equal_upper_bound(&header, &two, node_ptr_compare());

   //Now insert node "three" in the tree using the sorting functor
   algo::insert_equal_lower_bound(&header, &three, node_ptr_compare());

   //Now take the first node (the left node of the header)
   Active *n = header.left_;
   assert(n == &two);

   //Now go to the next node
   n = algo::next_node(n);
   assert(n == &three);

   //Erase a node just using a pointer to it
   algo::unlink(&two);

   //Erase a node using also the header (faster)
   algo::erase(&header, &three);
   return 0;
}

//]