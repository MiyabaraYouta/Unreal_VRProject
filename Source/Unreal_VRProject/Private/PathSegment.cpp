// Fill out your copyright notice in the Description page of Project Settings.


#include "PathSegment.h"

///
PathSegment::PathSegment(FVector startPos, FVector endPos, int segmentNum = 0) : startPos_(startPos), endPos_(endPos), segmentNumber_(segmentNum)
{
}

PathSegment::~PathSegment()
{
}
///


TArray<PathPoint> PathSegment::BreakTheSegment(float distanceBetweenPoints)
{
    //戻り値
    TArray<PathPoint> retPoints;
    //分けたセグメント
    TArray<PathSegment> brokenSegments;
    brokenSegments.Add(PathSegment(startPos_, endPos_));

    do
    {
        //新しく作ったセグメント
        TArray<PathSegment> newlyBrokenSegments;
        //ループでセグメントを分ける
        for(PathSegment seg : brokenSegments)
        {
            //中心を探す
            FVector center = (seg.startPos_ + seg.endPos_) * 0.5f;
            //左の部分
            PathSegment temp = PathSegment(seg.startPos_, center);
            newlyBrokenSegments.Add(temp);
            //右の部分
            temp = PathSegment(center, seg.endPos_);
            //作ったセグメントを保存
            newlyBrokenSegments.Add(temp);
        }
        //使っているセグメントのアップデート
        brokenSegments.Empty();
        brokenSegments = newlyBrokenSegments;
    } while (FVector::DistSquared(brokenSegments[0].startPos_, brokenSegments[0].endPos_) > distanceBetweenPoints* distanceBetweenPoints);

//順を追ってポイントの保存
    for (int i = 0; i < brokenSegments.Num(); i++)
    {
        retPoints.Add(PathPoint(brokenSegments[i].startPos_, segmentNumber_, i));
    }
    return retPoints;
}

PathPoint PathSegment::FindClosestPointToPlayer(FVector pos)
{
    //戻り値
    double distance;
    //最初ポイントを基準にすること
    PathPoint retPoint = points[0];
    distance = retPoint.GetDistanceToCoordinate(pos);
    retPoint.checkedDistance = distance;
    //最低距離が持っているポイントの検索
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
