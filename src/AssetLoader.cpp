#include "AssetLoader.h"
#include "Textures.h"
#include "Engine.h"

AssetLoader::AssetLoader()
{
}

AssetLoader::~AssetLoader()
{
}

bool AssetLoader::Start()
{
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Player/ShovelKnight_Atlas2.png", "ShovelKnightTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Enemies/Beeto.png", "BettoTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Enemies/Divedrake.png", "DivedrakeTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Objects/CheckPoint.png", "CheckpointTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Objects/DirtBlock_Small.png", "SmallDirtBlockTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Objects/DirtBlock_Big.png", "BigDirtBlockTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Objects/Gold.png","GoldCoinTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Objects/HealthPotion.png", "HealthPotionTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Objects/Moving_Platforms.png", "MovingPlatformTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/PathTexture.png", "PathfindingTexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/UI.png", "UITexture");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Font.png", "FontTexture");

	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Parallax/Cloud_Parallax.png", "CloudsParallax1");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Parallax/Castle_Parallax.png", "CastleParallax1");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Parallax/Trees_1_Parallax.png", "Trees1Parallax1");
	Engine::GetInstance().textures->CreateTexture("Assets/Textures/Parallax/Trees_2_Parallax.png", "Trees2Parallax1");

	return true;
}

bool AssetLoader::CleanUp()
{
	return true;
}
