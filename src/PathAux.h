//
//  PathAux.h
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__PathAux__
#define __MarXsCube__PathAux__

#include <glm/glm.hpp>

class ExtractPathDistance {
public:
	void setDistanceOnPathFlag(float distance) {
		distanceOnPathFlag_ = distance; }
	
	float distanceOnPathFlag() const {
		return distanceOnPathFlag_; }
	
protected:
	ExtractPathDistance() : distanceOnPathFlag_(0.0f) { }
	
	explicit ExtractPathDistance(float distance) : distanceOnPathFlag_(distance)
	{ /* Nothing to do. */ }
	
	~ExtractPathDistance() { }
	
private:
	float distanceOnPathFlag_;
};

class DontExtractPathDistance {
public:
	void setDistanceOnPathFlag(float) { }
	
	float distanceOnPathFlag() const { return 0.0f; };
	
protected:

	~DontExtractPathDistance() { }
};

class PointToPathMapping
: public DontExtractPathDistance {
	
public:
	
	PointToPathMapping() : pointOnPathCenterLine(0.0f, 0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f), distancePointToPath(0.0f) {}
	
	void setPointOnPathCenterLine(glm::vec3 const& point) {
		pointOnPathCenterLine = point; }
	
	void setPointOnPathBoundary(glm::vec3 const&) { }
	
	void setRadius(float) {}
	void setTangent(glm::vec3 const& t) {
		tangent = t; }
	void setSegmentIndex(size_t) {}
	void setDistancePointToPath(float distance) {
		distancePointToPath = distance; }
	void setDistancePointToPathCenterLine(float) {}
	void setDistanceOnPath(float) {}
	void setDistanceOnSegment(float) {}
	
	glm::vec3 pointOnPathCenterLine;
	// Vec3 pointOnPathBoundary;
	glm::vec3 tangent;
	float distancePointToPath;
	
};

class PathDistanceToPointMapping : public DontExtractPathDistance {
	
public:
	
	void setPointOnPathCenterLine(glm::vec3 const& vec){
		pointOnPathCenterLine = vec; }
	void setRadius(float) {}
	void setTangent(glm::vec3 const&) { }
	void setSegmentIndex(size_t) { }
	void setDistanceOnPath(float) { }
	void setDistanceOnSegment(float) { }
	
	glm::vec3 pointOnPathCenterLine;
	
};

class PointToPathDistanceMapping : public ExtractPathDistance {
public:
	PointToPathDistanceMapping() : distanceOnPath(0.0f) { }
	
	void setPointOnPathCenterLine(glm::vec3 const&) { }
	void setPointOnPathBoundary(glm::vec3 const&) { }
	void setRadius(float) { }
	void setTangent(glm::vec3 const&) { }
	void setSegmentIndex(size_t) {}
	void setDistancePointToPath(float) { }
	void setDistancePointToPathCenterLine(float) { }
	void setDistanceOnPath(float distance) {
		distanceOnPath = distance; }
	void setDistanceOnSegment(float) { }
	
	float distanceOnPath;
};

#endif /* defined(__MarXsCube__PathAux__) */
