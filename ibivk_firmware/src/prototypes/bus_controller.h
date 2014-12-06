/*
 * bus_controller.h
 *
 *  Created on: 06.12.2014
 *      Author: Ludmila
 */

#ifndef BUS_CONTROLLER_H_
#define BUS_CONTROLLER_H_

void Load1553MessageF1(EntryCore1553* entry, unsigned short cw, unsigned short data_words[]);
void Load1553MessageF2(EntryCore1553* entry, unsigned short cw);

#endif /* BUS_CONTROLLER_H_ */
