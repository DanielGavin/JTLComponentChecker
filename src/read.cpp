#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>
#include <map>

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

struct VerticalLine
{
	l_int32 w;
	l_int32 start;
	l_int32 length;
	l_int32 thickness;
};

void findMaxVerticalLines(Pix *image, std::vector<VerticalLine> &lines)
{
	l_int32 w, h, i, start, size;

	pixGetDimensions(image, &w, &h, NULL);

	std::vector<VerticalLine> tempLines;

	for (i = 0; i < w; ++i)
	{
		pixFindMaxVerticalRunOnLine(image, i, &start, &size);
		VerticalLine line;
		line.length = size;
		line.start = start;
		line.w = i;
		line.thickness = 1;
		tempLines.push_back(line);
	}

	if (tempLines.size() == 0)
	{
		return;
	}

	VerticalLine prevLine = tempLines[0];

	for (i = 1; i < tempLines.size(); ++i)
	{
		//this line is not the same as the previous add it to the final version
		if (!(tempLines[i].length == prevLine.length && tempLines[i].start == prevLine.start && tempLines[i].w == prevLine.w + 1))
		{
			if (prevLine.length > 500)
			{
				lines.push_back(prevLine);
			}
			prevLine = tempLines[i];
		}

		prevLine.thickness += 1;
	}
}

/*
	Uses vertical lines to segment out all the components and picks the one that seems most likely to fit.
	This is one method of finding it automatically, but there should be a manual version where the user can choose two points.
*/
Box findComponentBox(Pix *image, bool& ok)
{
	Pix *pix = pixConvertTo1(image, 100);

	std::vector<VerticalLine> candidates;

	findMaxVerticalLines(pix, candidates);

	std::vector<VerticalLine> sameLengthsAndStarting;

	std::map<int, std::vector<VerticalLine>> lengths;

	l_int32 minThickness = 100000;
	l_int32 maxLength = 0;
	l_int32 maxLengthDev = 10;

	//find the same lengths and maximum length
	for (auto i = 0; i < candidates.size(); ++i)
	{
		for (auto j = 0; j < candidates.size(); ++j)
		{
			if (candidates[i].length == candidates[j].length && candidates[i].start == candidates[j].start && i != j)
			{
				sameLengthsAndStarting.push_back(candidates[i]);
				minThickness = std::min(minThickness, candidates[i].thickness);
				maxLength = std::max(maxLength, candidates[i].length);
			}
		}
	}

	for (auto i = 0; i < sameLengthsAndStarting.size(); ++i)
	{
		if (minThickness == sameLengthsAndStarting[i].thickness && maxLength - maxLengthDev < sameLengthsAndStarting[i].length)
		{
			lengths[sameLengthsAndStarting[i].length].push_back(sameLengthsAndStarting[i]);
		}
	}

	if (lengths.size() > 2 || lengths.size() == 0)
	{
		ok = false;
		return {0};
	}

	//find the one two same lines that are furthest to the left
	l_int32 minW = 10000000;
	l_int32 index = 0;

	for (auto const &[key, val] : lengths)
	{
		for(auto line : val) {
			if (line.w < minW) {
				minW = line.w;
				index = key;
			}
		}
	}

	Box box;
	box.x = lengths[index][0].w;
	box.w = lengths[index][1].w - lengths[index][0].w;
	box.y = lengths[index][0].start;
	box.h = lengths[index][1].length;

	ok = true;

	return box;
}

char* readImage(Pix *input)
{
	char* outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

	char *configs[] = {"data/config/bazaar"};

	if (api->Init("data", "eng", tesseract::OcrEngineMode::OEM_DEFAULT, configs, 1, nullptr, nullptr, false))
	{
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	Pix *normalized = normalizationAndThreshold(input);

	Pix *invert = invertImage(normalized);

    Pix *blur = blurImage(invert);

	Pix *deskew = deskewImage(blur);

	Pix *scaled = pixScaleToSize(deskew, deskew->w * 4, deskew->h * 4);

	Pix *finalImage = scaled;

	bool foundComponent = false;

	Box box = findComponentBox(finalImage, foundComponent);

	if (!foundComponent)
	{
		return "";
	}

	api->SetImage(scaled);
	api->SetRectangle(box.x, box.y, box.w, box.h);

	api->SetSourceResolution(100);

	outText = api->GetUTF8Text();

	api->End();
	delete api;

	pixDestroy(&normalized);
	pixDestroy(&invert);
	pixDestroy(&blur);
	pixDestroy(&deskew);
	pixDestroy(&scaled);
	pixDestroy(&input);

	return outText;
}