//
//  graph_def.h
//  Gmap
//
//  data structure definition for Gmap, header only.
//

#ifndef GMP_GRAPH_DEF
#define GMP_GRAPH_DEF

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include <queue>

#include <cstdint>

namespace Gmap {

struct gGraph_listnode {
	public:
		std::size_t na = 0;
		std::size_t nb = 0;
		int weight = 0;

		gGraph_listnode(std::size_t verta, std::size_t vertb, int egweight = 0) :
			na(verta), nb(vertb), weight(egweight) { }
	
		gGraph_listnode(const gGraph_listnode& other) : na(other.na), nb(other.nb), weight(other.weight) { }

		bool operator==(const gGraph_listnode& op) const {
			return (this->na == op.na && this->nb == op.nb) || (this->na == op.nb && this->nb == op.na);
		}
	
		std::size_t other(std::size_t one) const {
			if (one == this->na) return this->nb;
			else return this->na;
		}

		friend std::ostream& operator<<(std::ostream& out, const gGraph_listnode& eg) {
			out << eg.na << " " << eg.nb << "(" << eg.weight << ")";
			return out;
		}
};

class gGraph {
	public:

		// you need to specific initial vertex counts
		// in the ctor
		gGraph(std::size_t nverts) : _count_vert(nverts) {
			for (std::size_t i = 0; i < nverts; i++) {
				this->adj_table[i] = std::unordered_set<gGraph_listnode *>();
				this->_mark[i] = false;
			}
		}
	
		~gGraph() {
			for (auto l : this->_edges)
				delete l;
		}

		std::size_t count_vert() const { return this->_count_vert; }

		std::size_t count_edge() const { return this->_count_edge; }

		void connect(std::size_t op1, std::size_t op2, int weight = 0) {
			if (!this->connected(op1, op2)) {
				auto edge = new gGraph_listnode(op1, op2, weight);
				this->adj_table.at(op1).insert(edge);
				this->adj_table.at(op2).insert(edge);
				this->_edges.push_back(edge);
				this->_count_edge++;
			}
		}

		void disconnect(std::size_t op1, std::size_t op2) {
			if (this->connected(op1, op2)) {
				//this->adj_table.at(op1).erase(op2);
				//this->adj_table.at(op2).erase(op1);
				this->_count_edge--;
			}
		}

		bool connected(std::size_t op1, std::size_t op2) const {
			for (auto edge : this->adj_table.at(op1))
				if ((edge->na == op1 && edge->nb == op2) || (edge->na == op2 && edge->nb == op1))
					return true;
			return false;
		}

		const std::unordered_set<gGraph_listnode *>& adj(std::size_t op) const { return this->adj_table.at(op); }

		const std::vector<gGraph_listnode *>& edges_all() { return this->_edges; }
	
		// functions about mark
		bool marked(std::size_t op) const { return this->_mark.at(op); }
		void mark(std::size_t op, bool mk) { this->_mark[op] = mk; }
		void clear_mark(bool mk = false) {
			for (std::pair<std::size_t, bool> mark_item : this->_mark)
				mark_item.second = false;
		}

	private:

		std::unordered_map<std::size_t, std::unordered_set<gGraph_listnode *>> adj_table;
		std::unordered_map<std::size_t, bool> _mark;
		std::vector<gGraph_listnode *> _edges;

		std::size_t _count_vert = 0;
		std::size_t _count_edge = 0;

};
	
}

// hash function for std::unordered_set & std::unordered_map
//	just hash the sum of vertex indexes.
//		from http://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
//
// it should be declared just below Gmap::gGraph_listnode
//		but I dunno know how to solve the namespace issue
template < >
struct std::hash<Gmap::gGraph_listnode> {
	std::size_t operator() (const Gmap::gGraph_listnode& op) const { return std::hash<std::size_t>()(op.na+op.nb); }
};

#endif
