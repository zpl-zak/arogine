#include "VoxelImage.h"
#include <stdio.h>
#include <stdlib.h>

float *VoxelImage::DownloadImage(size_t &size, const char* Path, size_t &twidth, size_t &theight)
{
	float *Buffer;
	size_t r = 0;
	{
		FILE *fp = fopen(Path, "rb");
		if(!fp)
		{
			printf("Could not load image: %s\n", Path);
			getchar();
			exit(-2);
		}

		size_t width, height;

		fread(reinterpret_cast<char*>(&width), sizeof(size_t), 1, fp);
		fread(reinterpret_cast<char*>(&height), sizeof(size_t), 1, fp);
		twidth = width;
		theight = height;

		//IMPORTANT(zaklaus): Don't forget to free it!!!
		Buffer = static_cast<float*>(malloc(sizeof(float) * width * height*3));

		for(size_t i=0; i<width*height*3; i++)
		{
			fread(reinterpret_cast<char*>(&Buffer[i]), sizeof(float), 1, fp);
			r += 1;
		}

		fclose(fp);
	}
	size = r/3; // r / 3 components (r,g,b) = 1 pixel
	return(Buffer);
}
