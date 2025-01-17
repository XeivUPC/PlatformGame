#pragma once

#include <string>
#include "pugixml.hpp"

using namespace pugi;

class GuiControl;

class Module
{
public:

	Module(bool isActive = true) : active(isActive)
	{
	
	}

	void Enable()
	{
		Start();
		active = true;
	}

	void Disable()
	{
		CleanUp();
		active = false;
	}

	// Called before render is available
	virtual bool Awake()
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadParameters(xml_node parameters)
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	std::string name;
	bool active;

};