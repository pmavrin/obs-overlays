#include "overlaymaster.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
const char* FILE_NAME = "c:\\work\\image.bin";

int c = 0;

int color = 0xff000088;

TutorialImage::TutorialImage(XElement *data){
	VBData *points = new VBData;
	points->VertList.SetSize(4);              //Reserve space for the vertices
	box = CreateVertexBuffer(points, FALSE);  //Store the vertex list in a vertex buffer
	capture = new MemoryCapture();
	capture->Init(FILE_NAME, HEIGHT, WIDTH);
	tex = CreateTexture(1, 1, GS_RGBA, &color, NULL, NULL);
}


void TutorialImage::Render(const Vect2 &pos, const Vect2 &size){
	VBData *data = box->GetData();   
	
	data->VertList[0].Set(pos.x, pos.y, 0);      
	data->VertList[1].Set(pos.x, pos.y + size.y, 0);
	data->VertList[2].Set(pos.x + size.x, pos.y, 0);
	data->VertList[3].Set(pos.x + size.x, pos.y + size.y, 0);
	
	box->FlushBuffers();         
	LoadVertexBuffer(box);     
	
	if (capture)
	{
		Texture *tex = capture->LockTexture();
		if (tex)
		{
			LoadTexture(tex);
			Draw(GS_TRIANGLESTRIP);
			capture->UnlockTexture();
		} else
		{
			LoadTexture(this->tex);
			Draw(GS_TRIANGLESTRIP);
			capture->UnlockTexture();
		}
	} else
	{
		Log(TEXT("No capture"));
	}
	
}

Vect2 TutorialImage::GetSize() const{  //Return the size of our image. This can be
	Vect2 ret;                         //anything you want. This just determines the
	ret.x = WIDTH;                       //standard size of your plugin image.
	ret.y = HEIGHT;                       //You can also change this as much as you want
	return ret;                        //during runtime.
}

ImageSource* STDCALL CreateImage(XElement *data){
//	GraphicsCaptureSource* source = new GraphicsCaptureSource();
//	source->Init(data);
//	return source;
	return new TutorialImage(data);
}

bool LoadPlugin()
{
	OBSRegisterImageSourceClass(TEXT("VideoSource"), TEXT("Overlay Master"), (OBSCREATEPROC)CreateImage, (OBSCONFIGPROC)NULL);
	return true;
}

void UnloadPlugin()
{
}

CTSTR GetPluginName()
{
	return TEXT("Overlay Master plugin");
}

CTSTR GetPluginDescription()
{
	return TEXT("Plugin for anvanced overlays.");
}

