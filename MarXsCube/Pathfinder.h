#ifndef CUBE_E_PATHFINDER_H
#define CUBE_E_PATHFINDER_H

#include "Common.h"

#include <vector>

class Abs_Cell;
class Abs_Object;

namespace Pathfinding {

	class Repathcache {
		public:
			std::vector<Abs_Cell *> cache;

			bool inside(Abs_Cell *src) {
				for (auto c : cache)
					if (c == src) { return true; }
				return false;
			}

			void insert(Abs_Cell *src) { cache.push_back(src); }

			void clear() { cache.clear(); }

			std::size_t size() { return cache.size(); }

			void resize(std::size_t x) {
				auto iter = cache.begin();
				for (std::size_t i = 0; i < x; i++) iter++;
				cache.erase(iter, cache.end());
			}
	};

	class PathFindingcache {
		public:
			std::size_t idx = 0;
			std::vector<Abs_Cell *> cache;

			void reset() {LOGFUNC; idx = 0; cache.clear(); }
		
			void setobegin() { idx = 0; }
		
			void init(std::vector<Abs_Cell *> &src) {
				this->reset();
//				this->cache = src;
				for (auto cell : src) {
					this->cache.push_back(cell);
				}
			}
		
			int size() { return this->cache.size(); }

			void initFromRev(std::vector<Abs_Cell *> &src) {LOGFUNC;
				reset();
				for (auto r = src.crbegin(); r != src.crend(); ++r)
					cache.push_back(*r);
			}
		
			void push(Abs_Cell *src) {
				this->cache.push_back(src);
			}

			void inc() {LOGFUNC; if(idx < cache.size()) idx++; }

			Abs_Cell *getCur() {LOGFUNC; return cache[idx]; }
			Abs_Cell *getNex() {LOGFUNC;
				if (idx < cache.size()-1) { return cache[idx+1]; }
				return cache[idx];
			}

			Abs_Cell *getNexNex() {LOGFUNC;
				if (idx < cache.size()-2) { return cache[idx+2]; }
				return cache[idx];
			}

			bool ended() {LOGFUNC; return idx > cache.size()-1; }
		
			bool aboutoend() { return idx >= cache.size() - 1; }

			bool inside(Abs_Cell *src) {LOGFUNC;
				for (std::size_t i = idx; i < cache.size(); i++)
					if (src == cache[i]) return true;
				return false;
			}

			void jumpTo(Abs_Cell *src) {LOGFUNC;
				for (std::size_t i = idx; i < cache.size(); i++)
					if (src == cache[i]) {
						idx = i+1;
						return;
					}
			}
	};

	void init(int _x, int _y);
	bool Find(Abs_Object *obj, Abs_Cell *src, Abs_Cell *dest, PathFindingcache *cache);
	
	void smooth_slow(PathFindingcache *src, Abs_Object *object);
}

#include "Cell.h"
#include "Object.h"

#endif