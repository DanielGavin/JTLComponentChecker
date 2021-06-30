#include <Windows.h>

#include <vector>
#include <string>

bool createOverlay();
void changedOverlay();
void updateOverlay();
void setComponentStringOverlay(const std::string& componentString);
void setRectOverlay(RECT* rect);