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
    closestPointsToPlayer = GetClosestPointsOnAreaX(currentSegment_, searchDepth, segments, playerPos_->GetActorLocation());
//リーク予防
    if (closestPointsToPlayer.Num() == 0)
        return;

///ディバッグ
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Distance: %f"), closestPointsToPlayer[0].checkedDistance));
///
	
    closestPoint = FindClosestPoint(closestPointsToPlayer, currentSegment_, playerPos_->GetActorLocation());
    closestPointsToPlayer.Empty();

//パスの範囲内かどうか確認する
    if (closestPoint.checkedDistance > (pathRadius * pathRadius))
    {
        pointPos = closestPoint.position_;

        FVector direction = pointPos - playerPos_->GetActorLocation();
        direction = direction * speed * DeltaTime;

       //TODO　パスから出た場合、力を与える処理
        //playerPos_->SetActorLocation(FVector(playerPos_->GetActorLocation().X + direction.X, playerPos_->GetActorLocation().Y + direction.Y, playerPos_->GetActorLocation().Z + direction.Z));
       
        playerPos_->SetActorLocation(FMath::Lerp(playerPos_->GetActorLocation(), playerPos_->GetActorLocation() + direction, DeltaTime * speed));

        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Out of Path!"));
    }

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
