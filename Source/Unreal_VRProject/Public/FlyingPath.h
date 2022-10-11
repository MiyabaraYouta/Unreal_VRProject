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
	AFlyingPath();

		TArray<PathSegment> segments;

	UPROPERTY(EditAnywhere, Category = "チェックポイントの配列")

		TArray<AActor*> pathCheckpoints;

	UPROPERTY(EditAnywhere, Category = "探索の深さ、いくつセグメントで最短距離を同時に探しているか")

		int searchDepth;

	UPROPERTY(EditAnywhere, Category = "セグメントにある点の間の距離")

		float distanceBetweenPointsOnSegment;

	UPROPERTY(EditAnywhere, Category = "パスに戻させる力・速度")

		double speed;

	UPROPERTY(EditAnywhere, Category = "自動的にパスを進む速度")

		double autoSpeed;

	UPROPERTY(EditAnywhere, Category = "アルゴリズムの全体速度の管理")

		double TimeSpeed;

	UPROPERTY(EditAnywhere, Category = "使いたいオブジェクトはStartPosで発生するPawnですか？")

		bool actorIsDefault = true;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "!actorIsDefault"), Category = "StartPos発生するPawn以外のオブジェクトを設定してください")

		AActor* actorToMove;

	UPROPERTY(EditAnywhere, Category = "エフェクトの速度")

		double effectSpeed;


	//今一番近い点の位置
	FVector pointPos;
	//次の点の位置
	FVector nextPointPos;
	//このフレームでキャラクターに一番近いポイント「数はsearchDepthによって違う」
	TArray<PathPoint> closestPointsToPlayer;
	//力を与えるために使う一番キャラクターに近いポイント
	PathPoint closestPoint;
	//今飛んでいるセグメント
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


	void NextPoint(int differenceFromStartPosition, TArray<PathSegment> seg, PathPoint curPoint);
	PathPoint NextPoint(TArray<PathSegment> seg, PathPoint curPoint);

	void PathMovement(float fixedTime);
};
