#pragma once
#include "Singleton.h"
#include "imgui.h"
#include "imgui_plot.h"
#include "IObject.h"
	
#include <vector>

class Debug : public dae::Singleton<Debug>, public IObject
{
public:
	Debug() {};

	void PlotData(const char* title, const std::vector<float>& x, const std::vector<float>& y);
private:

};

