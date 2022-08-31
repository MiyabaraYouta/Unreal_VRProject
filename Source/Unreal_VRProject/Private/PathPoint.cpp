// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPoint.h"

PathPoint::PathPoint(FVector position, int segment): position_(position), segment_(segment)
{
}

PathPoint::~PathPoint()
{

}PathPoint::PathPoint()
{

}

FVector PathPoint::GetPosition() const
{
	return position_;
}
double PathPoint::GetDistanceToCoordinate(FVector coor)
{
	return (coor.X - position_.X) * (coor.X - position_.X) + (coor.Y - position_.Y) * (coor.Y - position_.Y) + (coor.Z - position_.Z) * (coor.Z - position_.Z);
}