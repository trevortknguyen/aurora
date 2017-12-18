#include <cmath>

#include "simulation.h"

const int NMAX = 2000;
int nAtom = 100;

// position
float r [NMAX][3];
// velocity
float rv[NMAX][3];
// acceleration
float ra[NMAX][3];

void initializeR()
{
    for (int i = 0; i < nAtom; i++)
    {
        float t = i / 10.0f - 10.0f;
        r[i][0] = 9.1f*cos(t);
        r[i][1] = 9.1f*sin(t);
        r[i][2] = t;
    }


    r[0][0] = 0.9201f;
    r[0][1] = -0.4001f;
    r[0][2] = 0.0001f;
    r[1][0] = -0.7005f;
    r[1][1] = -0.7005f;
    r[1][2] = -0.7005f;
    r[2][0] = 0.91f;
    r[2][1] = 0.91f;
    r[2][2] = 0.91f;
    r[3][0] = 0.91f;
    r[3][1] = -0.91f;
    r[3][2] = -0.91f;
    r[4][0] = 0.01f;
    r[4][1] = -0.01f;
    r[4][2] = -0.01f;
}


void rij(int i, int j, float* x, float* y, float* z)
{
    *x = r[i][0] - r[j][0];
    *y = r[i][1] - r[j][1];
    *z = r[i][2] - r[j][2];
}

// Lennard-Jones Potential

float a(int i, int j, float dx, float dy, float dz)
{
    float r = sqrt(dx*dx + dy*dy + dz*dz);
    return 48.0f / (r * r) * (1.0f / pow(r, 12.0f) - 1.0f / 2.0f / pow(r, 6));
}

// computeAccel
void doLJPotential()
{
    for (int i = 0; i < nAtom; i++)
    {
        ra[i][0] = 0;
        ra[i][1] = 0;
        ra[i][2] = 0;
    }
    for (int i = 0; i < nAtom - 1; i++)
    {
        for (int j = i + 1; j < nAtom; j++)
        {
            float x, y, z;
            rij(i, j, &x, &y, &z);
            float aij = a(i, j, x, y, z);
            ra[i][0] += x * aij;
            ra[i][1] += y * aij;
            ra[i][2] += z * aij;
            ra[j][0] -= x * aij;
            ra[j][1] -= y * aij;
            ra[j][2] -= z * aij;
        }
    }
}

// Verlet Discretization

double DeltaT {0.0001f};

void singleStep()
{
    for (int i = 0; i < nAtom; i++)
    {
        rv[i][0] += ra[i][0] * DeltaT / 2.0f;
        rv[i][1] += ra[i][1] * DeltaT / 2.0f;
        rv[i][2] += ra[i][2] * DeltaT / 2.0f;

        r[i][0] += rv[i][0] * DeltaT;
        r[i][1] += rv[i][1] * DeltaT;
        r[i][2] += rv[i][2] * DeltaT;
    }

    doLJPotential();

    for (int i = 0; i < nAtom; i++)
    {
        rv[i][0] += ra[i][0] * DeltaT / 2.0f;
        rv[i][1] += ra[i][1] * DeltaT / 2.0f;
        rv[i][2] += ra[i][2] * DeltaT / 2.0f;
    }
}

unsigned int StepLimit {100};
void velocityVerletDiscretization()
{
    // initialize the stuff
    doLJPotential();

    for (int i = 0; i < StepLimit; i++)
    {
        singleStep();
    }
}
