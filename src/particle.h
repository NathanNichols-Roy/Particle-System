#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "ofGraphics.h"
#include "ofMain.h"

class Particle {
public:
    //Constructor
    Particle(float _x, float _y, float xvel, float yvel) :
        x(_x), y(_y), x_vel(xvel), y_vel(yvel) {
        x_force = 0; y_force = 0; friction = 0.99;
        }

    //Properties
    float x, y;
    float x_vel, y_vel;
    float x_force, y_force;
    float friction;


    //Methods
    void reset(){
        x_force = 0;
        y_force = 0;
    }

    void bounce(){
        if(x < 0)
        {
            x = 0;
            x_vel *= -1;
        }
        if(x > ofGetWidth())
        {
            x = ofGetWidth();
            x_vel *= -1;
        }
        if(y < 0)
        {
            y = 0;
            y_vel *= -1;
        }
        if(y > ofGetHeight())
        {
            y = ofGetHeight();
            y_vel *= -1;
        }
    }

    void update() {
        bounce();
        x_vel += x_force;
        y_vel += y_force;
        x_vel *= friction;
        y_vel *= friction;
        x += x_vel;
        y += y_vel;
    }

    void draw() {
        //Vertex of form GLfloat x, GLfloat y
        glVertex2f(x, y);
    }


};

#endif // PARTICLE_H_INCLUDED
