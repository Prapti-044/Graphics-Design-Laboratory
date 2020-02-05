#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "RGBpixmap.cpp"

#define PI 3.1415926535897932384626433832795
#define MaximumTheta 1.0
#define MinimumTheta 0.0009
#define MaximumRadious 10.5
#define MinimumRadious .9

//DATATYPE DECLERATION
GLfloat IncrementTheta = 0.05;
GLint WindowSizX=640,WindowSizY=480;
GLfloat EyePosition_X=2.0,EyePosition_Y=2,EyePosition_Z= 8.0;
GLfloat Radious=3.3,ProRadious=3.0,InitialTheta1=0.716,InitialTheta2=0.403;
GLfloat Center_X=0.0,Center_Y=0.0,Center_Z=-1.0;
GLfloat Up_X=0.0,Up_Y=1.0,Up_Z=0.0;
enum { ClockWise,AntiClockWise };
enum {Theta1,Theta2};
float Near=.05, Far=10.0,fova = 20.0;
GLint direction = AntiClockWise;
GLint PressMovenent=Theta1;

RGBpixmap pix[7];

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

static GLfloat windmill_mat_ambient[4]    = { 0.0f, 0.1f, 0.7f, 1.0f }; //0.1f, 0.1f, 0.7f, 1.0f
static GLfloat windmill_mat_diffuse[4]    = { 0.0f, 0.1f, 0.7f, 1.0f }; //0.1f, 0.1f, 0.7f, 1.0f
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


static GLfloat table_mat_ambient[4]    = { 0.4f, 0.3f, 0.2f, 1.0f }; //0.5f, 0.1f, 0.4f, 1.0f
static GLfloat table_mat_diffuse[4]    = { 0.5f, 0.3f, 0.2f, 1.0f }; //0.5f, 0.1f, 0.4f, 1.0f
static GLfloat table_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat table_shininess[1] = { 100.0f };

static void Table()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,   table_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   table_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  table_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, table_shininess);

    glPushMatrix();
        glTranslated(0, 1, 0);
        glScaled(1, 0.12, 1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.45, 0.5, 0.45);
        glScaled(0.1, 1, 0.1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-0.45, 0.5, 0.45);
        glScaled(0.1, 1, 0.1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.45, 0.5, -0.45);
        glScaled(0.1, 1, 0.1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-0.45, 0.5, -0.45);
        glScaled(0.1, 1, 0.1);
        glutSolidCube(1);
    glPopMatrix();

}

static GLfloat jack_mat_ambient[4]    = { 0.9f, 0.4f, 0.5f, 1.0f }; //0.7f, 0.1f, 0.1f, 1.0f
static GLfloat jack_mat_diffuse[4]    = { 0.9f, 0.4f, 0.5f, 1.0f }; //0.7f, 0.1f, 0.1f, 1.0f
static GLfloat jack_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat jack_shininess[1] = { 100.0f };

static void Jack(int slices=32, int stacks=32)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,   jack_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   jack_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  jack_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, jack_shininess);

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

static GLfloat room_mat_ambient[4]    = { 0.4f, 0.6f, 0.5f, 1.0f };
static GLfloat room_mat_diffuse[4]    = { 0.5f, 0.6f, 0.5f, 1.0f };
static GLfloat room_mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat room_shininess[1] = { 100.0f };

