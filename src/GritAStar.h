//
//  GritAStar.h
//  MarXsCube
//
//  Created by SeconDatke on 2/6/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__GritAStar__
#define __MarXsCube__GritAStar__

#include "GritMath.h"
#include "GritFoundation.h"
#include <algorithm>
#include <vector>

class GritAStar {
	static float GA_HEURIST_WEIGHT;
	
	template <typename T>
	class pri_queue {
	public:
		
		inline std::size_t size() const
			{ return this->m_data.size(); }
		
		inline std::size_t push(T *src) {
			this->m_data.push_back(src);
			this->resort();
			return this->size();
		}
		
		inline T *front() {
			if (this->size()) return this->m_data[0];
			else return nullptr; }
		
		inline bool contains(T *src) {
			for (std::size_t i = 0; i < this->size(); i++)
				if (this->m_data[i] == src)
					return true;
			return false;
		}
		
		inline void remove(T *src) {
			this->m_data.erase(std::remove(m_data.begin(), m_data.end(), src), m_data.end());
			this->resort(); }
		
	private:
		
		inline void resort() {
			std::sort(this->m_data.begin(), this->m_data.end(), [] (T *a, T *b) {
				return (*a) < (*b); }); }
		
		std::vector<T *> m_data;
		
	};
	
	static float estimate(const GritNode& cur, const GritNode& end, float weight) {
		return weight * (gmag2(gsub2(cur.pos, end.pos))); }
	
	static float cost(const GritNode& a, const GritNode& b) {
		return gmag2(gsub2(a.pos, b.pos)); }
	
public:
	static void path(GritNode *start, GritNode *end, std::vector<GritNode *> *all_nodes, std::vector<GPointType> *cache);
	
};

#endif /* defined(__MarXsCube__GritAStar__) */
