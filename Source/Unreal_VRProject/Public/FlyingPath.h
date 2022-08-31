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
	// Sets default values for this actor's properties
	AFlyingPath();
	TArray<PathSegment> segments;

	UPROPERTY(EditAnywhere)
		TArray<FVector> pathCheckpoints;

	TArray<PathPoint> closestPointsToPlayer;
	
	PathPoint closestPoint;

	int currentSegment_;

	UPROPERTY(EditAnywhere)
	int searchDepth;

	UPROPERTY(EditAnywhere)
		AActor* playerPos_;

	UPROPERTY(EditAnywhere)
		double pathRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	PathPoint FindClosestPoint(TArray<PathPoint> points, int& currentSegment, FVector playerPos);
	TArray<PathPoint> GetClosestPointsOnAreaX(int currentSegmentOnThePath, int x, TArray<PathSegment> seg, FVector playerPos);
};
