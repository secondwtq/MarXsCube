//
//  Pathway.cpp
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#include "Path.h"
#include "PathAux2.h"
#include "PathUtils.h"
#include "Pathway.h"

Pathway::Pathway() : path_(), radius_(0.0f) { }

Pathway::Pathway(float r) : path_(), radius_(r) { }

Pathway::Pathway(std::size_t numOfPoints, vec_type const points[], float r, bool closeCycle)
: path_(numOfPoints, points, closeCycle), radius_(r) { }

Pathway::Pathway(Pathway const& other) : path_(other.path_), radius_(other.radius_) { }

Pathway::~Pathway() { /* Nothing to do. */ }

Pathway&
Pathway::operator=(Pathway other) {
	swap(other);
	return *this; }

void
Pathway::swap(Pathway& other) {
	path_.swap(other.path_);
	std::swap(radius_, other.radius_); }

void
Pathway::movePoints( std::size_t startIndex,
															std::size_t numOfPoints,
															vec_type const newPointValues[] )
{
	path_.movePoints( startIndex, numOfPoints, newPointValues );
}

void
Pathway::setPathway( std::size_t numOfPoints,
															vec_type const points[],
															float r,
															bool closedCycle )
{
	path_.setPath( numOfPoints, points, closedCycle );
	setRadius( r );
}

void
Pathway::setRadius( float r )
{
	radius_ = r;
}

float
Pathway::radius() const
{
	return radius_;
}

bool
Pathway::isValid() const
{
	return pointCount() > 1;
}

Pathway::vec_type Pathway::mapPointToPath (const vec_type& point,
																 vec_type& tangent,
																 float& outside) const
{
	PointToPathMapping mapping;
	mapPointToPathAlike( *this, point, mapping );
	tangent = mapping.tangent;
	outside = mapping.distancePointToPath;
	return mapping.pointOnPathCenterLine;
}

Pathway::vec_type Pathway::mapPathDistanceToPoint (float pathDistance) const {
	PathDistanceToPointMapping mapping;
	mapDistanceToPathAlike(*this, pathDistance, mapping);
	return mapping.pointOnPathCenterLine;
}

float
Pathway::mapPointToPathDistance (const vec_type& point) const
{
	PointToPathDistanceMapping mapping;
	mapPointToPathAlike( *this, point, mapping );
	return mapping.distanceOnPath;
}

bool
Pathway::isCyclic() const
{
	return path_.isCyclic();
}

float
Pathway::length() const
{
	return path_.length();
}

std::size_t Pathway::pointCount() const {
	return path_.pointCount(); }

Pathway::vec_type Pathway::point(std::size_t pointIndex) const {
	return path_.point(pointIndex); }

std::size_t Pathway::segmentCount() const {
	return path_.segmentCount(); }

float
Pathway::segmentLength( std::size_t segmentIndex ) const
{
	return path_.segmentLength( segmentIndex );
}

Pathway::vec_type Pathway::segmentStart(std::size_t segmentIndex) const {
	return path_.segmentStart(segmentIndex); }

Pathway::vec_type Pathway::segmentEnd(std::size_t segmentIndex) const {
	return path_.segmentEnd(segmentIndex); }

float
Pathway::mapPointToSegmentDistance( std::size_t segmentIndex,
																		   vec_type const& point ) const
{
	return path_.mapPointToSegmentDistance( segmentIndex, point );
}

Pathway::vec_type Pathway::mapSegmentDistanceToPoint(std::size_t segmentIndex, float segmentDistance) const {
	return path_.mapSegmentDistanceToPoint(segmentIndex, segmentDistance); }

float Pathway::mapSegmentDistanceToRadius(std::size_t segmentIndex, float distanceOnSegment) const {
	UNUSED_PARAMETER(segmentIndex);
	UNUSED_PARAMETER(distanceOnSegment);
	return radius_;
}

Pathway::vec_type Pathway::mapSegmentDistanceToTangent(std::size_t segmentIndex, float segmentDistance) const {
	return path_.mapSegmentDistanceToTangent(segmentIndex, segmentDistance); }

void
Pathway::mapDistanceToSegmentPointAndTangentAndRadius( std::size_t segmentIndex,
																							  float distance,
																							  vec_type& pointOnPath,
																							  vec_type& tangent,
																							  float& radius ) const
{
	path_.mapDistanceToSegmentPointAndTangent( segmentIndex, distance, pointOnPath, tangent );
	radius = radius_;
}

void
Pathway::mapPointToSegmentDistanceAndPointAndTangentAndRadius( std::size_t segmentIndex,
																									  vec_type const& point,
																									  float& distance,
																									  vec_type& pointOnPath,
																									  vec_type& tangent,
																									  float& radius) const
{
	path_.mapPointToSegmentDistanceAndPointAndTangent( segmentIndex, point, distance, pointOnPath, tangent );
	radius = radius_;
}
