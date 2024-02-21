#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

    GameScene::GameScene() {
}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("white1x1.png");
	// スプライトの生成
	// sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// ビューポートプロジェクションの初期化
	viewProjection_.Initialize();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参考するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	// 自機の体の3Dモデルの生成
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	// 自機の頭の3Dモデルの生成
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	// 自機の左腕の3Dモデルの生成
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	// 自機の右腕の3Dモデルの生成
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	// 天球の3Dモデル生成
	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	// 地面の3Dモデル生成
	modelGround_.reset(Model::CreateFromOBJ("ground", true));

	// 自キャラの生成と初期化処理
	player_ = std::make_unique<Player>();
	player_->Initialize(
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get());

	// 天球の生成と初期化処理
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	// 地面の生成と初期化処理
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

	// 追従カメラの生成と初期化処理
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	// 自キャラのワールドトランスフォームを追従カメラのセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// 自キャラのビュープロジェクションに追従カメラのビュープロジェクションをセット
	player_->SetViewProjection(&followCamera_->GetViewProjection());
}

void GameScene::Update() {

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_1)) {
		isDebugCameraActive_ = 1;
	} else if (input_->TriggerKey(DIK_1) && isDebugCameraActive_ == 1) {
		isDebugCameraActive_ = 0;
	}

#endif
	// カメラの処理
	if (isDebugCameraActive_ == 1) {
		// デバックカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	// 自キャラの更新
	player_->Update();

	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	// 追従カメラの更新
	followCamera_->Update();

	// ビュープロジェクションの反映
	viewProjection_.matView = followCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;

	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3D描画
	// model_->Draw(worldTransform_,viewProjection_,textureHandle_);
	// デバッグカメラの描画
	// modelPlayer_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// プレイヤーの描画
	player_->Draw(viewProjection_);
	// 天球の描画
	skydome_->Draw(viewProjection_);
	// 地面の描画
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}