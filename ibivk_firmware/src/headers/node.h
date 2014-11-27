/*
 * node.h
 *
 *  Created on: 10.10.2014
 *      Author: User
 */

#ifndef NODE_H_
#define NODE_H_


	typedef struct Node
	{
	   signed int   value;
	   struct Node* next;
	}Node;


	typedef struct BigNode
	{
	   int          value1;
	   int          value2;
	   int          value3;
	   int          value4;
	   struct BigNode* next;
	}BigNode;


#endif /* NODE_H_ */
