#pragma once

#include "StdInc.h"
#include "Renderable.h"

class Text : public Renderable
{
private:

public:
	Text();
	~Text();

	virtual void render();
};