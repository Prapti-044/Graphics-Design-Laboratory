#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265359


static GLdouble cube_vertices[8*3] = {
    -1, -1, 1,
    1, -1, 1,
    1, 1, 1,
    -1, 1, 1,
    -1, -1, -1,
    1, -1, -1,
    1, 1, -1,
    -1, 1, -1
};
static GLdouble cube_colors[8*3] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    1, 1, 0,
    1, 0, 1,
    0, 1, 1,
    0, 0, 0,
    0.5, 0.5, 0.5
};
static GLubyte cube_indices[24] = {
    0, 1, 2, 3,
    1, 5, 6, 2,
    7, 6, 5, 4,
    0, 3, 7, 4,
    3, 2, 6, 7,
    4, 5, 1, 0
};
static void cube()
{
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_vertices);
    glColorPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_colors);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}

/// think camera is in polar co-ordinate system. i.e R theta
/// initially camera 10 units from origin along +ve Z axis.
/// 0° = +Z axis. 90° = +X axis and so on.
static GLdouble camRadius = 10;
static GLdouble camTheta = 0;
/// height - camera pos Y component -  simply controlled by keyboard
static GLdouble camHeight = 0;

/// initially up vector points vertically up (+ve Y axis)
static GLdouble camRoll = 90;

/// camera 'looks' at this point (controlled by keyboard)
/// regardless of it's position and orientation
static GLdouble camCenterX = 0;
static GLdouble camCenterY = 0;
static GLdouble camCenterZ = 0;

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void cameraMagic()
{
    GLdouble angleInRadians = camTheta * PI / 180.0;
    /// calculation of camera position - simply just polar to Cartesian
    /// I assume 0° to be somewhere along +Z axis
    /// and 90° to be along +X axis
    /// hence the following calculations
    GLdouble cosAngle = cos(angleInRadians); /// we need these values...
    GLdouble sinAngle = sin(angleInRadians); /// so I saved them here
    GLdouble camZ = camRadius * cosAngle;
    GLdouble camX = camRadius * sinAngle;
    GLdouble camY = camHeight; /// just for convenience :)

    /// now Up vector calculation for some Rock n 'Roll' action
    GLdouble upX = 1, upY = 0, upZ = 0; /// initially up = right lol
    GLdouble tX, tY, tZ; /// temporary variables
    /// remember - we're taking roll angle from +ve X direction
    /// so Up vector is found by rotating right vector [1,0,0] by roll angle about Z axis
    GLdouble rollInRadians = camRoll * PI / 180.0;
    tX = cos(rollInRadians) * upX - sin(rollInRadians) * upY; /// simple rotation formula
    tY = sin(rollInRadians) * upX + cos(rollInRadians) * upY; /// about Z axis
    upX = tX, upY = tY;
    /// so far this works if camera is positioned somewhere along Z axis
    /// because when we start 'yaw'ing, roll is affected
    /// so we have to take yaw angle (i.e Theta for cam position) into account
    /// we simply rotate the up vector by yaw angle (Theta) about Y axis
    tX = cosAngle * upX + sinAngle * upZ; /// simple rotation formula
    tZ = -sinAngle * upX + cosAngle * upZ; /// about Y axis
    upX = tX, upZ = tZ;

    /// let OpenGL handle the rest ;) Cheers!
    gluLookAt(camX, camY, camZ, camCenterX, camCenterY, camCenterZ, upX, upY, upZ);
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    /// all the camera calculations happen here
    cameraMagic();

    glPushMatrix();
        /// keep object at origin. a.k.a translate 0, 0, 0
        /// translate, rotate, ja iccha tai koro ekhane
        cube();
    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        /// R, Theta and height control
        /// basically eye position control
    case 'w':
        camRadius -= 1;
        break;
    case 's':
        camRadius += 1;
        break;
    case 'a':
        camTheta -= 5;
        break;
    case 'd':
        camTheta += 5;
        break;
    case 'h':
        camHeight += 1;
        break;
    case 'H':
        camHeight -= 1;
        break;

        /// center x y z control
    case 'x':
        camCenterX+=1;
        break;
    case 'y':
        camCenterY+=1;
        break;
    case 'z':
        camCenterZ+=1;
        break;
    case 'X':
        camCenterX-=1;
        break;
    case 'Y':
        camCenterY-=1;
        break;
    case 'Z':
        camCenterZ-=1;
        break;

        /// camera roll control
        /// basically up vector control
    case 'e':
        camRoll -= 5;
        break;
    case 'q':
        camRoll += 5;
        break;

    case 27 :
        exit(0);
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    printf("Center xyz control: xyz, shift+xyz\nRadius control: ws\nAngle or theta control: ad\nRoll control: qe\n");
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
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
