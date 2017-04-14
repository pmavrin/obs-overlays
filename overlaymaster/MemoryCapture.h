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

#pragma once

#include "OBSApi.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

class MemoryCapture 
{
    LPBYTE textureBuffers[2];
    UINT pitch;

    Texture *texture;
    UINT height;

	boost::interprocess::file_mapping m_file;
	boost::interprocess::mapped_region region;

public:
    void Destroy();

	virtual bool Init(const char* file_name, int height, int pitch);

    virtual Texture* LockTexture();
    virtual void UnlockTexture();
};