static void Room()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,   room_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   room_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  room_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, room_shininess);

    // Front
    glPushMatrix();
        glTranslated(0, 5, 5);
        glScaled(10, 10, 0.2);
        glutSolidCube(1);
    glPopMatrix();

    // Left
    glPushMatrix();
        glTranslated(-5, 5, 0);

        glRotated(90, 0, 1, 0);

        glPushMatrix();
            glTranslated(0, 0, 0.15);
            glScaled(3, 3, 0.15);
            glutSolidCube(1);
        glPopMatrix();

        glScaled(10, 10, 0.2);

        glBindTexture(GL_TEXTURE_2D, 6);
        glEnable(GL_TEXTURE_2D);


        glutSolidCube(1);

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    // Back
    glPushMatrix();
        glTranslated(0, 5, -5);
        glScaled(10, 10, 0.2);

        glBindTexture(GL_TEXTURE_2D, 6);
        glEnable(GL_TEXTURE_2D);

        glutSolidCube(1);


        glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    // Right
    glPushMatrix();
        glTranslated(5, 5, 0);

        glRotated(90, 0, 1, 0);
        glScaled(3.25, 3.25, 0.2);

        glBindTexture(GL_TEXTURE_2D, 7);
        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
            glScaled(5, 5, 0.2);
            glutSolidCube(1);

        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        glPushMatrix();
            glTranslated(1, 0, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1, 0, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, 1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, -1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(1, -1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(1, 1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(1, -1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1, 1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1, -1, 0);
            glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();

    // Floor
    glPushMatrix();
        glScaled(10, 0.2, 10);
        glBindTexture(GL_TEXTURE_2D,5);
        glEnable(GL_TEXTURE_2D);
        glutSolidCube(1);
        glDisable(GL_TEXTURE_2D);


    glPopMatrix();

    // Roof
    glPushMatrix();
        glTranslated(0, 10, 0);
        glScaled(10, 0.2, 10);
        glutSolidCube(1);
    glPopMatrix();

}

static void Room1()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT,   room_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   room_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  room_mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, room_shininess);

    // Front
    glPushMatrix();
        glTranslated(0, 5, 5);
        glScaled(10, 10, 0.2);
        glutSolidCube(1);
    glPopMatrix();

    // Left
    glPushMatrix();
        glTranslated(-5, 5, 0);

        glRotated(90, 0, 1, 0);

        glPushMatrix();
            glTranslated(0, 0, 0.15);
            glScaled(3, 3, 0.15);
            glutSolidCube(1);
        glPopMatrix();

        glScaled(10, 10, 0.2);

        glutSolidCube(1);

    glPopMatrix();

    // Back
    glPushMatrix();
        glTranslated(0, 5, -5);
        glScaled(10, 10, 0.2);

        glutSolidCube(1);


    glPopMatrix();

    // Right
    glPushMatrix();
        glTranslated(5, 5, 0);

        glRotated(90, 0, 1, 0);
        glScaled(3.25, 3.25, 0.2);


        glPushMatrix();
            glScaled(5, 5, 0.2);
            glutSolidCube(1);

        glPopMatrix();

        glPushMatrix();
            glTranslated(1, 0, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1, 0, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, 1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(0, -1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(1, -1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(1, 1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(1, -1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1, 1, 0);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix();
            glTranslated(-1, -1, 0);
            glutSolidCube(1);
        glPopMatrix();

    glPopMatrix();

    // Floor
    glPushMatrix();
        glScaled(10, 0.2, 10);
        glutSolidCube(1);
    glPopMatrix();

    // Roof
    glPushMatrix();
        glTranslated(0, 10, 0);
        glScaled(10, 0.2, 10);
        glutSolidCube(1);
    glPopMatrix();

}
static float roll = 90;
static float yaw = 270;
static float pitch = 180;

static float camX = 0;
static float camY = 0;
static float camZ = 5;

static float refX = 0;
static float refY = 0;
static float refZ = -5;

static float upX = 0;
static float upY = 1;
static float upZ = 0;

#define PI 3.14159

static bool light0 = true;
static bool light1 = true;


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup perspective view
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)640/480, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        camX, camY, camZ,
        refX, refY, refZ,
        upX, upY, upZ
    );

    if(light0)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);

    if(light1)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);


    glPushMatrix();
        glViewport(0,0,420, 640);
        glTranslated(0, -4, 0);

        glPushMatrix();

            glPushMatrix();
                glTranslated(-1, 3.6, 0);
                glScaled(1, 1, 1);
                glBindTexture(GL_TEXTURE_2D,3);
                glEnable(GL_TEXTURE_2D);
                Jack();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            glPushMatrix();
                glTranslated(0.6, 4.2, 0);
                glScaled(1, 1, 1);
                glBindTexture(GL_TEXTURE_2D,4);
                glEnable(GL_TEXTURE_2D);
                windMill();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            glTranslated(0, 0, -1);
            glScaled(4.5, 3, 3);
            glBindTexture(GL_TEXTURE_2D,2);
            glEnable(GL_TEXTURE_2D);
            Table();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();

        Room();
    glPopMatrix();


    glPushMatrix();
        glViewport(320,0,420, 640);
        glTranslated(0, -4, 0);

        glPushMatrix();

            glPushMatrix();
                glTranslated(-1, 3.6, 0);
                glScaled(1, 1, 1);
                Jack();
            glPopMatrix();

            glPushMatrix();
                glTranslated(0.6, 4.2, 0);
                glScaled(1, 1, 1);
                windMill();
            glPopMatrix();

            glTranslated(0, 0, -1);
            glScaled(4.5, 3, 3);
            Table();
        glPopMatrix();

        Room1();
    glPopMatrix();


    glutSwapBuffers();
}

