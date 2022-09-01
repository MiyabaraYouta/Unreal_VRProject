// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPoint.h"

//コンストラクタ
PathPoint::PathPoint(FVector position, int segment): position_(position), segment_(segment)
{
}
//デストラクタ
PathPoint::~PathPoint()
{
}
//距離の関数
double PathPoint::GetDistanceToCoordinate(FVector coor)
{
	return (coor.X - position_.X) * (coor.X - position_.X) + (coor.Y - position_.Y) * (coor.Y - position_.Y) + (coor.Z - position_.Z) * (coor.Z - position_.Z);
}
