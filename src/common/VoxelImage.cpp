#include "VoxelImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>

std::vector<float> VoxelImage::DownloadImage(size_t& size, const std::string& path, size_t& twidth, size_t& theight)
{
	std::vector<float> Buffer;
	size_t r = 0;
	{
		FILE *fp = fopen(path.c_str(), "rb");
		if(!fp)
		{
			printf("Could not load image: %s\n", path.c_str());
			getchar();
			exit(-2);
		}

		size_t width, height;

		fread(reinterpret_cast<char*>(&width), sizeof(size_t), 1, fp);
		fread(reinterpret_cast<char*>(&height), sizeof(size_t), 1, fp);
		twidth = width;
		theight = height;

		float value;
		for(size_t i=0; i<width*height*3; i++)
		{
			fread(&value, sizeof(float), 1, fp);
			Buffer.push_back(value);
			r += 1;
		}

		fclose(fp);
	}
	size = r/3; // r / 3 components (r,g,b) = 1 pixel
	return(Buffer);
}
