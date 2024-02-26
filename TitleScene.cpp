#include "TitleScene.h"
void TitleScene::Initialize() {

	worldTransform_.Initialize();
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 背景のスプライト
	titleTexHandle_ = TextureManager::Load("title.png");
	titleSprite_ =
	    Sprite::Create(titleTexHandle_, {640, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	uint32_t titleLettrTexHandle_ = TextureManager::Load("titleLetter.png");
	titleLetterSprite_ =
	    Sprite::Create(titleLettrTexHandle_, {-1, 360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void TitleScene::Update() {

	Vector2 position_ = titleLetterSprite_->GetPosition();

	position_.x += move_.x;
	position_.y += move_.y;

	if (position_.x <= 0) {
		move_.x = 5.0f;
	}

	if (position_.x >= 1280) {
		move_.x = -5.0f;
	}

	if (position_.y <= 0) {
		move_.y = 5.0f;
	}

	if (position_.y >= 720) {
		move_.y = -5.0f;
	}

	titleLetterSprite_->SetPosition(position_);

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
			Sleep(2 * 100);
			isSceneEnd = true;
		}
	}	
}

void TitleScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	titleSprite_->Draw();
	titleLetterSprite_->Draw();

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

void TitleScene::Reset() {
	isSceneEnd = false;
	fadeFlag_ = false;
	titleLetterSprite_->SetPosition({640,360});
}
