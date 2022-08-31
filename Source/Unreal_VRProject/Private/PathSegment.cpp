// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSegment.h"

PathSegment::PathSegment(FVector startPos, FVector endPos, int segmentNum = 0) : startPos_(startPos), endPos_(endPos), segmentNumber_(segmentNum)
{
}

PathSegment::~PathSegment()
{
}

TArray<PathPoint> PathSegment::BreakTheSegment(int iterations)
{
    TArray<PathPoint> retPoints;
    TArray<PathSegment> brokenSegments;
    brokenSegments.Add(PathSegment(startPos_, endPos_));

    for (int i = 0; i < iterations; i++)
    {
        TArray<PathSegment> newlyBrokenSegments;
        for(PathSegment seg : brokenSegments)
        {
            //中心を探す
            FVector center = (seg.startPos_ + seg.endPos_) * 0.5f;
            //左の部分
            PathSegment temp = PathSegment(seg.startPos_, center);
            newlyBrokenSegments.Add(temp);
            //右の部分
            temp = PathSegment(center, seg.endPos_);
            newlyBrokenSegments.Add(temp);
        }
        brokenSegments.Empty();
        brokenSegments = newlyBrokenSegments;
    }

    for (int i = 0; i < brokenSegments.Num(); i++)
    {
        retPoints.Add(PathPoint(brokenSegments[i].startPos_, segmentNumber_));
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Position: %f-%f-%f"), brokenSegments[i].startPos_.X, brokenSegments[i].startPos_.Y, brokenSegments[i].startPos_.Z));
    }
    return retPoints;
}

PathPoint PathSegment::FindClosestPointToPlayer(FVector pos)
{
    double distance;
    PathPoint retPoint = points[0];
    distance = retPoint.GetDistanceToCoordinate(pos);

    for (int i = 1; i < points.Num(); i++)
    {
        float tempDistance = points[i].GetDistanceToCoordinate(pos);

        if (tempDistance < distance)
        {
            distance = tempDistance;
            retPoint = points[i];
            retPoint.checkedDistance = distance;
        }
    }
    return retPoint;
}
