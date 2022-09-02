// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathPoint.h"

/**
 * 	
 */
class UNREAL_VRPROJECT_API PathSegment
{
	//セグメントの頂点
	FVector startPos_;
	FVector endPos_;
public:
	//パスでの順
	int segmentNumber_;
	//このセグメントのポイント
	TArray<PathPoint> points;
	//コンストラクタ
	PathSegment(FVector, FVector, int);
	~PathSegment();
	//ポイントの初期化
	TArray<PathPoint> BreakTheSegment(float);
	//このセグメントである位置に一番近いポイント
	PathPoint		 FindClosestPointToPlayer(FVector);
private:

};
