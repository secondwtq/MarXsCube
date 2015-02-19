//
//  Path.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Path.h"

#include "PathUtils.h"
#include "PathAux.h"
#include "PathAux2.h"
#include <numeric>
#include <algorithm>
#include <cassert>

typedef Path::vec_type vec_type;

namespace {

	typedef std::vector<vec_type> Vec3Container;
	typedef std::vector<float> FloatContainer;

	void updateSegmentTangentAndLength(std::size_t segmentIndex, Vec3Container const& points, 
						Vec3Container& segmentTangents, FloatContainer& segmentLengths) {
		assert( ( ( segmentIndex + 1 ) < points.size() ) &&
			   "Not enough points for segment segmentIndex." );
		assert( segmentIndex < segmentTangents.size() &&
			   "segmentIndex out of range." );
		assert( segmentTangents.size() == segmentLengths.size() && "segmentTangents and segmentLengths must have the same size." );
		
		vec_type tangent = points[segmentIndex + 1] - points[segmentIndex];
		float const length = tangent.length();
		assert(!zero(length) && "Segments must have lengths greater than 0.");
		
		tangent /= length;
		
		segmentTangents[ segmentIndex ] = tangent;
		segmentLengths[ segmentIndex] = length;
	}
	

	void updateTangentsAndLengths(Vec3Container const& points, Vec3Container& segmentTangents, FloatContainer& segmentLengths, 
		std::size_t firstChangedPointIndex, std::size_t numOfPoints, bool isCyclic) {

		assert( 0 < numOfPoints && "Only call if points have really changed." );
		assert( 1 < points.size() && "Two points are needed for a segment." );
		assert( points.size() == segmentTangents.size() + 1 &&
			   "Two points are needed for one segment, therefore there must be one segment less than points." );
		assert( segmentTangents.size() == segmentLengths.size() && "segmentTangents and segmentLengths must have the same size." );
		
		// The segment with end point @a firstChangedPointIndex has also
		// changed. Beware from range underflow by subtraction.
		std::size_t firstSegmentIndex = firstChangedPointIndex;
		if (0 < firstSegmentIndex) {
			firstSegmentIndex -= 1; }
		
		// The last segment to update has the last changed point as its start
		// point. This only holds true if the last changed point isn't the last
		// point.
		// lastSegmentIndex is one greater than the real last segment to update
		// like the last iterators in the stl.
		std::size_t lastSegmentIndex = firstChangedPointIndex + numOfPoints;
		lastSegmentIndex = clamp(lastSegmentIndex, static_cast<size_t>(0), segmentTangents.size());
		
		for (std::size_t i = firstSegmentIndex; i < lastSegmentIndex; ++i) {
			updateSegmentTangentAndLength(i, points, segmentTangents, segmentLengths); }
		
		// If path is cyclic and the first point changed and the cycle closing
		// segment hasn't been updated update it now.
		if ( isCyclic &&
			( 0 == firstSegmentIndex ) &&
			! ( lastSegmentIndex == segmentTangents.size() ) ) {
			
			updateSegmentTangentAndLength( segmentTangents.size() - 1,
										  points,
										  segmentTangents,
										  segmentLengths );
		}
		
	}
	
	template<typename Iterator>
	bool adjacentPathPointsDifferent(Iterator first, Iterator last, bool closedCycle) {
		
		assert(last - first > 1 && "A path needs at least two waypoints.");
		
		if (last != std::adjacent_find(first, last)) {
			return false; }
		
		if (closedCycle) {
			Iterator before_last(first);
			std::advance(before_last, last - first - 1);
			return *first == *before_last;
		}
		
		return true;
	}
	
}

Path::Path() : points_(0), segmentTangents_(0), segmentLengths_(0), closedCycle_(false) { }

