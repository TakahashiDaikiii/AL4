#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Scene.h"
#include "TextureManager.h"

class TitleScene {
public:
	void Initialize();

	void Update();

	void Draw();

	bool IsSceneEnd() { return isSceneEnd; }

	Scene NextScene() { return Scene::GAME; }

	void Reset();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	WorldTransform worldTransform_;

	// サウンド
	uint32_t bgmHandle_;
	uint32_t playBGM_;
	bool isBGM_;

	uint32_t buttonSound_;

	// シーンを終わらせるフラグ
	bool isSceneEnd = false;
	uint32_t titleTexHandle_ = 0;
	Sprite* titleSprite_ = nullptr;
	Sprite* titleLetterSprite_ = nullptr;

	Vector2 move_ = {5.0f, 5.0f};

	bool fadeFlag_ = false;

};
