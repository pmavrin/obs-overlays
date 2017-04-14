/********************************************************************************
 Copyright (C) 2012 Hugh Bailey <obs.jim@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/


#include"MemoryCapture.h"

void MemoryCapture::Destroy()
{
	textureBuffers[0] = NULL;
	textureBuffers[1] = NULL;
	delete texture;
	texture = NULL;
}

bool MemoryCapture::Init(const char* file_name, int height, int pitch)
{
	this->height = height;
	this->pitch = pitch;

	m_file = boost::interprocess::file_mapping(file_name, boost::interprocess::read_only);
	region = boost::interprocess::mapped_region(m_file, boost::interprocess::read_only);

	texture = CreateTexture(pitch, height, GS_RGBA, NULL, NULL, FALSE);
	if (!texture)
	{
		AppWarning(TEXT("MemoryCapture::Init: Could not create texture"));
		return false;
	}
	return true;
}

Texture* MemoryCapture::LockTexture()
{
	BYTE* lpData;
	UINT texPitch;

	if (texture->Map(lpData, texPitch))
	{
		void* addr = region.get_address();
		const char* mem = static_cast<char*>(addr);
		std::size_t len = region.get_size();
		for (std::size_t i = 0; i < len; ++i)
			lpData[i ^ 3] = *mem++;

		texture->Unmap();
	}

	return texture;
}

void MemoryCapture::UnlockTexture()
{
}
