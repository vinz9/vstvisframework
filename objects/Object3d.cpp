#include "Object3d.h"

int boxid;

//----------------------------------------------------------------------------
Object3d::Object3d()
{

}

Object3d::Object3d(AudioEffect* effect):effect(effect)
{

	for (int i = 0; i<MAXPARAMS; i++)
	{
		params[i] = NULL;
	}
}


//----------------------------------------------------------------------------
Object3d::~Object3d()
{

}

//----------------------------------------------------------------------------
void Object3d::setParams(int* inputs)
{
	params[0] = &(((VstPlugin*)effect)->audioInputs[inputs[0]]->level);
	params[1] = &(((VstPlugin*)effect)->audioInputs[inputs[1]]->level);
	params[2] = &(((VstPlugin*)effect)->audioInputs[inputs[2]]->level);
	params[3] = &(((VstPlugin*)effect)->audioInputs[inputs[3]]->level);
	
	/*params[0] = &(((VstPlugin*)effect)->audioInputs[inputs[0]]->level);
	params[1] = &(((VstPlugin*)effect)->audioInputs[inputs[1]]->level);*/
	
	/*for (int i = 0; i<MAXPARAMS; i++)
	{
		if (inputs[i] >= 0)
		{
			params[i] = &(((VstPlugin*)effect)->audioInputs[inputs[i]]->level);
		}
	}*/

}

