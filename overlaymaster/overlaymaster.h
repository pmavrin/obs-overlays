#pragma once
#include "OBSApi.h"
#include "MemoryCapture.h"

extern "C" __declspec(dllexport) bool LoadPlugin();
extern "C" __declspec(dllexport) void UnloadPlugin();
extern "C" __declspec(dllexport) CTSTR GetPluginName();
extern "C" __declspec(dllexport) CTSTR GetPluginDescription();

class TutorialImage : public ImageSource{
	VertexBuffer *box;
	Texture *tex;
	MemoryCapture *capture;
public:
	TutorialImage(XElement *data);
	void update_tex();
	void Render(const Vect2 &pos, const Vect2 &size);
	Vect2 GetSize() const;
};
