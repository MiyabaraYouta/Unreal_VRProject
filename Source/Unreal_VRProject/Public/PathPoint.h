// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * ポイントのクラス
 */
class UNREAL_VRPROJECT_API PathPoint
{
public:
	//ポイントの位置
	FVector position_;
	//「最適化」一回確認した距離をこちらに保存する
	double checkedDistance;
	//パスの中でこのポイントはどのセグメントに所属するか
	int segment_;
	//コンストラクタ「位置、セグメント」
	PathPoint(FVector position, int segment);
	PathPoint();
	~PathPoint();
	//ポイントからある位置までの距離
	double   GetDistanceToCoordinate(FVector);
};
