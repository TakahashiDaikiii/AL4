#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Item{
public:
	void Initialize(Model* model,Vector3 position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	bool IsDead() const { return isDead_; }

	// getter
	float GetRadius() { return radius_; }

private:

	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// デスフラグ
	bool isDead_ = false;

	// 半径
	float radius_ = 1;

};
