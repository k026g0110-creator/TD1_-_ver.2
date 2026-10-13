#include<stdio.h>
#include <Novice.h>
#include"関数.h"

int Add(int Index, int amout) {
	return Index + amout;
}
int Sub(int Index, int amout) {
	return Index - amout;
}
int Mul(int Index, int amout) {
	return Index * amout;
}
int floatAdd(float Index, float amout) {
	return static_cast<int>(Index) + static_cast<int>(amout);
}
int floatSub(float Index, float amout) {
	return static_cast<int>(Index) - static_cast<int>(amout);
}
int floatMul(float Index, float amout) {
	return static_cast<int>(Index) * static_cast<int>(amout);
}
void Circle(int positionX, int positionY, int radius, float angle, char color) {
	Novice::DrawEllipse(positionX, positionY, radius, radius, angle,color,kFillModeSolid);
}
