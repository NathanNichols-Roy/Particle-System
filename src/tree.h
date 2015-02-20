#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "particle.h"
#include "ofMain.h"

#define NODE_CAPACITY 4
#define THRESHOLD 0.5
//Quad Tree class to handle collision of particles
class Tree{
public:
    //Constructors
    //Root
    Tree() {
        minX = 0;
        minY = 0;
        maxX = ofGetWidth();
        maxY = ofGetHeight();
        mass = 0;
        cmassX = ofGetWidth()/2;
        cmassY = ofGetHeight()/2;
        num_particles = 0;
        has_children = false;
    }
    //Nodes
    Tree(float _minX, float _minY, float _maxX, float _maxY) :
        minX(_minX), minY(_minY), maxX(_maxX), maxY(_maxY) {
        num_particles = 0;
        has_children = false;
    }
    //Destructor
    ~Tree() {
        if(has_children){
            delete nw;
            delete ne;
            delete se;
            delete sw;
        }
    }

    //Properties
    float minX, minY, maxX, maxY;
    float mass, cmassX, cmassY;
    int num_particles;
    bool has_children;
    Particle *particles[NODE_CAPACITY];
    Tree *nw, *ne, *se, *sw;

    //Methods
    void insert(Particle &p) {
        float midX = (minX + maxX) / 2;
        float midY = (minY + maxY) / 2;
        //node is branch (has been split)
        if(has_children)
        {
            if(p.x < midX)
            {
                if(p.y < midY)
                    nw->insert(p);
                else
                    sw->insert(p);
            }
            else
            {
                if(p.y < midY)
                    ne->insert(p);
                else
                    se->insert(p);
            }
        }
        //node is leaf (not split)
        else
        {
            //add particle to node
            if(num_particles < NODE_CAPACITY)
            {
                particles[num_particles] = &p;
                num_particles++;
            }
            //NODE_CAPACITY = split
            else
            {
                //create children
                nw = new Tree(minX, minY, midX, midY);
                ne = new Tree(midX, minY, maxX, midY);
                se = new Tree(midX, midY, maxX, maxY);
                sw = new Tree(minX, midY, midX, maxY);
                has_children = true;
                //move points into respective subnodes recursively
                for(int i = 0; i < num_particles; ++i)
                {
                    insert(*particles[i]);
                }
                insert(p);
            }
        }
    }

    void applyForce(float x, float y, float radius, float force) {
        //particle is within node + radius
        if(x > (minX - radius) && x < (maxX + radius) && y > (minY - radius) && y < (maxY + radius))
        {
            float xd, yd;
            float dist;
            //internal node
            if(has_children)
            {
                nw->applyForce(x, y, radius, force);
                ne->applyForce(x, y, radius, force);
                se->applyForce(x, y, radius, force);
                sw->applyForce(x, y, radius, force);
            }
            //leaf
            else
            {
                //has at least 1 particle in node
                if(num_particles <= NODE_CAPACITY && num_particles != 0)
                {
                    for(int i = 0; i < num_particles; ++i)
                    {
                        Particle &cur = *particles[i];
                        xd = cur.x - x;
                        yd = cur.y - y;
                        if(xd != 0 && yd != 0)
                        {
                            dist = ofDist(x, y, cur.x, cur.y);
                            if(dist < radius)
                            {
                                ofVec2f force_vec;
                                if(force < 0)
                                {
                                    force_vec.set(x - cur.x, y - cur.y);
                                }
                                else
                                {
                                    force_vec.set(xd, yd);
                                }

                                //convert to unit vector
                                force_vec.normalize();
                                //force_vec * force;
                                force_vec.scale(force);

                                cur.x_force = force_vec.x;
                                cur.y_force = force_vec.y;
                            }
                        }
                    }

                }
            }
        }
    }

    //compute the mass and center of mass for every node in the tree
    void computeMass() {
        //internal node
        if(has_children)
        {
            nw->computeMass();
            ne->computeMass();
            se->computeMass();
            sw->computeMass();

            mass = nw->mass + ne->mass + se->mass + sw->mass;
            cmassX = (nw->mass*nw->cmassX +
                      ne->mass*ne->cmassX +
                      se->mass*se->cmassX +
                      sw->mass*se->cmassX) / mass;
            cmassY = (nw->mass*nw->cmassY +
                      ne->mass*ne->cmassY +
                      se->mass*se->cmassY +
                      sw->mass*se->cmassY) / mass;
        }
        //leaf node
        else if(num_particles < NODE_CAPACITY && num_particles != 0)
        {
            mass = num_particles;
            for(int i = 0; i < num_particles; ++i)
            {
                cmassX += particles[i]->x;
                cmassY += particles[i]->y;
            }
            cmassX /= mass;
            cmassY /= mass;
        }
        else
        {
            mass = 0;
            cmassX = (minX + maxX)/2;
            cmassY = (minY + maxY)/2;
        }
    }

    //Barnes-Hut Algorithm to
    //compute gravitational force on individual particle
    ofVec2f treeForce(Particle &p) {
        ofVec2f force (0, 0);
        float r = 0;
        float width = 0;
        float _x, _y;
        //gravitational constant
        float g = .01;
        if(has_children)
        {
            //distance from particle to center of mass of node
            r = ofDist(p.x, p.y, cmassX, cmassY);
            width = (maxX - minX);
            if((width/r) < THRESHOLD)
            {
                _x = (cmassX - p.x) * (cmassX - p.x);
                _y = (cmassY - p.y) * (cmassY - p.y);
                r = sqrt( _x + _y );
                r = r*r*r;
                force.x = g * mass * ( (cmassX - p.x)/r );
                force.y = g * mass * ( (cmassY - p.y)/r );

               // force.x = .01;
               // force.y = .01;

                return force/9;
            }
            else
            {
                force = nw->treeForce(p) + ne->treeForce(p) + se->treeForce(p) + sw->treeForce(p);
                return force;
            }
        }
        else if(num_particles < NODE_CAPACITY && num_particles != 0)
        {
            //check if p is in node
            float xd, yd;
            for(int i = 0; i < num_particles; ++i)
            {
                xd = particles[i]->x - p.x;
                yd = particles[i]->y - p.y;
                if(xd ==0 && yd == 0)
                {
                    return force;
                }
            }

            _x = (cmassX - p.x) * (cmassX - p.x);
            _y = (cmassY - p.y) * (cmassY - p.y);
            r = sqrt( _x + _y );
            r = r*r*r;
            force.x = g * mass * ( (cmassX - p.x)/r );
            force.y = g * mass * ( (cmassY - p.y)/r );


            //force.x = -.01;
            //force.y = -.01;

            return force/9;
        }
        else
        {
            return force;
        }
    }

    //Delete empty leaves
    //***Doesnt work well with drawTree***
    //Deletes children and other functions call those children
    void cleanTree() {
        if(has_children){
            nw->cleanTree();
            ne->cleanTree();
            se->cleanTree();
            sw->cleanTree();
        }
        else if(num_particles == 0)
        {
            delete this;
        }
    }

    void drawTree() {
        ofNoFill();
        ofSetColor(ofColor::orchid);
        ofRect(minX, minY, maxX - minX, maxY - minY);
        if(has_children)
        {
            nw->drawTree();
            ne->drawTree();
            sw->drawTree();
            se->drawTree();
        }
    }
};


#endif // TREE_H_INCLUDED
