#pragma once

struct Options
{
	double filterStat;
	float statTextColor[4];
	double statTextTopMargin;
	char recordKey[2];
	bool recordKeyShiftModifier;
	char executeKey[2];
	bool executeKeyShiftModifier;
	bool debug;
};

void drawOptionsWidget(Options& options);