#include <iostream>
#include <map>
#include "read.h"

Pix *normalizationAndThreshold(Pix *image, int threshold)
{
	Pix *pixG = pixConvertTo8(image, 0);
	Pix *pix1 = pixBackgroundNorm(pixG, NULL, NULL, 10, 15, threshold, 50, 255, 2, 2);
	Pix *pix2 = pixThresholdToBinary(pix1, 100);
	return pix2;
}

Pix *invertImage(Pix *image)
{
	Pixa *pixadb = pixaCreate(0);
	pixaAddPix(pixadb, image, L_COPY);
	Pix *tmp;
	return pixAutoPhotoinvert(image, 128, &tmp, pixadb);
}

Pix *blurImage(Pix *image)
{
	Pix *pixG = pixConvertTo8(image, 0);
	return pixAddGaussianNoise(pixG, 1);
}

struct VerticalLine
{
	l_int32 w;
	l_int32 start;
	l_int32 length;
	l_int32 thickness;
};

char* readImage(tesseract::TessBaseAPI* api, Pix *input, Box* box)
{
	char* outText;

	Pix* crop = pixClipRectangle(input, box, nullptr);

	Pix *normalized = normalizationAndThreshold(crop, 150);

	Pix *invert = invertImage(normalized);

    Pix *blur = blurImage(invert);
	Pix *scaled = pixScaleToSize(blur, blur->w * 4, blur->h * 4);
	Pix *dilate = pixDilateGray(scaled, 3, 3);
	Pix *finalImage = dilate;

	pixWrite("crop.png", finalImage, IFF_PNG);

	api->SetImage(finalImage);
	api->SetSourceResolution(100);

	outText = api->GetUTF8Text();

	pixDestroy(&normalized);
	pixDestroy(&invert);
	pixDestroy(&blur);
	pixDestroy(&finalImage);
	pixDestroy(&scaled);
	pixDestroy(&input);

	return outText;
}