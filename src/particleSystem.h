#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED

#include "particle.h"
#include "tree.h"
#include "ofMain.h"

//This class handles the vector of Particles
class ParticleSystem{
public:
    //Constructor
    ParticleSystem();

    //Properties
    vector<Particle> particles;
    int width, height;
    Tree *quadTree;

    //Methods
    void setup(int width, int height);
    void add(Particle particle);
    int size();
    void applyForce(float x, float y, float radius, float force);
    void buildTree();
    void update();
    void draw();
};


#endif // PARTICLESYSTEM_H_INCLUDED
