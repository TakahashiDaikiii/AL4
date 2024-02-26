#include "Player.h"
#include "Easings.h"
#include "ImGuiManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	/*assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);*/

	input_ = Input::GetInstance();

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	/*for (int i = 0; i < 5; i++) {
	    worldTransform_[i].parent_ = &worldTransform_[]
	}*/
	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 自機(体の部位)のworldTransfotm初期化
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformHammer_.Initialize();

	// 自機の頭・両腕の初期位置
	worldTransformHead_.translation_ = {0.0f, 2.0f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.5f, 1.3f, 0.0f};
	worldTransformR_arm_.translation_ = {0.5f, 1.0f, 0.0f};

	worldTransformL_arm_.rotation_ = {-0.2f, 0.0f, 0.0f};

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformBody_;

	// 浮遊ギッミク
	InitializeFloatingGimmick();
}

void Player::Update() {

	// リクエストがあったら初期化と次の行動に以降
	if (behaviorRequest_) {
		// ふるまいを変更する
		behavior_ = behaviorRequest_.value();
		// ふるまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:

			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:

			BehaviorAttackInitialize();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
		// 通常攻撃
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
		// 通常攻撃
	case Behavior::kAttack:
		break;
	}

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	// 体の部位を行列を定数バッファに転送
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

	if (behavior_ == Behavior::kAttack) {
		models_[kModelHammer]->Draw(worldTransformHammer_, viewProjection);
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

const WorldTransform& Player::GetWorldTransform() {
	// TODO: return ステートメントをここに挿入します
	return worldTransform_;
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFlotingGimmick() {

	// 浮遊移動のサイクル<frame>
	const uint16_t period = 120;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたらθに戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);

	// 浮遊の振幅<m>
	const float floatingAmplitube = 0.1f;
	// 浮遊を座標に反映
	worldTransform_.translation_.y += std::sin(floatingParameter_) * floatingAmplitube;

}

void Player::UpdataArmAnimation() {

	// 浮遊移動のサイクル<frame>
	const uint16_t period = 120;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * 3.14f / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;
	// 2πを超えたらθに戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * 3.14f);

	// 浮遊の振幅<m>
	const float floatingAmplitube = 0.02f;
	// 浮遊を座標に反映(左)
	worldTransformL_arm_.rotation_.x += std::sin(floatingParameter_) * floatingAmplitube;
	// 浮遊を座標に反映(右)
	worldTransformR_arm_.rotation_.x += std::sin(floatingParameter_) * floatingAmplitube;
}

void Player::BehaviorRootInitialize() {

	worldTransformL_arm_.rotation_ = {-0.2f, 0.0f, 0.0f};
	worldTransformR_arm_.rotation_ = {-0.2f, 0.0f, 0.0f};
}

void Player::BehaviorAttackInitialize() {}

void Player::BehaviorRootUpdate() {

	UpdateFlotingGimmick();

	UpdataArmAnimation();

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	// ゲームパッド状態取得、ゲームパッドが有効の場合if文が通る
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, // Lスティックの横成分
		    0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed, // Lスティックの縦成分
		};

		Matrix4x4 rotationXMatrix = MakeRotateXmatrix(viewProjection_->rotation_.x);
		Matrix4x4 rotationYMatrix = MakeRotateYmatrix(viewProjection_->rotation_.y);
		Matrix4x4 rotationZMatrix = MakeRotateZmatrix(viewProjection_->rotation_.z);
		Matrix4x4 rotationXYZMatrix =
		    Multiply(rotationXMatrix, Multiply(rotationYMatrix, rotationZMatrix));

		// 移動量に速さを反映
		move = Multiply(speed, Normalize(move));

		// 移動量に速さを反映(θ度の移動ベクトル)
		// rotation = (viewProjection_->rotation_.y);

		move = Transform(move, rotationXYZMatrix);

		if (move.y != 0 || move.z != 0) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}
}

void Player::ResetPosition() {
	// 自機の頭・両腕の初期位置
	worldTransformHead_.translation_ = {0.0f, 2.0f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.5f, 2.0f, 0.0f};
	worldTransformR_arm_.translation_ = {0.5f, 1.0f, 0.0f};

	worldTransformL_arm_.rotation_ = {-0.2f, 0.0f, 0.0f};

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};

	floatingParameter_ = 0;

	worldTransform_.UpdateMatrix();
}