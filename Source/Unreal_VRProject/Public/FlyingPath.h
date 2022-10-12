// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathSegment.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingPath.generated.h"


UCLASS()

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

UPROPERTY(EditAnywhere, Category = "エフェクト・プレイヤーを中心にして、移動する距離")

		int effectSpawnAndDestroyDistance;

UPROPERTY(EditAnywhere, Category = "エフェクト・発生する頻度")
		float spawnTime;

		
UPROPERTY(EditAnywhere, Category = "エフェクト")
		TSubclassOf<AActor> effect;
		 

//ブループリントで使う変数「エフェクト」
UPROPERTY(BlueprintReadOnly, Category = "Variable|Blueprint")
		int startPoint = 0;
UPROPERTY(BlueprintReadOnly, Category = "Variable|Blueprint")
		int startSegment = 0;
UPROPERTY(BlueprintReadOnly, Category = "Variable|Blueprint")
		int endPoint = 0;
UPROPERTY(BlueprintReadOnly, Category = "Variable|Blueprint")
		int endSegment = 0;


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

	virtual void Tick(float DeltaTime) override;

	TArray<PathPoint> GetClosestPointsOnAreaX(int currentSegmentOnThePath, int x, TArray<PathSegment> seg, FVector playerPos);

	PathPoint FindClosestPoint(TArray<PathPoint> points, int& currentSegment, FVector playerPos);

	void SpawnEffects(int differenceFromStartPosition, TArray<PathSegment> seg, PathPoint curPoint);

	PathPoint NextPoint(TArray<PathSegment> seg, PathPoint curPoint);

	void PathMovement(float fixedTime);

	//エフェクトのブループリントで設定する
	UFUNCTION(BlueprintCallable)
		void MoveForwardClass(int effectCurrentSegment, int& outNewSegment, int& outCurPoint, float effectSpeed, AActor* effectToMove, float efectFixedTime);

	PathPoint ClosestPoint(int& curSeg, int searchD, TArray<PathSegment> seg, AActor* actor);

private:
	float currentTick = 0;

};
