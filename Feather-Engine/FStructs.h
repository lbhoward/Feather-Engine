#ifndef FSTRUCTS_H
#define FSTRUCTS_H

struct FColour
{
	FColour(float R, float G, float B, float A)
	{
		Colour[0] = R;
		Colour[1] = G;
		Colour[2] = B;
		Colour[3] = A;
	}

	float Colour[4];
};

struct FVertex
{
	float X,Y,Z;
	FColour Colour;
};

#endif