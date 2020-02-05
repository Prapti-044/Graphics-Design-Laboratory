#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <cstring>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif // STB_IMAGE_IMPLEMENTATION

#include "RGBpixmap.cpp"



#define PI 3.1415926535897932384626433832795
RGBpixmap pix[7];

static bool light0 = true;
static bool light1 = true;

class Vector
{
    public:
        Vector(double x=0, double y=0, double z=0) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        double getX() { return x; }
        double getY() { return y; }
        double getZ() { return z; }

        void setX(double newX) { x = newX; }
        void setY(double newY) { y = newY; }
        void setZ(double newZ) { z = newZ; }

        Vector cross(Vector v) {
            double x = getY() * v.getZ() - getZ() * v.getY();
            double y = getZ() * v.getX() - getX() * v.getZ();
            double z = getX() * v.getY() - getY() * v.getX();
            return Vector(x, y, z);
        }

        Vector unit() {
            double div = sqrt(getX()*getX() + getY()*getY() + getZ()*getZ());
            return Vector(getX()/div, getY()/div, getZ()/div);
        }

        Vector operator*(Vector v) {
            return Vector(getX()*v.getX(), getY()*v.getY(), getZ()*v.getZ());
        }
        Vector operator+(Vector v) {
            return Vector(getX()+v.getX(), getY()+v.getY(), getZ()+v.getZ());
        }
        Vector operator-(Vector v) {
            return Vector(getX()-v.getX(), getY()-v.getY(), getZ()-v.getZ());
        }

        void operator*=(Vector v) {
            setX(getX()*v.getX());
            setY(getY()*v.getY());
            setZ(getZ()*v.getZ());
        }
        void operator+=(Vector v) {
            setX(getX()+v.getX());
            setY(getY()+v.getY());
            setZ(getZ()+v.getZ());
        }
        void operator-=(Vector v) {
            setX(getX()-v.getX());
            setY(getY()-v.getY());
            setZ(getZ()-v.getZ());
        }

        Vector operator*(double s) {
            return Vector(getX()*s, getY()*s, getZ()*s);
        }
        Vector operator+(double s) {
            return Vector(getX()+s, getY()+s, getZ()+s);
        }
        Vector operator-(double s) {
            return Vector(getX()-s, getY()-s, getZ()-s);
        }

        friend std::ostream & operator << (std::ostream &out, Vector v);

        double angle(Vector v) {
            double sinVal = (this->operator*(v)).magnitude() / (magnitude() * v.magnitude());
            return asin(sinVal);
        }
        double magnitude() {
            return sqrt(getX()*getX() + getY()*getY() + getZ()*getZ());
        }
    private:
        double x=0, y=0, z=0;
};

std::ostream& operator<< (std::ostream &out, Vector v) {
    out << "X : " << v.getX() << "; " << "Y : " << v.getY() << "; " << "Z : " << v.getZ() << ";";
}

static int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;
double fovy = 60.0;
Vector pos(0, 4, -5);
Vector dir = Vector(0, 0, 5);
Vector head(0, 1, 0);
double moveVelocity = 0.1;
double rotationVelocity = 0.1;

static void camUpdate() {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (double)WINDOW_WIDTH/WINDOW_HEIGHT, 0.01, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vector refPoint = pos + dir;
    gluLookAt(
              pos.getX(), pos.getY(), pos.getZ(),
              refPoint.getX(), refPoint.getY(), refPoint.getZ(),
              head.getX(), head.getY(), head.getZ()
              );
}

static void resize(int width, int height)
{
    WINDOW_HEIGHT = height;
    WINDOW_WIDTH = width;
    const float ar = (float) width/2 / (float) height;

    glViewport(0, 0, width, height);
    camUpdate();

    glFrustum(-ar, ar, -1.0, 1.0, 0.01, 300.0);
}


static bool light0_state = true;
const GLfloat light0_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat light0_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light0_position[] = { 2.0f, 5.0f, 3.0f, 0.0f };

static bool light1_state = true;
const GLfloat light1_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat light1_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
Vector light1_position(-3.0f, 5.0f, 0.0f);
Vector light1_direction = Vector(-3, 2, 3) - light1_position;
GLfloat light1_pos[] = { light1_position.getX(), light1_position.getY(), light1_position.getZ(), 1.0f };
GLfloat spot_dir[] = { light1_direction.getX(), light1_direction.getY(), light1_direction.getZ()};