static void idle(void)
{
    if(FANSTATE)
        fanRotate += 1;
    glutPostRedisplay();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        // Camera: Roll, Pitch, Yaw
        case 'r':
            roll += 5;
            upX = cos(roll / 180 * PI);
            upY = sin(roll / 180 * PI);
            break;
        case 'R':
            roll -= 5;
            upX = cos(roll / 180 * PI);
            upY = sin(roll / 180 * PI);
            break;
        case 'y':
            yaw += 5;
            refX = 5*cos(yaw / 180 * PI);
            refZ = 5*(1+sin(yaw / 180 * PI));
            //std::cout << refX << " " << refZ << std::endl;
            break;
        case 'Y':
            yaw -= 5;
            refX = 5*cos(yaw / 180 * PI);
            refZ = 5*(1+sin(yaw / 180 * PI));
            //std::cout << refX << " " << refZ << std::endl;
            break;
        case 'p':
            pitch += 5;
            refY = 5*sin(pitch / 180 * PI);
            refZ = 5*(1+cos(pitch / 180 * PI));
            break;
        case 'P':
            pitch -= 5;
            refY = 5*sin(pitch / 180 * PI);
            refZ = 5*(1+cos(pitch / 180 * PI));
            break;

        // Lights
        case 'l':
            light0 = !light0;
            break;
        case 'L':
            light1 = !light1;
            break;

        // Fan control
        case 'w':
            FANSTATE = !FANSTATE;
    }
    glutPostRedisplay();
}

const GLfloat light0_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f }; //0.1f, 0.1f, 0.1f, 1.0f
const GLfloat light0_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; // 1.0f, 1.0f, 1.0f, 1.0f
const GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light0_position[] = { 2.0f, 5.0f, 5.0f, 0.3f };

const GLfloat light1_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f }; //0.1f, 0.1f, 0.1f, 1.0f
const GLfloat light1_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f }; //1.0f, 1.0f, 1.0f, 1.0f
const GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light1_position[] = { 1.0f, 6.0f, -5.0f, 1.0f };  //0.0f, 3.0f, -5.0f, 1.0f

GLfloat spot_direction[] = { -1, -7, 0.0 }; //-1.0, -5.0, 0.0


void Init()
{

    pix[0].makeCheckImage();
	pix[0].setTexture(1);

	pix[1].readBMPFile("/home/insane/STUDY/Graphics_LAB/lab5_hw/hw_lab4/wood.bmp");
	pix[1].setTexture(2);

	pix[2].readBMPFile("/home/insane/STUDY/Graphics_LAB/lab5_hw/hw_lab4/jack.bmp");
	pix[2].setTexture(3);

	pix[3].readBMPFile("/home/insane/STUDY/Graphics_LAB/lab5_hw/hw_lab4/blade.bmp");
	pix[3].setTexture(4);

    pix[4].readBMPFile("/home/insane/STUDY/Graphics_LAB/lab5_hw/hw_lab4/tiles.bmp");
	pix[4].setTexture(5);

	pix[4].readBMPFile("/home/insane/STUDY/Graphics_LAB/lab5_hw/hw_lab4/wall.bmp");
	pix[4].setTexture(6);

	pix[5].readBMPFile("/home/insane/STUDY/Graphics_LAB/lab5_hw/hw_lab4/scenaryo.bmp");
	pix[5].setTexture(7);



}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

//    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(0,0,0,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, 15.0); //15
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, 10.0); //10

    Init();

    glutMainLoop();

    return EXIT_SUCCESS;
}
