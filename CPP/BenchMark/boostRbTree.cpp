#include <boost/intrusive/rbtree.hpp>
#include <cassert>
#include <iostream>
struct MyData
{
    int key;
    // Other members of your data structure
    boost::intrusive::set_member_hook<> member_hook;
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
    boost::intrusive::set_member_hook<> member_hook;
    Active(int curr_x_):curr_x(curr_x_) {};
};

struct ActiveEdgeCompare
{
    bool operator()(const Active& lhs, const Active& rhs)const
    {
        return lhs.curr_x < rhs.curr_x;
    }
};

struct MyDataCompare
{
    bool operator()(const MyData& lhs, const MyData& rhs) const
    {
        return lhs.key < rhs.key;
    }
};
using MyRbTree = boost::intrusive::rbtree<MyData,
                                           boost::intrusive::member_hook<MyData, boost::intrusive::set_member_hook<>, &MyData::member_hook>,
                                           boost::intrusive::compare<MyDataCompare>>;
using ActiveRbTree = boost::intrusive::rbtree<Active,
    boost::intrusive::member_hook<Active, boost::intrusive::set_member_hook<>, &Active::member_hook>,
    boost::intrusive::compare<ActiveEdgeCompare>>;

int main ()
{
    ActiveRbTree active_rb_tree;
    Active data1{42}; // Example data element

    Active data2{17}; // Another example data element
    Active data3{20};
 
    active_rb_tree.insert_unique(data1);
    active_rb_tree.insert_unique(data2);
    active_rb_tree.insert_unique(data3);
    active_rb_tree.insert_unique(data3);
    for(auto iter= active_rb_tree.begin(); iter!= active_rb_tree.end() ;iter++)
    {
        auto value = iter->curr_x;
        std::cout << "value is: " << value << std::endl;
    }


    return 0;
}