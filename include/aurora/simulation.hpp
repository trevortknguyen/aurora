#ifndef AURORA_SIMULATION_H
#define AURORA_SIMULATION_H

extern int nAtom;
extern float r [][3];
// extern float[] rv;
// extern float[] ra;

void initializeR();

void velocityVerletDiscretization();

#endif
