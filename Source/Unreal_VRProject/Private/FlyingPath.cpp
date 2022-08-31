// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingPath.h"

// Sets default values
AFlyingPath::AFlyingPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFlyingPath::BeginPlay()
{
	Super::BeginPlay();
    for (int i = 0; i < pathCheckpoints.Num() - 1; i++)
    {
        PathSegment tempSegment(pathCheckpoints[i], pathCheckpoints[i + 1], i);
        tempSegment.points = tempSegment.BreakTheSegment(5);
        segments.Add(tempSegment);
    }
    currentSegment_ = 0;
}

// Called every frame
void AFlyingPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    closestPointsToPlayer = GetClosestPointsOnAreaX(currentSegment_, searchDepth, segments, playerPos_->GetActorLocation());

   
 /*   closestPoint = FindClosestPoint(closestPointsToPlayer, currentSegment_, playerPos_->GetActorLocation());

    if (closestPoint.checkedDistance > (pathRadius * pathRadius))
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Out of Path!"));
    }*/

}

TArray<PathPoint> AFlyingPath::GetClosestPointsOnAreaX(int currentSegmentOnThePath, int x, TArray<PathSegment> seg, FVector playerPos)
{
    TArray<PathPoint> ret;

    for (int i = -x; i < x; i++)
    {
        if (currentSegmentOnThePath + i < 0 || currentSegmentOnThePath + i >= seg.Num())
            continue;

        ret.Add(seg[currentSegmentOnThePath + i].FindClosestPointToPlayer(playerPos));
    }
    return ret;
}

PathPoint AFlyingPath::FindClosestPoint(TArray<PathPoint> points, int &currentSegment, FVector playerPos)
{
    double distance;

    PathPoint retPoint = points[0];

    distance = points[0].GetDistanceToCoordinate(playerPos);

    for (int i = 1; i < points.Num(); i++)
    {
        double tempDistance = points[i].GetDistanceToCoordinate(playerPos);

        if (tempDistance < distance)
        {
            distance = tempDistance;
            retPoint = points[i];
        }
    }

    currentSegment = retPoint.segment_;
    return retPoint;
}