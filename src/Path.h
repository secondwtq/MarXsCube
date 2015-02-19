//
//  Path.h
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__Path__
#define __MarXsCube__Path__

#include "Common.h"
#include <vector>
#include <glm/glm.hpp>

class Path {
public:
	
	typedef glm::vec3 vec_type;

	Path();
	Path(std::size_t numOfPoints, vec_type const newPoints[], bool closedCycle);
	Path(Path const& other);

	virtual ~Path();
	Path& operator=(Path other);
	void swap(Path& other);

	void setPath(std::size_t numOfPoints, vec_type const newPoints[], bool closedCycle);
	void movePoints(std::size_t startIndex, std::size_t numOfPoints, vec_type const newPoints[]);

	virtual bool isValid() const;
	virtual vec_type mapPointToPath(const vec_type& point, vec_type& tangent, float& outside) const;
	virtual vec_type mapPathDistanceToPoint(float pathDistance) const;
	virtual float mapPointToPathDistance(const vec_type& point) const;
	virtual bool isCyclic() const;
	virtual float length() const;

	virtual std::size_t pointCount() const;
	virtual vec_type point(std::size_t pointIndex) const;


	virtual std::size_t segmentCount() const;
	virtual float segmentLength(std::size_t segmentIndex) const;
	virtual vec_type segmentStart(std::size_t segmentIndex) const;
	virtual vec_type segmentEnd(std::size_t segmentIndex) const;
	virtual float mapPointToSegmentDistance(std::size_t segmentIndex, vec_type const& point) const;
	virtual vec_type mapSegmentDistanceToPoint(std::size_t segmentIndex, float segmentDistance) const;
	virtual vec_type mapSegmentDistanceToTangent(std::size_t segmentIndex, float segmentDistance) const;

	virtual void mapDistanceToSegmentPointAndTangent(std::size_t segmentIndex, float distance, vec_type& pointOnPath, vec_type& tangent) const;

	virtual void mapPointToSegmentDistanceAndPointAndTangent(std::size_t segmentIndex, vec_type const& point, float& distance, vec_type& pointOnPath, vec_type& tangent) const;

private:

	std::vector<vec_type> points_;
	std::vector<vec_type> segmentTangents_;
	std::vector<float> segmentLengths_;
	bool closedCycle_;
};

inline void swap(Path& lhs, Path& rhs) { lhs.swap(rhs); }

template<class PathAlike>
class PointToPathAlikeBaseDataExtractionPolicy;

template<class PathAlike>
class DistanceToPathAlikeBaseDataExtractionPolicy;

template<>
class PointToPathAlikeBaseDataExtractionPolicy<Path> {
public:

static void extract(Path const& pathAlike, std::size_t segmentIndex, Path::vec_type const& point,
	float& segmentDistance, float&, float& distancePointToPath, Path::vec_type& pointOnPathCenterLine, Path::vec_type& tangent) {
	pathAlike.mapPointToSegmentDistanceAndPointAndTangent(segmentIndex, point, segmentDistance, pointOnPathCenterLine, tangent);
	distancePointToPath = glm::distance(point, pointOnPathCenterLine); }

};

template<>
class DistanceToPathAlikeBaseDataExtractionPolicy<Path> {
public:
	static void extract(Path const& pathAlike, std::size_t segmentIndex, float segmentDistance, 
						Path::vec_type& pointOnPathCenterLine, Path::vec_type& tangent, float&) {
		pathAlike.mapDistanceToSegmentPointAndTangent(segmentIndex, segmentDistance, pointOnPathCenterLine, tangent); }

};

#endif /* defined(__MarXsCube__Path__) */
