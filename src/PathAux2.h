//
//  PathAux2.h
//  MarXsCube
//
//  Created by SeconDatke on 2/19/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef __MarXsCube__PathAux2__
#define __MarXsCube__PathAux2__

#include "Path.h"
#include "PathAux.h"
#include "PathUtils.h"
#include <limits>

template<class PathAlike, class Mapping, class BaseDataExtractionPolicy = PointToPathAlikeBaseDataExtractionPolicy< PathAlike >>
class PointToPathAlikeMapping {
public:

		static void map(PathAlike const& pathAlike, glm::vec3 const& queryPoint, Mapping& mapping) {
			float minDistancePointToPath = std::numeric_limits< float >::max();
			mapping.setDistanceOnPathFlag( 0.0f );
			
			std::size_t const segmentCount = pathAlike.segmentCount();
			for (std::size_t segmentIndex = 0; segmentIndex < segmentCount; ++segmentIndex) {
				
				float segmentDistance = 0.0f;
				float radius = 0.0f;
				float distancePointToPath = 0.0f;
				glm::vec3 pointOnPathCenterLine( 0.0f, 0.0f, 0.0f );
				glm::vec3 tangent( 0.0f, 0.0f, 0.0f );
				
				BaseDataExtractionPolicy::extract( pathAlike, segmentIndex, queryPoint, segmentDistance, radius, distancePointToPath, pointOnPathCenterLine, tangent );
				
				if ( distancePointToPath < minDistancePointToPath ) {
					minDistancePointToPath = distancePointToPath;
					mapping.setPointOnPathCenterLine(pointOnPathCenterLine);
					mapping.setPointOnPathBoundary(pointOnPathCenterLine + (glm::normalize(queryPoint - pointOnPathCenterLine) * radius));
					mapping.setRadius( radius );
					mapping.setTangent( tangent );
					mapping.setSegmentIndex( segmentIndex );
					mapping.setDistancePointToPath( distancePointToPath );
					mapping.setDistancePointToPathCenterLine( distancePointToPath + radius );
					mapping.setDistanceOnPath( mapping.distanceOnPathFlag() + segmentDistance );
					mapping.setDistanceOnSegment( segmentDistance );
				}
				
				mapping.setDistanceOnPathFlag( mapping.distanceOnPathFlag() + pathAlike.segmentLength( segmentIndex ) );
			}
		}
		
};
	
template< class PathAlike, class Mapping >
void mapPointToPathAlike( PathAlike const& pathAlike, glm::vec3 const& point, Mapping& mapping ) {
		PointToPathAlikeMapping< PathAlike, Mapping >::map( pathAlike, point, mapping ); }

	template< class PathAlike, class Mapping, class BaseDataExtractionPolicy = DistanceToPathAlikeBaseDataExtractionPolicy< PathAlike > >
	class DistanceToPathAlikeMapping {
	public:
		
		static void map( PathAlike const& pathAlike, float distanceOnPath, Mapping& mapping ) {
			float const pathLength = pathAlike.length();
			
			// Modify @c distanceOnPath to applicable values.
			if ( pathAlike.isCyclic() ) {
				distanceOnPath = modulo( distanceOnPath, pathLength ); }
			distanceOnPath = clamp( distanceOnPath, 0.0f, pathLength );
			
			// Which path alike segment is reached by @c distanceOnPath?
			float remainingDistance = distanceOnPath;
			std::size_t segmentIndex = 0;
			std::size_t const maxSegmentIndex = pathAlike.segmentCount() - 1;
			while( ( segmentIndex < maxSegmentIndex ) &&
				  ( remainingDistance > pathAlike.segmentLength( segmentIndex ) ) ) {
				remainingDistance -= pathAlike.segmentLength( segmentIndex );
				++segmentIndex;
			}
			
			// Extract the path related data associated with the segment reached
			// by @c distanceOnPath.
			glm::vec3 pointOnPathCenterLine( 0.0f, 0.0f, 0.0f );
			glm::vec3 tangent( 0.0f, 0.0f, 0.0f );
			float radius = 0.0f;
			BaseDataExtractionPolicy::extract( pathAlike, segmentIndex, remainingDistance, pointOnPathCenterLine, tangent, radius );
			
			// Store the extracted data in @c mapping to return it to the caller.
			mapping.setPointOnPathCenterLine( pointOnPathCenterLine );
			mapping.setRadius( radius );
			mapping.setTangent( tangent );
			mapping.setSegmentIndex( segmentIndex );
			mapping.setDistanceOnPath( distanceOnPath );
			mapping.setDistanceOnSegment( remainingDistance );
		}
		
	};
	
template< class PathAlike, class Mapping >
void mapDistanceToPathAlike(PathAlike const& pathAlike, float distance, Mapping& mapping) {
	DistanceToPathAlikeMapping<PathAlike, Mapping>::map(pathAlike, distance, mapping); }

#endif /* defined(__MarXsCube__PathAux2__) */
