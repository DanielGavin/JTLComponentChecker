#pragma once

#include <string>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


char* readImage(tesseract::TessBaseAPI* api, Pix* image);