Path::Path(std::size_t numOfPoints, vec_type const newPoints[], bool closedCycle)
	: points_(0), segmentTangents_(0), segmentLengths_(0), closedCycle_(closedCycle)
	{ setPath(numOfPoints, newPoints, closedCycle); }

Path::Path(Path const& other) : points_(other.points_), segmentTangents_(other.segmentTangents_), 
	segmentLengths_(other.segmentLengths_), closedCycle_(other.closedCycle_) { /* Nothing to do. */ }

Path::~Path() { /* Nothing to do. */ }

Path& Path::operator=(Path other) {
	swap(other);
	return *this;
}

void Path::swap(Path& other) {
	points_.swap(other.points_);
	segmentTangents_.swap(other.segmentTangents_);
	segmentLengths_.swap(other.segmentLengths_);
	std::swap(closedCycle_, other.closedCycle_);
}

void Path::setPath(std::size_t numOfPoints, vec_type const newPoints[], bool closedCycle) {
	assert( 1 < numOfPoints && "Path must have at least two distinct points." );
	// While testing say that no cyclus is used because the first point hasn't
	// been copied to the back.
	assert( adjacentPathPointsDifferent( newPoints, newPoints + numOfPoints, false ) && "Adjacent path points must be different." );
	
	closedCycle_ = closedCycle;
	
	std::size_t numberOfPoints = numOfPoints;
	if ( closedCycle_ ) {
		++numberOfPoints;
	}
	
	points_.reserve( numberOfPoints );
	segmentTangents_.resize( numberOfPoints - 1 );
	segmentLengths_.resize( numberOfPoints - 1 );
	
	points_.assign( newPoints, newPoints + numOfPoints );
	
	if ( closedCycle_ ) {
		points_.push_back( points_[ 0 ] );
	}
	
	updateTangentsAndLengths( points_ ,
							 segmentTangents_,
							 segmentLengths_,
							 0,
							 numOfPoints,
							 closedCycle_ );
	
	shrink_to_fit(points_);
	shrink_to_fit(segmentTangents_);
	shrink_to_fit(segmentLengths_);
}

void Path::movePoints(std::size_t startIndex, std::size_t numOfPoints, vec_type const newPoints[]) {
	assert( ( startIndex < ( pointCount() - ( isCyclic() ? 1 : 0 ) ) ) &&
		   "startIndex must be inside index range." );
	assert( ( ( startIndex + numOfPoints ) <= ( pointCount() - ( isCyclic() ? 1 : 0 ) ) ) &&
		   "The max. index of a point to set must be inside the index range." );
	
	// Update the point positions.
	// @todo Remove this line std::size_t const pathPointCount = pointCount();
	for ( std::size_t i = 0; i < numOfPoints; ++i ) {
		points_[ startIndex + i ] = newPoints[ i ];
	}
	
	// If the first point is changed and the path is cyclic also change the
	// last point, which is just a copy of the first point.
	if ( isCyclic() && ( 0 == startIndex ) ) {
		points_.back() = points_.front();
	}
	
	// Recalculate the tangents and lengths.
	updateTangentsAndLengths( points_,
							 segmentTangents_,
							 segmentLengths_,
							 startIndex,
							 numOfPoints,
							 isCyclic() );
	
	
	assert( adjacentPathPointsDifferent( points_.begin(), points_.end(), isCyclic() ) && "Adjacent path points must be different." );
}

bool Path::isValid() const {
	return pointCount() > 1; }

vec_type Path::mapPointToPath(const vec_type& point, vec_type& tangent, float& outside) const {
	PointToPathMapping mapping;
	mapPointToPathAlike(*this, point, mapping);
	tangent = mapping.tangent;
	outside = mapping.distancePointToPath;
	return mapping.pointOnPathCenterLine;
}

vec_type Path::mapPathDistanceToPoint (float pathDistance) const {
	PathDistanceToPointMapping mapping;
	mapDistanceToPathAlike(*this, pathDistance, mapping);
	return mapping.pointOnPathCenterLine;
}

