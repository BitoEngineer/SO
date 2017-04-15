#include "stdafx.h"
#include <stdio.h>
#include <stdbool.h>
#include "tchar.h"
#include <Windows.h>


//#pragma pack(push, 1)
struct SDimension
{
	double px;
	double py;
} typedef Dimension;


struct JPEGInfo
{
	PUCHAR Model;
	Dimension * Values = new Dimension();
	PUCHAR Date;
	double Velocity;
	double Aperture;
	double Latitude;
	double Longitude;
	double Altitude;
}typedef IMAGE_INFO;




//#pragma pack(pop)
int getConcat(byte a, byte b);

void getImage_Info(wchar_t * fileName)
{
	IMAGE_INFO * img = new IMAGE_INFO();
	int tagsOfLE[9] = { 0x1001,0x1a01,0x1b01,0x3201,0x8299,0x0292,0x0100,0x1500,0x0600 };
	int tagsOfBE[9] = { 0x0110,0x011a,0x011b,0x0132,0x9982,0x9202,0x0001,0x0015,0x0006 };
	PUCHAR correspondentOffsetTags[9];

	HANDLE hFile = CreateFileW(fileName,
		GENERIC_READ ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
	}

	HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

	if (hMap == NULL) {
	}

	PUCHAR hView = (PUCHAR)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	int offsetToEndian = 0xc;
	char endian = *(hView + offsetToEndian);
	int * tags;
	bool BIGENDIAN = false;
	if (endian == 'I')
		tags = tagsOfLE;
	else
	{
		tags = tagsOfBE;
		BIGENDIAN = true;
	}
		

	char * exifData;
	int closeTag = 0xFFD9;
	PUCHAR iter = hView;
	iter += 0x4;
	int headerLength = getConcat(*iter, *(iter + 0x1));
	iter++;
	int i = 0;
	while (i<9)
	{
		iter++;
		if (getConcat(*iter, *(iter + 1)) == tags[i])
		{
			int format,component,offset;
		
			if(BIGENDIAN)
			{
				format = getConcat(*(iter + 2), *(iter + 3));
				component = (int)((*(iter + 6) << 28) | (*(iter + 7) << 16) | (*(iter + 4) << 8) | (*(iter + 5)));
				offset = (int)((*(iter + 8) << 28) | (*(iter +9) << 16) | (*(iter + 10) << 8) | (*(iter + 11)));
			}
			else
			{
				format = getConcat(*(iter + 3), *(iter + 2));
				component = (int)((*(iter + 7) << 28) | (*(iter + 6) << 16) | (*(iter + 5) << 8) | (*(iter + 4)));
				offset = (int)((*(iter + 11) << 28) | (*(iter + 10) << 16) | (*(iter + 9) << 8) | (*(iter + 8)));
			}

			int mult = format*component;
			PUCHAR res;
			if (mult < offset)
				res = hView + 0xc + offset;
			else
				res = hView + mult;
			

			correspondentOffsetTags[i] = res;
			++i;
		}
		--headerLength;
	}
	img->Model = correspondentOffsetTags[0];
	img->Values->px = *correspondentOffsetTags[1];
	img->Values->py = *correspondentOffsetTags[2];
	img->Date = correspondentOffsetTags[3];

	printf("Modelo: %s \n", img->Model);
	printf("Resolucao X: %f \n", img->Values->px);
	printf("Resolucao Y: %f \n", img->Values->py);
	printf("Data: %s \n", img->Date);
}

int getConcat(byte a, byte b)
{
	int combined = a << 8 | b;
	return combined;
}

int getConcat(int a, int b)
{
	int combined = a << 8 | b;
	return combined;
}

int main()
{
	wchar_t name[100] = L"C:/Users/Nuno Cardoso/Desktop/tst.jpg";

	getImage_Info(name);
	return 0;
}