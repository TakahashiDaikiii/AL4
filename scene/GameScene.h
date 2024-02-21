#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
	// スプライト
	// Sprite* sprite_ = nullptr;
	// 自キャラの3Dモデル
	std::unique_ptr<Model> modelPlayer_;
	// 天球の3Dモデル
	std::unique_ptr<Model> modelSkydome_;
	// 地面の3Dモデル
	std::unique_ptr<Model> modelGround_;
	// 自キャラの3Dモデル
	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ビュープロダクション
	ViewProjection viewProjection_;
	// 自キャラ
	std::unique_ptr<Player> player_;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// 天球
	std::unique_ptr<Skydome> skydome_;
	// 地面
	std::unique_ptr<Ground> ground_;
	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;
};