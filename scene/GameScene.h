#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include"Ground.h"
#include"FollowCamera.h"
#include<memory>

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	/// 
	
	uint32_t textureHandle_ = 0;

	std::unique_ptr<Player> player_;

	// Player* player_ = nullptr;

	std::unique_ptr<Model> model_;

	ViewProjection viewProjection_;

	WorldTransform worldTransform_;

	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Ground> ground_;

	Model* modelSkydome_ = nullptr;

	Model* modelground_ = nullptr;

	std::unique_ptr<FollowCamera> followcamera_;

	// デバッグカメラ有効

	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;
};
