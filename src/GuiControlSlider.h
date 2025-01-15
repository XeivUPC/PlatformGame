#pragma once
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "Vector2D.h"


class GuiControlSlider : public GuiControl
{

public:
	GuiControlSlider(SDL_Rect b, GuiControlButton* thumb, SDL_Texture* tex, float val = 1, float min=0, float max=1);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	void Render();
	void CleanUp();

	float GetValue();
	void SetValue(float value);

	void SetMinValue(float value);
	float GetMinValue();
	void SetMaxValue(float value);
	float GetMaxValue();

private:
	void MoveThumb();
	void RecalculateThumbPosition();
	void RecalculateValue();

	GuiControlButton* thumb = nullptr;
	float minVal;
	float maxVal;
	float value;

	bool thumbSelected = false;
};