unsigned int textureIDs[5];
static unsigned int loadTexture(char* filename) {
    int imwidth, imheight, nrChannels;
    unsigned char *data = stbi_load(filename, &imwidth, &imheight, &nrChannels, STBI_rgb);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //Defines parameters for texture coordinate generation
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imwidth, imheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    stbi_image_free(data);
    return textureID;
}

static void texturedCube(double width, double height, double length, unsigned int textureID) {
    double sizes[] = {
        width, height, // front
        width, height, // back
        length, height, // left
        length, height, // right
        width, length, // top
        width, length // bottom
    };

    double rotations[] = {
        180, 0, 1, 0,
        0, 0, 1, 0,
        -90, 0, 1, 0,
        90, 0, 1, 0,
        -90, 1, 0, 0,
        90, 1, 0, 0
    };

    double translations[] = {
        0, 0, -length/2,
        0, 0, +length/2,
        -width/2, 0, 0,
        width/2, 0, 0,
        0, height/2, 0,
        0, -height/2, 0
    };

    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glRotated(180, 0, 0, 1);
        for(int i=0; i<6; i++) {
            glPushMatrix();
                glTranslated(translations[i*3+0], translations[i*3+1], translations[i*3+2]);
                glRotated(rotations[i*4], rotations[i*4+1], rotations[i*4+2], rotations[i*4+3]);
                glTranslated(-sizes[2*i + 0]/2, -sizes[2*i + 1]/2, 0);
                glBegin(GL_QUADS);
                    glVertex3d(0,              0,              0);
                    glVertex3d(sizes[2*i + 0], 0,              0);
                    glVertex3d(sizes[2*i + 0], sizes[2*i + 1], 0);
                    glVertex3d(0,              sizes[2*i + 1], 0);
                glEnd();
            glPopMatrix();
        }
        glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

static void setMaterial(float mat_ambient[], GLfloat mat_diffuse[], GLfloat mat_specular[], GLfloat high_shininess[]) {
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

static unsigned int fractalDepth = 0;
void drawFractal(double depth, double len, double cx, double cy) {
    if(depth == 0) {
        glBegin(GL_TRIANGLES);
            glVertex2d(cx+len*0.5, cy+len*0.8660254037844386);
            glVertex2d(cx+len, cy);
            glVertex2d(cx, cy);
        glEnd();
        return;
    }
    depth--;

    drawFractal(depth, len/2, cx, cy);
    drawFractal(depth, len/2, cx+len/2, cy);
    drawFractal(depth, len/2, cx+len/2*0.5, cy+len/2*0.8660254037844386);
}

static GLfloat room_mat_ambient[4]    = { 0.7f, 0.7f, 0.7f, 1.0f };
static GLfloat room_mat_diffuse[4]    = { 0.8f, 0.8f, 0.8f, 1.0f };
static GLfloat room_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat room_shininess[1] = { 100.0f };
GLfloat fractal_mat_ambient1[4]    = { 0.5f, 0.0f, 0.0f, 1.0f };
GLfloat fractal_mat_diffuse1[4]    = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat fractal_mat_specular1[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat fractal_shininess1[1] = { 100.0f };


static void Cube(double height=1, double width=1, double length=1)
{
    GLdouble cube_vertices[] = {
        -width/2, -height/2,  length/2, // 0
         width/2, -height/2,  length/2, // 1
         width/2,  height/2,  length/2, // 2
        -width/2,  height/2,  length/2, // 3
        -width/2, -height/2, -length/2, // 4
         width/2, -height/2, -length/2, // 5
         width/2,  height/2, -length/2, // 6
        -width/2,  height/2, -length/2  // 7
    };

    GLubyte cube_indices[] = {
        3, 2, 6, 7,
        7, 6, 5, 4,
        1, 5, 6, 2,
        0, 1, 2, 3,
        0, 3, 7, 4,
        4, 5, 1, 0
    };


    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_vertices);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
    glDisableClientState( GL_VERTEX_ARRAY );
}

static void Pyramid(double baseWidth=1, double baseLength=1, double height=1)
{
    glBegin(GL_TRIANGLES);
        // Front
        glVertex3f( -baseWidth/2, 0, baseLength/2 );
        glVertex3f( baseWidth/2, 0, baseLength/2 );
        glVertex3f( 0, height, 0 );

        // Back
        glVertex3f( -baseWidth/2, 0, -baseLength/2 );
        glVertex3f( 0, height, 0 );
        glVertex3f( baseWidth/2, 0, -baseLength/2);

        //right
        glVertex3f( baseWidth/2, 0, baseLength/2 );
        glVertex3f( baseWidth/2, 0, -baseLength/2);
        glVertex3f( 0, height, 0 );

        // Left
        glVertex3f( -baseWidth/2, 0, -baseLength/2 );
        glVertex3f( -baseWidth/2, 0, baseLength/2 );
        glVertex3f( 0, height, 0);
    glEnd();

    // Bottom
    glBegin(GL_QUADS);
        glVertex3f( -baseWidth/2, 0, baseLength/2 );
        glVertex3f( -baseWidth/2, 0, -baseLength/2 );
        glVertex3f( baseWidth/2, 0, -baseLength/2 );
        glVertex3f( baseWidth/2, 0, baseLength/2 );
    glEnd();
}

static void windMillStand()
{
    // Left front
    glPushMatrix();
        glTranslated(-0.3, 0, -0.3);
        glRotated(15, 1, 0, 0); //confused
        glRotated(-15, 0, 0, 1);
        Cube(2, 0.05, 0.05);
    glPopMatrix();

    // Right front
    glPushMatrix();
        glTranslated(0.3, 0, -0.3);
        glRotated(15, 1, 0, 0);
        glRotated(15, 0, 0, 1);
        Cube(2, 0.05, 0.05);
    glPopMatrix();

    // Left back
    glPushMatrix();
        glTranslated(-0.3, 0, 0.3);
        glRotated(-15, 1, 0, 0);
        glRotated(-15, 0, 0, 1);
        glTranslated(0, 0.05, 0);
        Cube(2.04, 0.05, 0.05);
    glPopMatrix();

    // Right back
    glPushMatrix();
        glTranslated(0.3, 0, 0.3);
        glRotated(-15, 1, 0, 0);
        glRotated(15, 0, 0, 1);
        glTranslated(0, 0.05, 0);
        Cube(2.04, 0.05, 0.05);
    glPopMatrix();


    // Support back
    glPushMatrix();
        glTranslated(0, -0.5, 0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();

    // Support front
    glPushMatrix();
        glTranslated(0, -0.5, -0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();

    // Support left
    glPushMatrix();
        glRotated(90, 0, 1, 0);
        glTranslated(0, -0.5, 0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();

    // Support right
    glPushMatrix();
        glRotated(-90, 0, 1, 0);
        glTranslated(0, -0.5, 0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();



    // Upper Support back
    glPushMatrix();
        glTranslated(0, 0.1, 0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();

    // Upper Support front
    glPushMatrix();
        glTranslated(0, 0.1, -0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();

    // Upper Support left
    glPushMatrix();
        glRotated(90, 0, 1, 0);
        glTranslated(0, 0.1, 0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();

    // Upper Support right
    glPushMatrix();
        glRotated(-90, 0, 1, 0);
        glTranslated(0, 0.1, 0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();
}


static void windMillBlade(double length=1)
{
    glPushMatrix();
        glTranslated(0, length/2, 0);
        Cube(length, 0.2, 0.05);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, length, 0);
        Pyramid(0.2, .05, 0.3);
    glPopMatrix();
}

static void windMillFan(double length=0.8, double gap=0.18)
{
    glPushMatrix();
        glRotated(0*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    glPushMatrix();
        glRotated(1*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    glPushMatrix();
        glRotated(2*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
        glTranslated(0, 0, -0.45);
        gluCylinder(quad, 0.1, 0.2, 0.6, 32, 32);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 0, 0.15);
        gluDisk(quad, 0, 0.2, 32, 32);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 0, -0.45);
        glRotated(180, 1, 0, 0);
        gluDisk(quad, 0, 0.1, 32, 32);
    glPopMatrix();
}

static float fanRotate = 0;
static float FANSTATE = true;

static GLfloat windmill_mat_ambient[4]    = { 0.0f, 0.1f, 0.6f, 1.0f }; //0.1f, 0.1f, 0.7f, 1.0f
static GLfloat windmill_mat_diffuse[4]    = { 0.0f, 0.1f, 0.6f, 1.0f }; //0.1f, 0.1f, 0.7f, 1.0f
static GLfloat windmill_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat windmill_shininess[1] = { 100.0f };

static void windMill()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,   windmill_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   windmill_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  windmill_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, windmill_shininess);

    glPushMatrix();
        windMillStand();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 1.05, 0.3);
        glRotated(fanRotate, 0, 0, 1);
        windMillFan();
    glPopMatrix();

}


static void Room() {
    setMaterial(room_mat_ambient, room_mat_diffuse, room_mat_specular, room_shininess);

    // Front
    glPushMatrix();
        glTranslated(0, 5, 5);
        glPushMatrix();
            glTranslated(-2, 1.5, -0.25);
            glScaled(4, 4, 0.01);
            glPushMatrix();
                setMaterial(fractal_mat_ambient1, fractal_mat_diffuse1, fractal_mat_specular1, fractal_shininess1);
                glTranslated(0, 0, -0.51);
                drawFractal(fractalDepth, 0.9, -0.45, -0.4);
                setMaterial(room_mat_ambient, room_mat_diffuse, room_mat_specular, room_shininess);
            glPopMatrix();
            glutSolidCube(1);
        glPopMatrix();
        texturedCube(10, 10, 0.2, textureIDs[3]);
    glPopMatrix();

    // Right
    glPushMatrix();
        glTranslated(-5, 5, 0);
        glRotated(90, 0, 1, 0);

        glPushMatrix();
            glTranslated(2.5, -5, -8);
            glScaled(20, 20, 0.1);
            texturedCube(2, 2, 0.1, textureIDs[2]);
        glPopMatrix();

        glPushMatrix();
            glTranslated(3.25, 0, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-3.25, 0, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, 3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, -3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(3.25, -3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(3.25, 3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-3.25, -3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-3.25, 3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
    glPopMatrix();

    // Back
    glPushMatrix();
        glTranslated(0, 5, -5);
        texturedCube(10, 10, 0.2, textureIDs[3]);
    glPopMatrix();

    // Left
    glPushMatrix();
        glTranslated(5, 5, 0);
        glRotated(90, 0, 1, 0);
        glPushMatrix();
            glTranslated(-1, 0, -0.15);
            glScaled(2, 3, 1);
            texturedCube(1, 1, 0.15, textureIDs[1]);
        glPopMatrix();
        texturedCube(10, 10, 0.2, textureIDs[3]);
    glPopMatrix();

    // Floor
    glPushMatrix();
        glTranslated(0, -10, 0);
        glScaled(13, 0.2, 10);
        texturedCube(10, 0, 10, textureIDs[5]);
        //glutSolidCube(1);
    glPopMatrix();

    // Ceil
    glPushMatrix();
        glTranslated(0, 10, 0);
        glScaled(10, 0.2, 10);
        glutSolidCube(1);
    glPopMatrix();
}

GLfloat jack_mat_ambient[4]    = { 0.7f, 0.1f, 0.1f, 1.0f };
GLfloat jack_mat_diffuse[4]    = { 0.8f, 0.1f, 0.1f, 1.0f };
GLfloat jack_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat jack_shininess[1] = { 100.0f };

static void Jack() {
    unsigned int slices = 16, stacks = 16;
    setMaterial(jack_mat_ambient, jack_mat_diffuse, jack_mat_specular, jack_shininess);
    glPushMatrix();
        glRotated(45, 1, 0, 0);
        glRotated(45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45, 1, 0, 0);
        glRotated(45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45, 1, 0, 0);
        glRotated(-45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45, 1, 0, 0);
        glRotated(-45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45+90, 1, 0, 0);
        glRotated(45+90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45-90, 1, 0, 0);
        glRotated(45+90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45+90, 1, 0, 0);
        glRotated(-45-90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45-90, 1, 0, 0);
        glRotated(-45-90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    double x=0.42, y=0.32, z=0.32;

    glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, -y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, -y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, -y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, -y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();
}

double tableSized[] = {
    1, 0.33, 1, //Top
    0.1, 1, 0.1, // Leg
    0.1, 1, 0.1,
    0.1, 1, 0.1,
    0.1, 1, 0.1, // Leg
//    0.9, 0.2, 0.03, // Below top
//    0.9, 0.2, 0.03,
//    0.03, 0.2, 0.9,
//    0.03, 0.2, 0.9,// Below top
//    1, 0.05, 0.05, // Leg support
//    1, 0.05, 0.05,
//    0.05, 0.05, 1,
//    0.05, 0.05, 1, // Leg support
//    0.9, 0.05, 0.05, // Support Support
};

double tableTranslations[] = {
    0, 0.85, 0, // Top
    0.45, 0.5, 0.45,
    -0.45, 0.5, 0.45,
    0.45, 0.5, -0.45,
    -0.45, 0.5, -0.45,
//    0, 0.9, -0.45,
//    0, 0.9, 0.45,
//    0.45, 0.9, 0,
//    -0.45, 0.9, 0,
//    0, 0.1, 0.45,
//    0, 0.1, -0.45,
//    -0.45, 0.1, 0,
//    0.45, 0.1, 0,
//    0, 0.1, 0
};

GLfloat table_mat_ambient[4]    = { 0.7f, 0.2f, 0.1f, 1.0f };
GLfloat table_mat_diffuse[4]    = { 0.8f, 0.2f, 0.1f, 1.0f };
GLfloat table_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat table_shininess[1] = { 100.0f };

static void Table() {
    setMaterial(table_mat_ambient, table_mat_diffuse, table_mat_specular, table_shininess);
    for(int i=0; i<5; i++) {
        glPushMatrix();
            glTranslated(tableTranslations[i*3+0], tableTranslations[i*3+1], tableTranslations[i*3+2]);
            texturedCube(tableSized[i*3+0], tableSized[i*3+1], tableSized[i*3+2], textureIDs[4]);
        glPopMatrix();
    }
}

static void Jack1() {
    unsigned int slices = 16, stacks = 16;

    glPushMatrix();
        glRotated(45, 1, 0, 0);
        glRotated(45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45, 1, 0, 0);
        glRotated(45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45, 1, 0, 0);
        glRotated(-45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45, 1, 0, 0);
        glRotated(-45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45+90, 1, 0, 0);
        glRotated(45+90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45-90, 1, 0, 0);
        glRotated(45+90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45+90, 1, 0, 0);
        glRotated(-45-90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45-90, 1, 0, 0);
        glRotated(-45-90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    double x=0.42, y=0.32, z=0.32;

    glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, -y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, -y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, -y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, -y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();
}

/*static void Cube(double height=1, double width=1, double length=1)
{
    GLdouble cube_vertices[] = {
        -width/2, -height/2,  length/2, // 0
         width/2, -height/2,  length/2, // 1
         width/2,  height/2,  length/2, // 2
        -width/2,  height/2,  length/2, // 3
        -width/2, -height/2, -length/2, // 4
         width/2, -height/2, -length/2, // 5
         width/2,  height/2, -length/2, // 6
        -width/2,  height/2, -length/2  // 7
    };

    GLubyte cube_indices[] = {
        3, 2, 6, 7,
        7, 6, 5, 4,
        1, 5, 6, 2,
        0, 1, 2, 3,
        0, 3, 7, 4,
        4, 5, 1, 0
    };


    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_vertices);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
    glDisableClientState( GL_VERTEX_ARRAY );
}

static void Pyramid(double baseWidth=1, double baseLength=1, double height=1)
{
    glBegin(GL_TRIANGLES);
        // Front
        glVertex3f( -baseWidth/2, 0, baseLength/2 );
        glVertex3f( baseWidth/2, 0, baseLength/2 );
        glVertex3f( 0, height, 0 );

        // Back
        glVertex3f( -baseWidth/2, 0, -baseLength/2 );
        glVertex3f( 0, height, 0 );
        glVertex3f( baseWidth/2, 0, -baseLength/2);

        //right
        glVertex3f( baseWidth/2, 0, baseLength/2 );
        glVertex3f( baseWidth/2, 0, -baseLength/2);
        glVertex3f( 0, height, 0 );

        // Left
        glVertex3f( -baseWidth/2, 0, -baseLength/2 );
        glVertex3f( -baseWidth/2, 0, baseLength/2 );
        glVertex3f( 0, height, 0);
    glEnd();

    // Bottom
    glBegin(GL_QUADS);
        glVertex3f( -baseWidth/2, 0, baseLength/2 );
        glVertex3f( -baseWidth/2, 0, -baseLength/2 );
        glVertex3f( baseWidth/2, 0, -baseLength/2 );
        glVertex3f( baseWidth/2, 0, baseLength/2 );
    glEnd();
}

static void windMillStand()
{
    // Left front
    glPushMatrix();
        glTranslated(-0.3, 0, -0.3);
        glRotated(15, 1, 0, 0); //confused
        glRotated(-15, 0, 0, 1);
        Cube(2, 0.05, 0.05);
    glPopMatrix();

    // Right front
    glPushMatrix();
        glTranslated(0.3, 0, -0.3);
        glRotated(15, 1, 0, 0);
        glRotated(15, 0, 0, 1);
        Cube(2, 0.05, 0.05);
    glPopMatrix();

    // Left back
    glPushMatrix();
        glTranslated(-0.3, 0, 0.3);
        glRotated(-15, 1, 0, 0);
        glRotated(-15, 0, 0, 1);
        glTranslated(0, 0.05, 0);
        Cube(2.04, 0.05, 0.05);
    glPopMatrix();

    // Right back
    glPushMatrix();
        glTranslated(0.3, 0, 0.3);
        glRotated(-15, 1, 0, 0);
        glRotated(15, 0, 0, 1);
        glTranslated(0, 0.05, 0);
        Cube(2.04, 0.05, 0.05);
    glPopMatrix();


    // Support back
    glPushMatrix();
        glTranslated(0, -0.5, 0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();

    // Support front
    glPushMatrix();
        glTranslated(0, -0.5, -0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();

    // Support left
    glPushMatrix();
        glRotated(90, 0, 1, 0);
        glTranslated(0, -0.5, 0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();

    // Support right
    glPushMatrix();
        glRotated(-90, 0, 1, 0);
        glTranslated(0, -0.5, 0.43);
        Cube(0.05, 0.9, 0.05);
    glPopMatrix();



    // Upper Support back
    glPushMatrix();
        glTranslated(0, 0.1, 0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();

    // Upper Support front
    glPushMatrix();
        glTranslated(0, 0.1, -0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();

    // Upper Support left
    glPushMatrix();
        glRotated(90, 0, 1, 0);
        glTranslated(0, 0.1, 0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();

    // Upper Support right
    glPushMatrix();
        glRotated(-90, 0, 1, 0);
        glTranslated(0, 0.1, 0.28);
        Cube(0.05, 0.5, 0.05);
    glPopMatrix();
}


static void windMillBlade(double length=1)
{
    glPushMatrix();
        glTranslated(0, length/2, 0);
        Cube(length, 0.2, 0.05);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, length, 0);
        Pyramid(0.2, .05, 0.3);
    glPopMatrix();
}

static void windMillFan(double length=0.8, double gap=0.18)
{
    glPushMatrix();
        glRotated(0*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    glPushMatrix();
        glRotated(1*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    glPushMatrix();
        glRotated(2*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();
        glTranslated(0, 0, -0.45);
        gluCylinder(quad, 0.1, 0.2, 0.6, 32, 32);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 0, 0.15);
        gluDisk(quad, 0, 0.2, 32, 32);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 0, -0.45);
        glRotated(180, 1, 0, 0);
        gluDisk(quad, 0, 0.1, 32, 32);
    glPopMatrix();
}

static float fanRotate = 0;
static float FANSTATE = true;



static void windMill1()
{
    glPushMatrix();
        windMillStand();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 1.05, 0.3);
        glRotated(fanRotate, 0, 0, 1);
        windMillFan();
    glPopMatrix();

}


static void Room1() {
    setMaterial(room_mat_ambient, room_mat_diffuse, room_mat_specular, room_shininess);

    // Front
    glPushMatrix();
        glTranslated(0, 5, 5);
        glPushMatrix();
            glTranslated(-2, 1.5, -0.25);
            glScaled(4, 4, 0.01);
            glPushMatrix();
                glTranslated(0, 0, -0.51);
                drawFractal(fractalDepth, 0.9, -0.45, -0.4);
            glPopMatrix();
            glutSolidCube(1);
        glPopMatrix();
        texturedCube(10, 10, 0.2, textureIDs[3]);
    glPopMatrix();

    // Right
    glPushMatrix();
        glTranslated(-5, 5, 0);
        glRotated(90, 0, 1, 0);

        glPushMatrix();
            glTranslated(2.5, -5, -8);
            glScaled(20, 20, 0.1);
            texturedCube(2, 2, 0.1, textureIDs[2]);
        glPopMatrix();

        glPushMatrix();
            glTranslated(3.25, 0, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-3.25, 0, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, 3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, -3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(3.25, -3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(3.25, 3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-3.25, -3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-3.25, 3.25, 0);
            texturedCube(3.25, 3.25, 0.2, textureIDs[3]);
        glPopMatrix();
    glPopMatrix();

    // Back
    glPushMatrix();
        glTranslated(0, 5, -5);
        texturedCube(10, 10, 0.2, textureIDs[3]);
    glPopMatrix();

    // Left
    glPushMatrix();
        glTranslated(5, 5, 0);
        glRotated(90, 0, 1, 0);
        glPushMatrix();
            glTranslated(-1, 0, -0.15);
            glScaled(2, 3, 1);
            texturedCube(1, 1, 0.15, textureIDs[1]);
        glPopMatrix();
        texturedCube(10, 10, 0.2, textureIDs[3]);
    glPopMatrix();

    // Floor
    glPushMatrix();
        glTranslated(0, -10, 0);
        glScaled(13, 0.2, 10);
        texturedCube(10, 0, 10, textureIDs[5]);
        //glutSolidCube(1);
    glPopMatrix();

    // Ceil
    glPushMatrix();
        glTranslated(0, 10, 0);
        glScaled(10, 0.2, 10);
        glutSolidCube(1);
    glPopMatrix();
}

GLfloat jack_mat_ambient[4]    = { 0.7f, 0.1f, 0.1f, 1.0f };
GLfloat jack_mat_diffuse[4]    = { 0.8f, 0.1f, 0.1f, 1.0f };
GLfloat jack_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat jack_shininess[1] = { 100.0f };

static void Jack() {
    unsigned int slices = 16, stacks = 16;

    glPushMatrix();
        glRotated(45, 1, 0, 0);
        glRotated(45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45, 1, 0, 0);
        glRotated(45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45, 1, 0, 0);
        glRotated(-45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45, 1, 0, 0);
        glRotated(-45, 0, 0, 1);
        glTranslated(0, 0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45+90, 1, 0, 0);
        glRotated(45+90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45-90, 1, 0, 0);
        glRotated(45+90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(45+90, 1, 0, 0);
        glRotated(-45-90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glRotated(-45-90, 1, 0, 0);
        glRotated(-45-90, 0, 0, 1);
        glTranslated(0, -0.2*3/2, 0);
        glScaled(1, 3, 1);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    double x=0.42, y=0.32, z=0.32;

    glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, -y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, -y, z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(x, -y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-x, -y, -z);
        glutSolidSphere(0.1, slices, stacks);
    glPopMatrix();
}

double tableSized[] = {
    1, 0.33, 1, //Top
    0.1, 1, 0.1, // Leg
    0.1, 1, 0.1,
    0.1, 1, 0.1,
    0.1, 1, 0.1, // Leg
//    0.9, 0.2, 0.03, // Below top
//    0.9, 0.2, 0.03,
//    0.03, 0.2, 0.9,
//    0.03, 0.2, 0.9,// Below top
//    1, 0.05, 0.05, // Leg support
//    1, 0.05, 0.05,
//    0.05, 0.05, 1,
//    0.05, 0.05, 1, // Leg support
//    0.9, 0.05, 0.05, // Support Support
};

double tableTranslations[] = {
    0, 0.85, 0, // Top
    0.45, 0.5, 0.45,
    -0.45, 0.5, 0.45,
    0.45, 0.5, -0.45,
    -0.45, 0.5, -0.45,
//    0, 0.9, -0.45,
//    0, 0.9, 0.45,
//    0.45, 0.9, 0,
//    -0.45, 0.9, 0,
//    0, 0.1, 0.45,
//    0, 0.1, -0.45,
//    -0.45, 0.1, 0,
//    0.45, 0.1, 0,
//    0, 0.1, 0
};

GLfloat table_mat_ambient[4]    = { 0.7f, 0.2f, 0.1f, 1.0f };
GLfloat table_mat_diffuse[4]    = { 0.8f, 0.2f, 0.1f, 1.0f };
GLfloat table_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat table_shininess[1] = { 100.0f };

static void Table() {
    setMaterial(table_mat_ambient, table_mat_diffuse, table_mat_specular, table_shininess);
    for(int i=0; i<5; i++) {
        glPushMatrix();
            glTranslated(tableTranslations[i*3+0], tableTranslations[i*3+1], tableTranslations[i*3+2]);
            texturedCube(tableSized[i*3+0], tableSized[i*3+1], tableSized[i*3+2], textureIDs[4]);
        glPopMatrix();
    }
}
*/
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    camUpdate();

    if(light0)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);

    if(light1)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
        /*
    if(light0_state) {
        glEnable(GL_LIGHT0);
        glPushMatrix();
            glTranslated(light0_position[0], light0_position[1], light0_position[2]);
//            glutSolidSphere(0.5, 32, 32);
        glPopMatrix();
    }
    else glDisable(GL_LIGHT0);
    if(light1_state) {
        glEnable(GL_LIGHT1);
        glPushMatrix();
            glTranslated(light1_pos[0], light1_pos[1], light1_pos[2]);

            float angleX = light1_direction.angle(Vector(1, 0, 0)) * 180.0 / PI;
            float angleY = light1_direction.angle(Vector(0, 1, 0)) * 180.0 / PI;
            float angleZ = light1_direction.angle(Vector(0, 0, 1)) * 180.0 / PI;

            glRotated(-angleY, 1, 0, 0);
            glRotated(-90, 1, 0, 0);
//            glutSolidCone(0.5, 1, 32, 32);
        glPopMatrix();
    }
    else glDisable(GL_LIGHT1);
*/


    glPushMatrix();
        glViewport(0,0,WINDOW_WIDTH/2, WINDOW_HEIGHT);
        glTranslated(0, 0, 0);

        Room();

        glTranslated(-3, 0, 3);
        glPushMatrix();
            glTranslated(3, 2.7, -0.5);
            glScaled(1 ,1, 1);
            Jack();
        glPopMatrix();

        glPushMatrix();
            glTranslated(4.9, 3.5, -0.5);
            glRotated(190, 0,1, 0);
            glBindTexture(GL_TEXTURE_2D,2);
                glEnable(GL_TEXTURE_2D);
                glScaled(1.4,1.4,1.4);
                windMill();
                glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glTranslated(4.2, 0.1, 0);
        glScaled(3.9, 2, 2);
        Table();

    glPopMatrix();



    glPushMatrix();
        glViewport(WINDOW_WIDTH/2,0,WINDOW_WIDTH/2, WINDOW_HEIGHT);
        glTranslated(0, 0, 0);

        Room();

        glTranslated(-3, 0, 3);
        glPushMatrix();
            glColor3d(1,0,0);
            glTranslated(3, 2.7, -0.5);
            glScaled(1 ,1, 1);
            glColor3d(1,0,0);
            Jack1();
        glPopMatrix();

        glPushMatrix();
            glTranslated(4.9, 3.5, -0.5);
            glRotated(190, 0,1, 0);
                glScaled(1.4,1.4,1.4);
                windMill();
                glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        glTranslated(4.2, 0.1, 0);
        glScaled(3.9, 2, 2);
        Table();

    glPopMatrix();




    glFlush();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y) {
    switch (key)
    {
        case 27 :
            exit(0);
            break;

        // Fractal
        case 'b':
            fractalDepth += 1;
        break;
        case 'B':
            if(fractalDepth > 0) fractalDepth -= 1;
        break;

        // Camera
        case 'z':
        pos += dir * moveVelocity;
        break;

        case 'Z':
            pos -= dir * moveVelocity;
            break;

        case 'a':
            pos += head.cross(dir) * moveVelocity;
            break;

        case 'A':
            pos += dir.cross(head) * moveVelocity;
            break;

        case 'q':
            pos += head * moveVelocity;
            break;

        case 'Q':
            pos -= head * moveVelocity;
            break;

        case 'r':
            dir += head * rotationVelocity;
            break;

        case 'R':
            dir -= head * rotationVelocity;
            break;

        case 'y':
            dir -= head.cross(dir) * rotationVelocity;
            break;

        case 'Y':
            dir += head.cross(dir) * rotationVelocity;
            break;

        case 'p':
            head += dir.cross(head) * rotationVelocity;
            break;

        case 'P':
            head += head.cross(dir) * rotationVelocity;
            break;

        case 'l':
            light0 = !light0;
            break;
        case 'L':
            light1 = !light1;
            break;

        case 'f':
            fovy += 5;
            break;

        case 'F':
            fovy -= 5;
            break;

        case 'w':
            FANSTATE = !FANSTATE;
    }
    glutPostRedisplay();
}

static void idle(void)
{
    if(FANSTATE)
        fanRotate += 1;
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

GLfloat spot_direction[] = { 4, 2.34, -0.5 }; //-1.0, -5.0, 0.0

void Init()
{

    pix[0].makeCheckImage();
	pix[0].setTexture(1);

	pix[1].readBMPFile("/home/insane/STUDY/Graphics_LAB/final_project/textures/blade.jpg");
	pix[1].setTexture(2);


}

/* Program entry point */

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    // Load the texture
    textureIDs[0] = loadTexture("/home/insane/STUDY/Graphics_LAB/final_project/textures/jack.jpg");
    textureIDs[1] = loadTexture("/home/insane/STUDY/Graphics_LAB/final_project/textures/a.jpg");
    textureIDs[2] = loadTexture("/home/insane/STUDY/Graphics_LAB/final_project/textures/scenary.jpg");
    textureIDs[3] = loadTexture("/home/insane/STUDY/Graphics_LAB/final_project/textures/wood.jpeg");
    textureIDs[4] = loadTexture("/home/insane/STUDY/Graphics_LAB/final_project/textures/wall.jpg");
    textureIDs[5] = loadTexture("/home/insane/STUDY/Graphics_LAB/final_project/textures/tiles.jpeg");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);


    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 90.0); //15
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, 1.0); //10

    glutMainLoop();

    return EXIT_SUCCESS;
}
