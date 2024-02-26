#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class BaseCharacter {

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Updata();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_;}

protected:

	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロダクション
	ViewProjection viewProjection_;

};
