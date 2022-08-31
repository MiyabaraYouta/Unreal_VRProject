// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathPoint.h"

/**
 * 
 */
class UNREAL_VRPROJECT_API PathSegment
{
	FVector startPos_;
	FVector endPos_;
public:
	int segmentNumber_;
	TArray<PathPoint> points;

	PathSegment(FVector, FVector, int);
	~PathSegment();
	TArray<PathPoint> BreakTheSegment(int);
	PathPoint		 FindClosestPointToPlayer(FVector);
private:

};
