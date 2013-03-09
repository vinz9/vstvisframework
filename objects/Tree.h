#ifndef TREE_H
#define TREE_H

#include "Object3d.h"

class Tree : public Object3d
{

public:

	Tree();
	Tree(AudioEffect* effect);
	Tree(AudioEffect* effect, int animMode, float armsSt);
	~Tree();

	void draw();
	void updateParams();


private:

	void buildList();

	float leftArm;
	float rightArm;
	float boxScale;
	float redcolor;
	float aSt;

	int chemin;
	int trigger;
	int animate;

	int counter;

};

#endif