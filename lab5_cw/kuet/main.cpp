#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

static int a=1;
static void resize(int width, int height) {
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

//static void Cube() {
//    GLdouble cube_vertices[] = {
//        -1, -1,  1, // 0
//         1, -1,  1, // 1
//         1,  1,  1, // 2
//        -1,  1,  1, // 3
//        -1, -1, -1, // 4
//         1, -1, -1, // 5
//         1,  1, -1, // 6
//        -1,  1, -1  // 7
//    };
//
//    GLubyte cube_indices[] = {
//        3, 2, 6, 7,
//        7, 6, 5, 4,
//        1, 5, 6, 2,
//        0, 1, 2, 3,
//        0, 3, 7, 4,
//        4, 5, 1, 0
//    };
//
//    glEnableClientState( GL_VERTEX_ARRAY );
//    glVertexPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_vertices);
//    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
//    glDisableClientState( GL_VERTEX_ARRAY );
//}


static void cube()
{
    double cube_vertices[] = {
        -1, -1, 1, //0
        1, -1, 1, //1
        1, 1, 1,//2
        -1, 1,1,//3
        -1, -1, -1,//4
        1, -1,-1,//5
        1, 1, -1,//6
        -1, 1, -1//71
    };

    GLubyte cube_indices[] = {
        0,1,2,3,
        1,5,6,2,
        4,0,3,7,
        5,4,7,6,
        3,2,6,7,
        4,5,1,0
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 3*sizeof(GL_DOUBLE), cube_vertices);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_vertices);
    glDisableClientState(GL_VERTEX_ARRAY);

}


static void pyramid() {
    double vertices[]= {
        0,1,0,     0,0,0,//0
        -1,0,-1,   0,0,1, //1
        1,0,-1,    0,1,0,//2
        1,0,1,     0,0,1//3
        -1,0,1,    1,0,0//4
    };

    GLubyte indices[] = {
        0,1,2,
        0,2,3,
        0,4,1,
        0,3,4
    };

    glColor3d(1, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 6*sizeof(double), vertices);
    glColorPointer(3, GL_DOUBLE, 6*sizeof(double), vertices+3);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, indices);
    glDisableClientState( GL_COLOR_ARRAY );

//    glColor3f(0,0,1);
//    glBegin(GL_QUADS);
//        glVertex3d(-1,0, 1);
//        glVertex3d( 1,0, 1);
//        glVertex3d( 1,0,-1);
//        glVertex3d(-1,0,-1);
//    glEnd();

//    glColor3d(0, 1, 0);
//
//    GLubyte base_indices[] = {
//        4, 3, 2, 1
//    };
//    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, base_indices);


    glDisableClientState(GL_VERTEX_ARRAY);


}


static void k()
{

    glPushMatrix();
        glTranslated(-3,0,-4);
        glScalef(0.5,3,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-2.2,-0.5,-4);
        glRotated(45, 0,0,1);
        glScalef(0.5,2,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-2.2,0.5,-4);
        glRotated(-45, 0,0,1);
        glScalef(0.5,2,0.5);
        glutSolidCube(1);
    glPopMatrix();

}

static void e()
{
    glPushMatrix();
        glTranslated(-3,0,-4);
        glScalef(0.5,3,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-1.9,1.2,-4);
        glScalef(2,.5,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-1.9,0,-4);
        glScalef(2,.5,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-1.9,-1.2,-4);
        glScalef(2,.5,0.5);
        glutSolidCube(1);
    glPopMatrix();

}

static void u()
{
    glPushMatrix();
        glTranslated(-3,0,-4);
        glScalef(0.5,3,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-1.9,-1.2,-4);
        glScalef(2,0.5,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-1,0,-4);
        glScalef(0.5,3,0.5);
        glutSolidCube(1);
    glPopMatrix();

}

static void t()
{
    glPushMatrix();
        glTranslated(-3,0,-4);
        glScalef(0.5,3,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-1.9,-1.2,-4);
        glScalef(2,0.5,0.5);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.5,0,-4);
        glScalef(0.5,3,0.5);
        glutSolidCube(1);
    glPopMatrix();

}
double b = 0;
static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

//    glMatrixMode (GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(60.0, (double)640/480, 0.1, 300.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();

    //gluLookAt(0, 0, -2, 0, 0, 0, 0, 1, 0);




    glPushMatrix();
        glTranslated(0, 0, -5);
        glRotated(a, 0,1,0);
        glRotated(b, 1, 0, 0);
        k();
    glPopMatrix();


    glPushMatrix();
        glTranslated(2, 0, -5);
        glRotated(a, 0,1,0);
        glRotated(b, 1, 0, 0);
        e();
    glPopMatrix();

    glPushMatrix();
        glTranslated(4.5, 0, -5);
        glRotated(a, 0,1,0);
        glRotated(b, 1, 0, 0);
        u();
    glPopMatrix();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case 'p':
            a = a+10;
            break;

        case 'o':
            b = b+10;
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
