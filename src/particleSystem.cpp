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

void ParticleSystem::size(){
    particles.size();
}

void ParticleSystem::applyForce(float x, float y, float radius){
    float dist;
    for(int i = 0; i < particles.size(); ++i)
    {
        dist = ofDist(x, y, particles[i].x, particles[i].y);
        if(dist < radius)
        {
            ofVec2f force_vec(particles[i].x - x, particles[i].y - y);
            particles[i].x_force = force_vec.x/90;
            particles[i].y_force = force_vec.y/90;
        }
        else
        {
            particles[i].x_force = 0;
            particles[i].y_force = 0;
        }
    }
}

void ParticleSystem::buildTree(){
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

    if(quadTree != NULL)
    {
        delete quadTree;
        quadTree = NULL;
    }
    buildTree();
    for(int i = 0; i < particles.size(); ++i)
    {
        //force = quadTree->treeForce(particles[i]);
        //cout << i << " (" << force.x << ", " << force.y << ")" << endl;
        //particles[i].x_force = force.x;
        //particles[i].y_force = force.y;
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
