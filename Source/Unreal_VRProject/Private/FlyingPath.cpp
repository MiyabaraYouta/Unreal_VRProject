// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingPath.h"

// Sets default values
AFlyingPath::AFlyingPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

//UnityのAwakeかStartと同じ
void AFlyingPath::BeginPlay()
{
	Super::BeginPlay();

    if(actorIsDefault)
        actorToMove = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

//セグメントとポイントの初期化
    for (int i = 0; i < pathCheckpoints.Num() - 1; i++)
    {
        PathSegment tempSegment(pathCheckpoints[i]->GetActorLocation(), pathCheckpoints[i + 1]->GetActorLocation(), i);
        tempSegment.points = tempSegment.BreakTheSegment(distanceBetweenPointsOnSegment);
        segments.Add(tempSegment);
    }
    currentSegment_ = 0;
}

// Called every frame
void AFlyingPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


//プレイヤに「searchDepth」セグメントの中から一番近いポイントを探す
    closestPointsToPlayer = GetClosestPointsOnAreaX(currentSegment_, searchDepth, segments, actorToMove->GetActorLocation());
//リーク予防
    if (closestPointsToPlayer.Num() == 0)
        return;

	
    closestPoint = FindClosestPoint(closestPointsToPlayer, currentSegment_, actorToMove->GetActorLocation());
    closestPointsToPlayer.Empty();

//パスの範囲内かどうか確認する

        pointPos = closestPoint.position_;
        nextPointPos = NextPoint(segments, closestPoint).position_;

        PathMovement(DeltaTime);
}

TArray<PathPoint> AFlyingPath::GetClosestPointsOnAreaX(int currentSegmentOnThePath, int x, TArray<PathSegment> seg, FVector playerPos)
{
    TArray<PathPoint> ret;

    for (int i = -x; i <= x; i++)
    {
        if (currentSegmentOnThePath + i < 0)
            continue;

        if (currentSegmentOnThePath + i >= seg.Num())
            break;

        ret.Add(seg[currentSegmentOnThePath + i].FindClosestPointToPlayer(playerPos));
    }
    return ret;
}

PathPoint AFlyingPath::FindClosestPoint(TArray<PathPoint> points, int &currentSegment, FVector playerPos)
{
    double distance;

    PathPoint retPoint = points[0];
    distance = retPoint.checkedDistance;

    for (int i = 1; i < points.Num(); i++)
    {
        double tempDistance = points[i].checkedDistance;

        if (tempDistance < distance)
        {
            distance = tempDistance;
            retPoint = points[i];
        }
    }

    currentSegment = retPoint.segment_;
    return retPoint;
}


PathPoint AFlyingPath::NextPoint(TArray<PathSegment> seg, PathPoint curPoint)
{
    if (seg[curPoint.segment_].points.Num() == curPoint.numberInSegment_ + 1)
    {
        if (curPoint.segment_ + 1 < seg.Num())
        {
            return seg[curPoint.segment_ + 1].points[0];
        }
        else
        {
            return curPoint;
        }
    }
    return seg[curPoint.segment_].points[curPoint.numberInSegment_ + 1];
}

void AFlyingPath::PathMovement(float fixedTime)
{
    FVector autoMove = nextPointPos - pointPos;

    autoMove = autoMove * autoSpeed;

    FVector direction = pointPos - actorToMove->GetActorLocation() + autoMove;

    direction *= speed;

    actorToMove->SetActorLocation(FMath::Lerp(actorToMove->GetActorLocation(), actorToMove->GetActorLocation() + direction, fixedTime * 1.0f));
}