#pragma once
#include "BaseCharacter.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "MyMath.h"

#include <cassert>
#include <optional>

enum class Behavior {
	kRoot,   // 通常状態
	kAttack, // 攻撃中
};

class Player : public BaseCharacter {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	///  ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();
	const WorldTransform& GetWorldTransform();

	void SetViewProjection(const ViewProjection* viewProjection) 
	{
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

	/// <summary>
	/// 通常行動初期化
	/// </summary>
	void BehaviorRootInitialize();

	/// <summary>
	/// 攻撃行動初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 通常行動更新
	/// </summary>
	void BehaviorRootUpdate();

	/// <summary>
	/// 攻撃行動更新
	/// </summary>

	Behavior behavior_ = Behavior::kRoot;

	// getter

	float GetRadius() { return radius_; }

	void ResetPosition();

private:
	Input* input_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	enum PlayerNum {
		kModelIndexBody,
		kModelIndexHead,
		kModelIndexL_arm,
		kModelIndexR_arm,
		kModelHammer
	};

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 浮遊ギミックの媒介変数
	float floatingParameter_ = 0.0f;

	// カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	// 攻撃時間
	float attackTime_ = 0;

	// 攻撃時間Max時間
	const float attackTimeMax_ = 120;

	// フレーム
	float frameEnd_ = 120;

	// 半径
	float radius_ = 1;
};