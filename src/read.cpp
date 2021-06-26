#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

Pix *normalizationAndThreshold(Pix *image)
{
	Pix *pixG = pixConvertTo8(image, 0);
	Pix *pix1 = pixBackgroundNorm(pixG, NULL, NULL, 10, 15, 230, 50, 255, 2, 2);
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

Pix *deskewImage(Pix *image)
{
	return pixUnsharpMasking(image, 1, 0.3f);
}

std::string readImage(Pix *input)
{
	std::string outText;

	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

	char *configs[] = {"data/config/bazaar"};

	if (api->Init("data", "eng", tesseract::OcrEngineMode::OEM_DEFAULT, configs, 1, nullptr, nullptr, false))
	{
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	api->SetPageSegMode(tesseract::PageSegMode::PSM_SPARSE_TEXT_OSD);

	Pix *normalized = normalizationAndThreshold(input);

	Pix *invert = invertImage(normalized);

	Pix *blur = blurImage(invert);

	Pix *deskew = deskewImage(blur);

	Pix *scaled = pixScaleToSize(deskew, blur->w * 2, blur->h * 2);

	Pix *finalImage = scaled;

	pixWrite("test.png", finalImage, 3);

	Pixa* pixa1 = pixExtractTextlines(finalImage, 150, 150, 0, 0, 5, 5, NULL);

    Boxa* boxa1 = pixaGetBoxa(pixa1, L_CLONE);

	Pix* pix2 = pixaDisplayRandomCmap(pixa1, 0, 0);
    pixcmapResetColor(pixGetColormap(pix2), 0, 255, 255, 255);
    pixDisplay(pix2, 400, 0);
    pixWrite("lines1.png", pix2, IFF_PNG);
	
	api->SetImage(scaled);

	outText = api->GetUTF8Text();

	printf("OCR output:\n%s", outText.c_str());
	

	api->End();
	delete api;

	pixDestroy(&normalized);
	pixDestroy(&invert);
	pixDestroy(&blur);
	pixDestroy(&deskew);
	pixDestroy(&scaled);
	pixDestroy(&input);
	pixDestroy(&finalImage);

	return outText;
}