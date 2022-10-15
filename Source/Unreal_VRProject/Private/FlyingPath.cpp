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


//一番近いポイントを探す
        closestPoint = ClosestPoint(currentSegment_, searchDepth, segments, actorToMove);

//移動させる
        pointPos = closestPoint.position_;
        nextPointPos = NextPoint(segments, closestPoint).position_;

        PathMovement(DeltaTime);

//Effectsを発生する
        currentTick += DeltaTime;
        if (currentTick > spawnTime)
        {
            currentTick = 0;
            SpawnEffects(effectSpawnAndDestroyDistance, segments, closestPoint);
        }
}

/// <summary>
/// 一番近いポイントを探す
/// </summary>
PathPoint AFlyingPath::ClosestPoint(int& currentSeg, int searchD, TArray<PathSegment> seg, AActor* act)
{
    TArray<PathPoint> closestPoints = GetClosestPointsOnAreaX(currentSeg, searchD, seg, act->GetActorLocation());
    PathPoint closestP = FindClosestPoint(closestPoints, currentSeg, act->GetActorLocation());
    currentSeg = closestP.segment_;
    closestPointsToPlayer.Empty();
    return closestP;
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



/// <summary>
/// エフェクトを発生して、最初と最後の位置を計算する
/// </summary>
/// <param name="differenceFromStartPosition"></param>
/// <param name="seg"></param>
/// <param name="curPoint"></param>
void AFlyingPath::SpawnEffects(int differenceFromStartPosition, TArray<PathSegment> seg, PathPoint curPoint)
{
    startPoint = curPoint.numberInSegment_;
    startSegment = seg[curPoint.segment_].segmentNumber_;

    endPoint = curPoint.numberInSegment_;
    endSegment = seg[curPoint.segment_].segmentNumber_;

//発生する位置
if (startPoint - differenceFromStartPosition < 0)
    {
        if (seg[curPoint.segment_].segmentNumber_ == 0)
        {
            startPoint = 0;
            startSegment = 0;
        }
        else
        {
            startPoint -= differenceFromStartPosition;
            do
            {
                if (startPoint < 0)
                {
                    startSegment--;
                    startPoint = seg[startSegment].points.Num() - abs(startPoint);
                }
                else
                {
                    break;
                }

                if (startSegment < 0)
                {
                    startSegment = 0;
                    startPoint = 0;
                    break;
                }

            } while (startSegment >= 0);
        }
    }
    else
    {
        startSegment = seg[curPoint.segment_].segmentNumber_;
        startPoint -= differenceFromStartPosition;
    }
//破棄する場所
    if (curPoint.numberInSegment_ + differenceFromStartPosition > seg[curPoint.segment_].points.Num() - 1)
    {
        if (seg[curPoint.segment_].segmentNumber_ == seg.Num() - 1)
        {
            endPoint = seg[curPoint.segment_].points.Num() - 1;
            endSegment = seg.Num() - 1;
        }
        else
        {
            endPoint += differenceFromStartPosition;
            do
            {
                if (endPoint > seg[curPoint.segment_].points.Num() - 1)
                {
                    endPoint -= seg[endSegment].points.Num();
                    endSegment++;
                }
                else
                {
                    break;
                }

                if (endSegment >= seg.Num())
                {
                    endPoint = seg[curPoint.segment_].points.Num() - 1;
                    endSegment = seg.Num() - 1;
                    break;
                }

            } while (endSegment >= 0);
           
        }
    }
    else
    {
        endSegment = seg[curPoint.segment_].segmentNumber_;
        endPoint += differenceFromStartPosition;
    }

    UWorld* world = GetWorld();
    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    FRotator rotator;
    FVector spawnLocation = segments[startSegment].points[startPoint].position_;
    AActor* spawned = world->SpawnActor<AActor>(effect, spawnLocation, rotator, spawnParams);
    
}

/// <summary>
/// 次のポイントの取得
/// </summary>
/// <param name="seg"></param>
/// <param name="curPoint"></param>
/// <returns></returns>
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
/// <summary>
/// プレイヤーを前に移動させるクラス
/// </summary>
/// <param name="fixedTime"></param>
void AFlyingPath::PathMovement(float fixedTime)
{
    FVector autoMove = nextPointPos - pointPos;

    autoMove = autoMove * autoSpeed;

    FVector direction = pointPos - actorToMove->GetActorLocation() + autoMove;

    direction *= speed;

    actorToMove->SetActorLocation(FMath::Lerp(actorToMove->GetActorLocation(), actorToMove->GetActorLocation() + direction, fixedTime * 1.0f));
}


/// <summary>
/// エフェクトを前に移動指せるクラス
/// </summary>
void AFlyingPath::MoveForwardClass(int effectCurrentSegment, int& outNewSegment, int& outCurPoint ,float effectSpeed, AActor* effectToMove, float efectFixedTime)
{
    PathPoint closestP = ClosestPoint(effectCurrentSegment, 1, segments, effectToMove);

    outNewSegment = closestP.segment_;
    outCurPoint = closestP.numberInSegment_;
    FVector pointP = closestP.position_;
    FVector nextPointP = NextPoint(segments, closestP).position_;

    FVector movement = nextPointP - pointP;
    movement *= effectSpeed;

    effectToMove->SetActorLocation(FMath::Lerp(effectToMove->GetActorLocation(), effectToMove->GetActorLocation() + movement, efectFixedTime * 1.0f));
}