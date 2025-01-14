#include "GuiControlSlider.h"
#include "Engine.h"


GuiControlSlider::GuiControlSlider(SDL_Rect b, GuiControlButton* thumb, SDL_Texture* tex, float val , float min , float max) : GuiControl(GuiControlType::SLIDER, b, tex)
{
	this->thumb = thumb;
	SetMinValue(min);
	SetMaxValue(max);
	SetValue(val);
	RecalculateThumbPosition();
}

GuiControlSlider::~GuiControlSlider()
{
}

bool GuiControlSlider::Update(float dt)
{
	if (!isEnabled)
	{
		thumb->isEnabled = false;
		state = GuiControlState::DISABLED;
		return true;
	}
	else {
		thumb->isEnabled = true;
		state = GuiControlState::NORMAL;
	}
	GuiControl::Update(dt);

	thumb->Update(dt);

	if (thumb->CurrentState() == GuiControlState::PRESSED) {
		MoveThumb();
	}

	return false;
}

void GuiControlSlider::CleanUp()
{
	thumb->CleanUp();
	delete thumb;
	GuiControl::CleanUp();
}

float GuiControlSlider::GetValue()
{
	return value;
}

void GuiControlSlider::SetValue(float value)
{
	this->value = value;

	if (value > maxVal)
		value = maxVal;
	if (value < minVal)
		value = minVal;
	RecalculateThumbPosition();
	NotifyObserver();
}

void GuiControlSlider::SetMinValue(float value)
{
	minVal = value;
	RecalculateThumbPosition();
}

float GuiControlSlider::GetMinValue()
{
	return minVal;
}

void GuiControlSlider::SetMaxValue(float value)
{
	maxVal = value;
	RecalculateThumbPosition();
}

float GuiControlSlider::GetMaxValue()
{
	return maxVal;
}

void GuiControlSlider::MoveThumb()
{

	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
	mousePos.setX(mousePos.getX() - thumb->bounds.w / 2.f);
	Vector2D moveBounds = { bounds.x - thumb->bounds.w / 2.f , bounds.x + bounds.w - thumb->bounds.w / 2.f };

	if (mousePos.getX() < moveBounds.getX())
		mousePos.setX(moveBounds.getX());
	if (mousePos.getX() > moveBounds.getY())
		mousePos.setX( moveBounds.getY());

	if (thumb->bounds.x != mousePos.getX()) {
		thumb->bounds.x = mousePos.getX();
		RecalculateValue();
		NotifyObserver();
	}

}

void GuiControlSlider::RecalculateThumbPosition()
{
	Vector2D moveBounds = { bounds.x - thumb->bounds.w / 2 , bounds.x + bounds.w - thumb->bounds.w / 2 };
	thumb->bounds.x = moveBounds.getX() + (value - minVal) * (moveBounds.getY() - moveBounds.getX()) / (maxVal - minVal);
}

void GuiControlSlider::RecalculateValue()
{
	Vector2D moveBounds = { bounds.x - thumb->bounds.w / 2 , bounds.x + bounds.w - thumb->bounds.w / 2 };

	value =  minVal + (thumb->bounds.x - moveBounds.getX()) * (maxVal - minVal) / (moveBounds.getY() - moveBounds.getX());
}


