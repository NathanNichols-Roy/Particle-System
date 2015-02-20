#include "particleSystem.h"

ParticleSystem::ParticleSystem() {
    quadTree = NULL;
}

void ParticleSystem::setup(int width, int height){
    this->width = width;
    this->height = height;
}

void ParticleSystem::add(Particle particle){
    particles.push_back(particle);
}

int ParticleSystem::size(){
    return particles.size();
}

void ParticleSystem::applyForce(float x, float y, float radius, float force){
    quadTree->applyForce(x, y, radius, force);
}

void ParticleSystem::buildTree(){
    if(quadTree != NULL)
    {
        delete quadTree;
        quadTree = NULL;
    }

    quadTree = new Tree;
    for(int i = 0; i < particles.size(); ++i)
    {
        quadTree->insert(particles[i]);
    }
    //quadTree->cleanTree();
    //quadTree->computeMass();
}

void ParticleSystem::update(){
    //ofVec2f force (0, 0);

    //buildTree();
    for(int i = 0; i < particles.size(); ++i)
    {
        particles[i].update();
        particles[i].reset();
    }

}

void ParticleSystem::draw(){
    //Specifies the primatives that will be created from vertices between Begin and End
    glBegin(GL_POINTS);
    for(int i = 0; i < particles.size(); ++i)
    {
        particles[i].draw();
    }
    glEnd();
}
