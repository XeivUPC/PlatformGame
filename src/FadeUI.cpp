#include "FadeUI.h"
#include "Engine.h"
#include "Render.h"

FadeUI::FadeUI(Module* ModuleAt) : UI(ModuleAt)
{
}

FadeUI::~FadeUI()
{
}

void FadeUI::Update(float dt)
{
	FadeUpdate();
	Render();
}

void FadeUI::Render()
{
	FadeRender();
}

void FadeUI::ForceRender()
{
	int opacity;

	opacity = (int)(255 * (fadeTimer.ReadMSec() / fadeTime));
	if (opacity > 255)
		opacity = 255;

	SDL_Color color = { fadeColor.r,fadeColor.g,fadeColor.b,opacity };
	Engine::GetInstance().render->LockLayer(Render::Layer6);
	SDL_Rect rect = { 0,0,427,239 };
	Engine::GetInstance().render->DrawRectangle(rect, color.r, color.g, color.b, color.a, true, false);
	Engine::GetInstance().render->UnlockLayer();
}

void FadeUI::CleanUp()
{
}

void FadeUI::StartFadeIn(SDL_Color color, float time)
{
	if (doingFadeIn)
		return;
	fadeColor = color;
	fadeTime = time;
	doingFadeIn = true;
	doingFadeOut = false;
	fadeTimer.Start();
	FadeIn();
}

void FadeUI::StartFadeOut(SDL_Color color, float time)
{
	if (doingFadeOut)
		return;
	fadeColor = color;
	fadeTime = time;
	doingFadeOut = true;
	doingFadeIn = false;
	fadeTimer.Start();
	FadeOut();
}

bool FadeUI::IsFading()
{
	return doingFadeIn || doingFadeOut;
}

void FadeUI::FadeUpdate()
{
	if (doingFadeIn)
		FadeIn();
	if (doingFadeOut)
		FadeOut();
}

void FadeUI::FadeRender()
{
	if (doingFadeIn || doingFadeOut) {
		int opacity;

		if (doingFadeIn) {
			opacity = (int)(255 * (fadeTimer.ReadMSec() / fadeTime));
			if (opacity > 255)
				opacity = 255;
		}
		else {
			opacity = 255 - (int)(255 * (fadeTimer.ReadMSec() / fadeTime));
			if (opacity < 0)
				opacity = 0;
		}

		SDL_Color color = { fadeColor.r,fadeColor.g,fadeColor.b,opacity };
		Engine::GetInstance().render->LockLayer(Render::Layer6);
		SDL_Rect rect = { 0,0,427,239 };
		Engine::GetInstance().render->DrawRectangle(rect, color.r, color.g, color.b, color.a,true, false);
		Engine::GetInstance().render->UnlockLayer();
	}

}

void FadeUI::FadeIn()
{
	if (fadeTimer.ReadMSec() >= fadeTime) {
		doingFadeIn = false;
	}
}

void FadeUI::FadeOut()
{
	if (fadeTimer.ReadMSec() < fadeTime) {

	}
	else {
		doingFadeOut = false;
	}
}
