//
//  Pathway.h
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Pathway__
#define __MarXsCube__Pathway__

#include "Path.h"

#include <glm/glm.hpp>

class Pathway {
public:

	typedef glm::vec3 vec_type;

	Pathway();
	explicit Pathway(float r);
	Pathway(std::size_t numOfPoints, vec_type const points[], float r, bool closeCycle);
	Pathway(Pathway const& other);
	virtual ~Pathway();
	Pathway& operator=(Pathway other);
	void swap(Pathway& other);

	void movePoints( std::size_t startIndex,
					std::size_t numOfPoints,
					vec_type const newPointValues[] );

	void setPathway( std::size_t numOfPoints,
					vec_type const points[],
					float r,
					bool closedCycle );

	void setRadius( float r );

	float radius() const;
	
	virtual bool isValid() const;
	virtual vec_type mapPointToPath(const vec_type& point, vec_type& tangent, float& outside) const;
	virtual vec_type mapPathDistanceToPoint(float pathDistance) const;
	virtual float mapPointToPathDistance(const vec_type& point) const;
	virtual bool isCyclic() const;
	virtual float length() const;
	
	virtual std::size_t pointCount() const;
	virtual vec_type point( std::size_t pointIndex ) const;
	
	virtual std::size_t segmentCount() const;
	virtual float segmentLength( std::size_t segmentIndex ) const;
	virtual vec_type segmentStart( std::size_t segmentIndex ) const;
	virtual vec_type segmentEnd( std::size_t segmentIndex ) const;
	virtual float mapPointToSegmentDistance( std::size_t segmentIndex,
											vec_type const& point ) const;
	virtual vec_type mapSegmentDistanceToPoint( std::size_t segmentIndex,
										   float segmentDistance ) const;
	virtual float mapSegmentDistanceToRadius( std::size_t segmentIndex,
											 float distanceOnSegment ) const;
	virtual vec_type mapSegmentDistanceToTangent( std::size_t segmentIndex,
											 float segmentDistance ) const;
	
	virtual void mapDistanceToSegmentPointAndTangentAndRadius( std::size_t segmentIndex,
															  float segmentDistance,
															  vec_type& pointOnPath,
															  vec_type& tangent,
															  float& radius ) const;
	
	virtual void mapPointToSegmentDistanceAndPointAndTangentAndRadius(std::size_t segmentIndex,
																	  vec_type const& point,
																	  float& distance,
																	  vec_type& pointOnPath,
																	  vec_type& tangent,
																	  float& radius) const;
	
private:
	Path path_;
	float radius_;
};

inline void swap(Pathway& lhs, Pathway& rhs) {
	lhs.swap(rhs); }

template<>
class PointToPathAlikeBaseDataExtractionPolicy<Pathway> {
public:
	
	static void extract(Pathway const& pathAlike, std::size_t segmentIndex, Pathway::vec_type const& point, float& segmentDistance, 
				float& radius, float& distancePointToPath, Pathway::vec_type& pointOnPathCenterLine, Pathway::vec_type& tangent) {
		pathAlike.mapPointToSegmentDistanceAndPointAndTangentAndRadius(segmentIndex, point, segmentDistance, pointOnPathCenterLine, tangent, radius);
		distancePointToPath = distance(point, pointOnPathCenterLine) - radius;
	}
	
};

template<>
class DistanceToPathAlikeBaseDataExtractionPolicy<Pathway> {
public:
	static void extract(Pathway const& pathAlike, std::size_t segmentIndex, float segmentDistance, 
			Pathway::vec_type& pointOnPathCenterLine, Pathway::vec_type& tangent, float& radius) {
		pathAlike.mapDistanceToSegmentPointAndTangentAndRadius(segmentIndex, segmentDistance, pointOnPathCenterLine, tangent, radius); }
};

#endif /* defined(__MarXsCube__Pathway__) */
