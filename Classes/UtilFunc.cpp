#include "UtilFunc.h"

float UtilFunc::getTimeTick() {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	return (ltm->tm_hour * 3600) + (ltm->tm_min * 60) + (ltm->tm_sec);
}