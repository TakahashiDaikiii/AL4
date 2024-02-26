#include "Item.h"
#include <cassert>

void Item::Initialize(Model* model,Vector3 position) {
	
	// NULLポインタチェック
	assert(model);

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数からデータを受け取る
	model_ = model;
	// ポジション
	worldTransform_.translation_ = position;

	//worldTransform_.scale_ = {1, 1, 1};

	worldTransform_.UpdateMatrix();

}
void Item::Update() { worldTransform_.UpdateMatrix(); }

void Item::Draw(ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection); }

void Item::OnCollision() { isDead_ = true; }

Vector3 Item::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}




