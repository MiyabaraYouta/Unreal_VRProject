// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathSegment.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingPath.generated.h"




UCLASS(Blueprintable)
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
		TArray<AActor*> pathCheckpoints;
	//セグメントでの最低距離を持っているポイントを探す範囲「セグメント」
	UPROPERTY(EditAnywhere)
	int searchDepth;
	UPROPERTY(EditAnywhere)
		float distanceBetweenPointsOnSegment;
	//パスに戻される力
	UPROPERTY(EditAnywhere)
		double speed;
	UPROPERTY(EditAnywhere)
		double autoSpeed;

	UPROPERTY(EditAnywhere)
		double TimeSpeed;


	UPROPERTY(EditAnywhere, Category = "移動するオブジェクト")
		bool actorIsDefault = true;

	//プレイヤのActor「必ず初期化する！！」
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!actorIsDefault"), Category = "移動するオブジェクト")
		AActor* actorToMove;

	//このフレームでキャラクターに一番近いポイント「数はsearchDepthによって違う」
	TArray<PathPoint> closestPointsToPlayer;
	//力を与えるために使う一番キャラクターに近いポイント
	PathPoint closestPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "飛ぶコース（デバッグ）")
		FVector pointPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "飛ぶコース（デバッグ）")
		FVector nextPointPos;



	int currentSegment_;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//複数セグメントの中で、セグメント毎に一番近いポイントを探す
	TArray<PathPoint> GetClosestPointsOnAreaX(int currentSegmentOnThePath, int x, TArray<PathSegment> seg, FVector playerPos);
	//一番近いポイントを探す
	PathPoint FindClosestPoint(TArray<PathPoint> points, int& currentSegment, FVector playerPos);
	PathPoint NextPoint(TArray<PathSegment> seg, PathPoint curPoint);

	void PathMovement(float fixedTime);
};
