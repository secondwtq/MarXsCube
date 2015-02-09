//
//  Timer.h
//  MarXsCube
//
//  Created by SeconDatke on 2/7/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Timer__
#define __MarXsCube__Timer__

#include <cstddef>

void timer_init();
double time_in_millisec();

class fps_counter {
	
	public:
	
		double fps = 0;
		bool updated = false;
	
		fps_counter() { };
	
		void update() {
			double time_current = time_in_millisec();
			if (this->m_first) {
				this->m_time_start = time_current;
				this->m_first = false;
			}
			if (updated) this->updated = false;
			this->m_cframe++;
			if (time_current - this->m_time_start > 1000 && this->m_cframe > 100) {
				this->fps = (double)this->m_cframe / ((time_current - this->m_time_start)/1000.0);
				this->m_time_start = time_current;
				this->m_cframe = 0;
				this->updated = true;
			}
		}
		
	private:
		bool m_first = true;
		double m_time_start = -1.;
		std::size_t m_cframe = 0;
	
};

#endif /* defined(__MarXsCube__Timer__) */