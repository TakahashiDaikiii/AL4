#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include"ViewProjection.h"
#include <cassert>
class Player {
public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();
	const WorldTransform& GetWorldTransform();

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFlotingGimmick();

	void UpdataArmAnimation();

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

private:
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* modelFighterBody_;
	Model* modelFighterHead_;
	Model* modelFighterL_arm_;
	Model* modelFighterR_arm_;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
};