#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include<fstream>

    GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
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

	uint32_t fadeTexHandle = TextureManager::Load("fade.png");
	fadeSprite_ = Sprite::Create(fadeTexHandle, {0, 0});

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

	modelItem_.reset(Model::CreateFromOBJ("Item", true));

	// 自キャラモデルまとめ
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	    modelFighterR_arm_.get()};


	// 自キャラの生成と初期化処理
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);

	

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

	LoadItemPopData();

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

	if (fadeColor_.w >= 0 && count_ == 0) {
		fadeColor_.w -= 0.005f;
		fadeSprite_->SetColor(fadeColor_);
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


	
	if (fadeColor_.w <= 0) 
	{
		CheckAllCollision();
	}


	UpdataItemPopCommands();

	if (count_ >= 3) {

		fadeColor_.w += 0.005f;
		fadeSprite_->SetColor(fadeColor_);
		if (fadeColor_.w >= 1.0f) {
			isSceneEnd = true;
		}
	}


	// ビュープロジェクション行列の転送
	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// プレイヤーの描画
	player_->Draw(viewProjection_);
	// 天球の描画
	skydome_->Draw(viewProjection_);
	// 地面の描画
	ground_->Draw(viewProjection_);

	

	for (const std::unique_ptr<Item>& item : items_) {
		item->Draw(viewProjection_);
	}

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

void GameScene::Reset() {

	isSceneEnd = false;
	count_ = 0;

	player_->ResetPosition();

	fadeColor_.w = 1.0f;
	fadeSprite_->SetColor(fadeColor_);

	// アイテムのCSVファイル読み込み
	LoadItemPopData();
}

void GameScene::CheckAllCollision() {

	Vector3 posA, posB;

	for (const std::unique_ptr<Item>& item : items_) {

		posA = player_->GetWorldPosition();

		posB = item->GetWorldPosition();

		float Hit = (posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y) +
		            (posA.z - posB.z) * (posA.z - posB.z);

		float Radius = (player_->GetRadiusHammer() + item->GetRadius()) *
		               (player_->GetRadiusHammer() + item->GetRadius());

		if (Hit <= Radius) {
			item->OnCollision();
			count_++;
		}
	}
}

void GameScene::LoadItemPopData() {
	itemPopCommands.clear();
	std::ifstream file;
	file.open("Resources/ItemPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	itemPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdataItemPopCommands() {
	std::string line;

	// コマンド実行ループ
	while (getline(itemPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;
		// 　,区切りで行の先頭文字列を所得

		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			ItemGenerate({x, y, z});
		}
	}
}

void GameScene::ItemGenerate(Vector3 position) {
	// アイテムの生成と初期化処理
	Item* item = new Item();
	item->Initialize(modelItem_.get(), position);

	items_.push_back(static_cast<std::unique_ptr<Item>>(item));
}