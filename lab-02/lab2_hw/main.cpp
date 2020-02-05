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

/*
static void Cube(double height=1, double width=1, double length=1)
{
    // Bottom
    glBegin(GL_QUADS);
        glVertex3f(-width/2, -height/2, -length/2); //a
        glVertex3f(width/2, -height/2, -length/2); //d
        glVertex3f(width/2, -height/2, length/2); //c
        glVertex3f(-width/2, -height/2, length/2); //b


    // Front

        glVertex3f(-width/2, -height/2, length/2); //b
        glVertex3f(width/2, -height/2, length/2); //c
        glVertex3f(width/2, height/2, length/2); //f
        glVertex3f(-width/2, height/2, length/2); //e


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

*/

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
    // glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_vertices);
    // glColorPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_colors);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
    // glDisableClientState( GL_COLOR_ARRAY );
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

static float scaleX = 1; //.5
static float scaleY = 1;
static float scaleZ = 1;

static float roll = 90;
static float yaw = 270;
static float pitch = 180;

static float camX = 0;
static float camY = 0;
static float camZ = 5;

static float refX = 0;
static float refY = 0;
static float refZ = -5; //-5

static float upX = 0;
static float upY = 1;
static float upZ = 0;

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

//    gluLookAt(0,0,5,   0,0,0,   0,1,0);

    gluLookAt(

        camX, camY, camZ,

        refX, refY, refZ,

        upX, upY, upZ
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

    /*
    glColor3d(1, 1, 0);
    glPushMatrix();
        glTranslated(2, 0, 0);
        glRotated(5, 0, 1, 0);
        Cube(0.5, 0.5, 0.5);
    glPopMatrix();
    */

    glutSwapBuffers();
}

static void idle(void)
{
    if(FANSTATE)
        fanRotate += 10;
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


        case 'k':
            shiftZ += 0.1;
            break;
        case 'K':
            shiftZ -= 0.1;
            break;

        case 'j':
            shiftY += 0.1;
            break;
        case 'J':
            shiftY -= 0.1;
            break;

        case 'i':
            shiftX += 0.1;
            break;
        case 'I':
            shiftX -= 0.1;
            break;

        // Scaling
        case 'u':
            scaleX += 0.1;
            break;
        case 'U':
            scaleX -= 0.1;
            break;
        case 'h':
            scaleY += 0.1;
            break;
        case 'H':
            scaleY -= 0.1;
            break;
        case 'b':
            scaleZ += 0.1;
            break;
        case 'B':
            scaleZ -= 0.1;
            break;

        // Camera: Roll, Pitch, Yaw
        case 't':
            roll += 5;
            upX = cos(roll / 180 * PI);
            upY = sin(roll / 180 * PI);
            break;
        case 'T':
            roll -= 5;
            upX = cos(roll / 180 * PI);
            upY = sin(roll / 180 * PI);
            break;
        case 'g':
            yaw += 5;
            refX = 5*cos(yaw / 180 * PI);
            refZ = 5*(1+sin(yaw / 180 * PI));
            //std::cout << refX << " " << refZ << std::endl;
            break;
        case 'G':
            yaw -= 5;
            refX = 5*cos(yaw / 180 * PI);
            refZ = 5*(1+sin(yaw / 180 * PI));
            //std::cout << refX << " " << refZ << std::endl;
            break;
        case 'v':
            pitch += 5;
            refY = 5*sin(pitch / 180 * PI);
            refZ = 5*(1+cos(pitch / 180 * PI));
            break;
        case 'V':
            pitch -= 5;
            refY = 5*sin(pitch / 180 * PI);
            refZ = 5*(1+cos(pitch / 180 * PI));
            break;

        // Fan control
        case 'w':
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
