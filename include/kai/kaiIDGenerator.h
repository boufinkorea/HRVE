/**
@file kaiIDGenerator.h
@brief simple id generator class 
@author Wonsub Choi
@date 2008/2/27
*/
#ifndef __KAIIDGENERATOR_H__
#define __KAIIDGENERATOR_H__

class KAIEXPORT kaiIDGenerator
{	
public:
	kaiIDGenerator(unsigned short size);
	~kaiIDGenerator();

	unsigned short getID();
	void releaseID(unsigned short id);

private:
	struct Node
	{
		unsigned short	id;
		Node*			next;
	};

	Node* _beginNode;
	Node* _endNode;

	Node* _nodeMemory;
	unsigned short _size;

};

#endif
