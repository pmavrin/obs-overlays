#include "overlaymaster.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <fstream>

const int WIDTH = 1280;
const int HEIGHT = 720;
const int BUF_SIZE = WIDTH * HEIGHT * 4;
const char* FILE_NAME = "c:\\work\\image.bin";

char *pixels;
char *mapped_data;

int c = 0;

boost::interprocess::file_mapping m_file;
boost::interprocess::mapped_region region;

TutorialImage::TutorialImage(XElement *data){
	VBData *points = new VBData;
	points->VertList.SetSize(4);              //Reserve space for the vertices
	box = CreateVertexBuffer(points, FALSE);  //Store the vertex list in a vertex buffer
	pixels = static_cast<char*>(malloc(BUF_SIZE));
	for (int i = 0; i < BUF_SIZE; i++) {
		char q = (i + c) & 255;
		pixels[i] = q;
	}
	{  
		m_file = boost::interprocess::file_mapping(FILE_NAME, boost::interprocess::read_only);
		region = boost::interprocess::mapped_region(m_file, boost::interprocess::read_only);
	}
	tex = CreateTexture(WIDTH, HEIGHT, GS_RGBA, pixels, FALSE, TRUE);  
}

void TutorialImage::update_tex()
{
	BYTE *lpData;
	UINT texPitch;
	
	if (tex->Map(lpData, texPitch))
	{
		
		void * addr = region.get_address();
		const char *mem = static_cast<char*>(addr);
		std::size_t len = region.get_size();
		for (std::size_t i = 0; i < len; ++i)
			lpData[i ^ 3] = *mem++;
		
		tex->Unmap();
	}
	

}

void TutorialImage::Render(const Vect2 &pos, const Vect2 &size){
	VBData *data = box->GetData();   
	
	update_tex();
	
	data->VertList[0].Set(pos.x, pos.y, 0);      
	data->VertList[1].Set(pos.x, pos.y + size.y, 0);
	data->VertList[2].Set(pos.x + size.x, pos.y, 0);
	data->VertList[3].Set(pos.x + size.x, pos.y + size.y, 0);
	box->FlushBuffers();         
	LoadVertexBuffer(box);     
	LoadTexture(tex);
	Draw(GS_TRIANGLESTRIP);
	//delete tex;
}

Vect2 TutorialImage::GetSize() const{  //Return the size of our image. This can be
	Vect2 ret;                         //anything you want. This just determines the
	ret.x = WIDTH;                       //standard size of your plugin image.
	ret.y = HEIGHT;                       //You can also change this as much as you want
	return ret;                        //during runtime.
}

ImageSource* STDCALL CreateTutorialImage(XElement *data){
	return new TutorialImage(data);
}


bool LoadPlugin()
{
	OBSRegisterImageSourceClass(TEXT("VideoSource"), TEXT("Overlay Master"), (OBSCREATEPROC)CreateTutorialImage, (OBSCONFIGPROC)NULL);
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

