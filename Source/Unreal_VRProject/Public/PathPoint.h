// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREAL_VRPROJECT_API PathPoint
{
	FVector position_;
public:
	double checkedDistance;
	int segment_;
	PathPoint(FVector position, int segment);
	PathPoint();
	~PathPoint();

	FVector GetPosition() const;
	double   GetDistanceToCoordinate(FVector);
};
