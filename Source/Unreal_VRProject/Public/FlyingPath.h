// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathSegment.h"
#include "FlyingPath.generated.h"


UCLASS()
class UNREAL_VRPROJECT_API AFlyingPath : public AActor
{
	GENERATED_BODY()
	

public:	
	//コンストラクタ
	AFlyingPath();
	//セグメント
	TArray<PathSegment> segments;
	//チェックポイントの配列
	UPROPERTY(EditAnywhere)
		TArray<FVector> pathCheckpoints;
	//セグメントでの最低距離を持っているポイントを探す範囲「セグメント」
	UPROPERTY(EditAnywhere)
	int searchDepth;
	//プレイヤのActor「必ず初期化する！！」
	UPROPERTY(EditAnywhere)
		AActor* playerPos_;
	//パスの半径
	UPROPERTY(EditAnywhere)
		double pathRadius;
	//このフレームでキャラクターに一番近いポイント「数はsearchDepthによって違う」
	TArray<PathPoint> closestPointsToPlayer;
	//力を与えるために使う一番キャラクターに近いポイント
	PathPoint closestPoint;
	//最適のために使ている
	int currentSegment_;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//一番近いポイントを探す
	PathPoint FindClosestPoint(TArray<PathPoint> points, int& currentSegment, FVector playerPos);
	//複数セグメントの中で、セグメント毎に一番近いポイントを探す
	TArray<PathPoint> GetClosestPointsOnAreaX(int currentSegmentOnThePath, int x, TArray<PathSegment> seg, FVector playerPos);
};
