#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <iostream>

static void ownScalef(double sx, double sy, double sz) {

    glMatrixMode(GL_MODELVIEW);

    GLfloat transformationMat[] = {
        sx, 0,  0, 0,
        0, sy,  0, 0,
        0,  0, sz, 0,
        0,  0,  0, 1
    };

    glMultMatrixf(transformationMat);
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


static void Cube(double height=1, double width=1, double length=1)
{
    // Bottom
    glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, -length/2); //a
        glVertex3f(width/2, -height/2, -length/2); //d
        glVertex3f(width/2, -height/2, length/2); //c
        glVertex3f(-width/2, -height/2, length/2); //b
    glEnd();

    // Front
    glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, length/2); //b
        glVertex3f(width/2, -height/2, length/2); //c
        glVertex3f(width/2, height/2, length/2); //f
        glVertex3f(-width/2, height/2, length/2); //e
    glEnd();

    // Right
    glBegin(GL_QUADS);
        glVertex3f(width/2, -height/2, length/2); //c
        glVertex3f(width/2, -height/2, -length/2); //d
        glVertex3f(width/2, height/2, -length/2); //g
        glVertex3f(width/2, height/2, length/2); //f
    glEnd();

    // Back
    glBegin(GL_QUADS);
        glVertex3f(-width/2, height/2, -length/2); //h
        glVertex3f(width/2, height/2, -length/2); //g
        glVertex3f(width/2, -height/2, -length/2); //d
        glVertex3f(-width/2, -height/2, -length/2); //a
    glEnd();

    // Left
    glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, -length/2); //a
        glVertex3f(-width/2, -height/2, length/2); //b
        glVertex3f(-width/2, height/2, length/2); //e
        glVertex3f(-width/2, height/2, -length/2); //h
    glEnd();

    // Top
    glBegin(GL_QUADS);
        glVertex3f(-width/2, height/2, length/2); //e
        glVertex3f(width/2, height/2, length/2); //f
        glVertex3f(width/2, height/2, -length/2); //g
        glVertex3f(-width/2, height/2, -length/2); //h
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
    glColor3d((double)236/255, (double)74/255, (double)43/255);
    glPushMatrix();
        glRotated(0*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    glColor3d((double)230/255, (double)202/255, (double)13/255);
    glPushMatrix();
        glRotated(1*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    glColor3d((double)54/255, (double)143/255, (double)30/255);
    glPushMatrix();
        glRotated(2*120, 0, 0, 1);
        glTranslated(0, gap, 0.1);
        windMillBlade(length);
    glPopMatrix();

    GLUquadric* quad = gluNewQuadric();

    glColor3d((double)8/255, (double)0/255, (double)187/255);
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
static void windMill()
{
    glColor3d((double)157/255, (double)150/255, (double)101/255);
    glPushMatrix();
        windMillStand();
    glPopMatrix();

    glColor3d(0, 0, 1);
    glPushMatrix();
        glTranslated(0, 1.05, 0.3);
        glRotated(fanRotate, 0, 0, 1);
        windMillFan();
    glPopMatrix();
}


static float rotateX = 0;
static float rotateY = 0;
static float rotateZ = 0;

static float shiftX = 0;
static float shiftY = 0;
static float shiftZ = 0;

static float scaleX = 0.5;
static float scaleY = 0.5;
static float scaleZ = 0.5;

static float roll = 0;
static float pitch = 0;
static float yaw = 0;

static float camX = 0;
static float camY = 0;
static float camZ = 5;

#define PI 3.1416

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup perspective view
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)640/480, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(roll > 360)
        roll = roll - 360;
    else if(roll < 0)
        roll = 360 + roll;

//    gluLookAt(0,0,6,   0,0,0,   0,1,0);

    gluLookAt(
        camX, camY, camZ,

        camX + sin(yaw / 180 * PI),
        camY + sin(pitch / 180 * PI),
        camZ - sin(yaw / 180 * PI) - cos(pitch / 180 * PI),

        0, camY + cos(pitch / 180 * PI), camZ + sin(pitch / 180 * PI)
    );


	glColor3d(1, 0, 0);
    glPushMatrix();
        glTranslated(shiftX, shiftY, shiftZ);
        glRotated(rotateX, 1, 0, 0);
        glRotated(rotateY, 0, 1, 0);
        glRotated(rotateZ, 0, 0, 1);
        // glScaled(0.5, 0.5, 0.5);
        ownScalef(scaleX, scaleY, scaleZ);
        windMill();
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
        //case 'q':
            exit(0);
            break;

        case 'x':
            rotateX += 5;
            break;
        case 'X':
            rotateX -= 5;
            break;
        case 'y':
            rotateY += 5;
            break;
        case 'Y':
            rotateY -= 5;
            break;
        case 'z':
            rotateZ += 5;
            break;
        case 'Z':
            rotateZ -= 5;
            break;


        case 'e':
            shiftZ += 0.1;
            break;
        case 'q':
            shiftZ -= 0.1;
            break;

        case 'w':
            shiftY += 0.1;
            break;
        case 's':
            shiftY -= 0.1;
            break;

        case 'd':
            shiftX += 0.1;
            break;
        case 'a':
            shiftX -= 0.1;
            break;

        // Scaling
        case 'b':
            scaleX += 0.1;
            break;
        case 'B':
            scaleX -= 0.1;
            break;
        case 'n':
            scaleY += 0.1;
            break;
        case 'N':
            scaleY -= 0.1;
            break;
        case 'm':
            scaleZ += 0.1;
            break;
        case 'M':
            scaleZ -= 0.1;
            break;

        // Camera: Roll, Pitch, Yaw
        case 'i':
            roll += 5;
            break;
        case 'I':
            roll -= 5;
            break;
        case 'o':
            pitch += 5;
            break;
        case 'O':
            pitch -= 5;
            break;
        case 'p':
            yaw += 5;
            break;
        case 'P':
            yaw -= 5;
            break;

        // Fan control
        case 'v':
            FANSTATE = !FANSTATE;

    }

    glutPostRedisplay();
}

/* Program entry point */

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

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutMainLoop();

    return EXIT_SUCCESS;
}
