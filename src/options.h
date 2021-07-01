#pragma once

struct Options
{
	double filterStat;
	float statTextColor[4];
	double statTextTopMargin;
	bool debug;
};

void drawOptionsWidget(Options& options);