float Path::mapPointToPathDistance (const vec_type& point) const {
	PointToPathDistanceMapping mapping;
	mapPointToPathAlike(*this, point, mapping);
	return mapping.distanceOnPath;
}

bool Path::isCyclic() const {
	return closedCycle_; }

float Path::length() const {
	return std::accumulate(segmentLengths_.begin(), segmentLengths_.end(), 0.0f); }

std::size_t Path::pointCount() const {
	return points_.size(); }

vec_type Path::point(std::size_t pointIndex) const {
	assert(pointIndex < pointCount() && "pointIndex out of range.");
	return points_[pointIndex];
}

std::size_t Path::segmentCount() const {
	return segmentTangents_.size(); }

float Path::segmentLength(std::size_t segmentIndex) const {
	assert(segmentIndex < segmentCount() && "segmentIndex out of range.");
	return segmentLengths_[segmentIndex];
}

vec_type Path::segmentStart(std::size_t segmentIndex) const {
	assert(segmentIndex < segmentCount() && "segmentIndex out of range.");
	assert(segmentIndex < pointCount() && "The max. index of a point must be inside range.");
	return points_[segmentIndex];
}

vec_type Path::segmentEnd(std::size_t segmentIndex) const {
	assert(segmentIndex < segmentCount() && "segmentIndex out of range.");
	assert(segmentIndex + 1< pointCount() && "The max. index of a point must be inside range.");
	
	return points_[segmentIndex + 1];
}

float Path::mapPointToSegmentDistance(std::size_t segmentIndex, vec_type const& point) const {
	assert(segmentIndex < segmentCount() && "segmentIndex is out of range.");
	
	vec_type const segmentStartToPoint(point - points_[segmentIndex]);
	const float distance = glm::dot(segmentStartToPoint, segmentTangents_[segmentIndex]);
	
	return clamp(distance, 0.0f, segmentLengths_[segmentIndex]);
}

vec_type Path::mapSegmentDistanceToPoint(std::size_t segmentIndex, float segmentDistance) const {
	assert(segmentIndex < segmentCount() && "segmentIndex is out of range." );
	float const segmentLength = segmentLengths_[segmentIndex];
	segmentDistance = clamp(segmentDistance, 0.0f, segmentLength);
	return segmentTangents_[segmentIndex] * segmentDistance + points_[segmentIndex];
}

vec_type Path::mapSegmentDistanceToTangent(std::size_t segmentIndex, float) const {
	assert(segmentIndex < segmentCount() && "segmentIndex is out of range.");
	return segmentTangents_[segmentIndex];
}

void Path::mapDistanceToSegmentPointAndTangent(std::size_t segmentIndex, 
		float segmentDistance, vec_type& pointOnPath, vec_type& tangent) const {
	assert( segmentIndex < segmentCount() && "segmentIndex is out of range." );
	
	float const segmentLength = segmentLengths_[segmentIndex];

	segmentDistance = clamp(segmentDistance, 0.0f, segmentLength);
	
	pointOnPath = segmentTangents_[segmentIndex] * segmentDistance + points_[segmentIndex];
	tangent = segmentTangents_[segmentIndex];
}

void Path::mapPointToSegmentDistanceAndPointAndTangent(std::size_t segmentIndex, vec_type const& point, 
									float& distance, vec_type& pointOnPath, vec_type& tangent) const {
	assert( segmentIndex < segmentCount() && "segmentIndex is out of range." );
	
	vec_type const segmentStartPoint = points_[ segmentIndex ];
	vec_type const segmentStartToPoint( point - segmentStartPoint );
	tangent = segmentTangents_[segmentIndex];
	distance = glm::dot(segmentStartToPoint, tangent);
	distance =  clamp( distance, 0.0f, segmentLengths_[ segmentIndex ] );
	pointOnPath = tangent * distance + segmentStartPoint;
}