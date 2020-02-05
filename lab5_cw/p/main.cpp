
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

static int slices = 16;
static int stacks = 16;

/* GLUT callback Handlers */
float xx=0;
float yy=0;
float zz=0;
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

float x_col=0.0,y_col=0.0,z_col=0.0,M=0.0;

void kuet()
{
    glColor3d(1,0,0);
    glBegin(GL_QUADS);
        //front
        glVertex3f(1,0,1);
        glVertex3f(2,0,1);
        glVertex3f(2,5,1);
        glVertex3f(1,5,1);

        glColor3d(0, 1, 0);
        //left
        glVertex3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(1,5,1);
        glVertex3f(1,5,0);
        //back
        glVertex3f(2,0,0);
        glVertex3f(1,0,0);
        glVertex3f(1,5,0);
        glVertex3f(2,5,0);
        //right
        glVertex3f(2,0,1);
        glVertex3f(2,0,0);
        glVertex3f(2,5,0);
        glVertex3f(2,5,1);
        //top
        glVertex3f(1,5,1);
        glVertex3f(2,5,1);
        glVertex3f(2,5,0);
        glVertex3f(1,5,0);
        //bottom
        glVertex3f(1,0,0);
        glVertex3f(2,0,0);
        glVertex3f(2,0,1);
        glVertex3f(1,0,1);

    glEnd();
//    glScaled(1,4,1);
//    glutSolidCube(2);
}


static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glTranslated(-1,0,-5);
        glTranslated(x_col+0,0,0);
        glRotated(xx,1,0,0);
        glRotated(yy,0,1,0);
        glRotated(zz,0,0,1);
        glScalef(M+.1,M+.1,M+.1);
        glPushMatrix();
        glTranslated(0,-2,0);
        glScaled(1,2.5,1);
        kuet();
        glPopMatrix();

        glPushMatrix();
            glTranslated(1,5,0);
            glScaled(1,1.5,1);
            glRotated(-45,0,0,1);
            kuet();
        glPopMatrix();

        glPushMatrix();
        glTranslated(4,-3,0);
        glScaled(1,1.5,1);
        glRotated(45,0,0,1);
        kuet();
        glPopMatrix();
        //u starts

        glPushMatrix();
        glTranslated(6,-2,0);
        glScaled(1,2.5,1);
        kuet();
        glPopMatrix();

        glPushMatrix();
        glTranslated(13,-4,0);
        glRotated(90,0,0,1);
        kuet();
        glPopMatrix();

        glPushMatrix();
        glTranslated(12,-2,0);
        glScaled(1,2.5,1);
        kuet();
        glPopMatrix();

        //e starts

        glPushMatrix();
        glTranslated(15,-2,0);
        glScaled(1,2.5,1);
        kuet();
        glPopMatrix();

        glPushMatrix();
        glTranslated(22,-3,0);
        glRotated(90,0,0,1);
        kuet();
        glPopMatrix();

        glPushMatrix();
        glTranslated(22,2,0);
        glRotated(90,0,0,1);
        kuet();
        glPopMatrix();

        glPushMatrix();
        glTranslated(22,8.5,0);
        glRotated(90,0,0,1);
        kuet();
        glPopMatrix();

        //t starts

        glPushMatrix();
        glTranslated(24,-2,0);
        glScaled(1,2.5,1);
        kuet();
        glPopMatrix();

        glPushMatrix();
        glTranslated(28,8.5,0);
        glRotated(90,0,0,1);
        kuet();
        glPopMatrix();


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

        case 'x': //anti-clock wise
            xx+=2.5;
            break;
        case 'y':
            yy+=2.5;
            break;
        case 'z':
            zz+=2.5;
            break;
        case '1':
            x_col+=.1;
            break;
        case '2':
            y_col+=.1;
            break;
        case '3':
            z_col+=.1;
            break;
        case 'M':
            M+=.005;
            break;
        case 'm':
            M-=.005;
            break;
//        case '+':
//            slices++;
//            stacks++;
//            break;
//
//        case '-':
//            if (slices>3 && stacks>3)
//            {
//                slices--;
//                stacks--;
//            }
//            break;
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

    glutCreateWindow("MK");

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
