/*
 * mil1553.h
 *
 *  Created on: 24.11.2014
 *      Author: Ludmila
 */

#ifndef MIL1553_H_
#define MIL1553_H_

enum Mil1553_Direction
 {
	 BCtoRT 		= 0x00,
	 RTtoBC 		= 0x01,
 };

#define MIL1553_GET_DIRECTION_BIT(cw, dir)												\
{																						\
	*dir = (cw >> 10) & 0x01;															\
}

#define MIL1553_GET_WORDCOUNT(cw, n_dw)													\
{																						\
	*n_dw = (cw >> 0) & 0x1F;															\
}


#endif /* MIL1553_H_ */
