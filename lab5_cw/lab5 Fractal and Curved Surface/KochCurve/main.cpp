/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<math.h>


GLfloat xComplexMin = -0.25, xComplexMax = 1.25;
GLfloat yComplexMin = -0.75, yComplexMax = 0.75;

GLfloat oldx = -0.7, oldy = 0.5;
GLfloat lftx = oldx - 0.1, lfty = oldy - 0.1;
GLfloat rhtx = oldx + 0.1, rhty = oldy + 0.1;

int Depth=6;

void drawkoch(GLfloat dir, GLfloat len, GLint iter) {
	GLdouble dirRad = 0.0174533 * dir;
	GLfloat newX = oldx + len * cos(dirRad);
	GLfloat newY = oldy + len * sin(dirRad);

	// This part is required to modify viewing window
	lftx = (newX - 0.1)<lftx ? newX - 0.1 : lftx;
	lfty = (newY - 0.1)<lfty ? newY - 0.1 : lfty;
	rhtx = (newX + 0.1)>rhtx? newX + 0.1:rhtx;
	rhty = (newY + 0.1)>rhty ? newY + 0.1 : rhty;


	if (iter == 0) {
		glVertex2f(oldx, oldy);
		glVertex2f(newX, newY);
		oldx = newX;
		oldy = newY;
	}
	else {
		iter--;
		//draw the four parts of the side _/\_

		drawkoch(dir, len, iter);
		dir += 60.0;
		drawkoch(dir, len, iter);
		dir -= 120.0;
		drawkoch(dir, len, iter);
		dir += 60.0;
		drawkoch(dir, len, iter);

		/*drawkoch(dir, len, iter);
		dir += 90.0;
		drawkoch(dir, len, iter);
		dir -= 90.0;
		drawkoch(dir, len, iter);
		dir -= 90.0;
		drawkoch(dir, len, iter);
		dir += 90.0;
		drawkoch(dir, len, iter);*/
	}
}

void KochCurve(int depth) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // make it red


	drawkoch(0.0, 0.05, depth);
	drawkoch(-120.0, 0.05, depth);
	drawkoch(+120.0, 0.05, depth);

	/*drawkoch(0.0, 0.05, depth);
	drawkoch(-90.0, 0.05, depth);
	drawkoch(-180.0, 0.05, depth);
	drawkoch(90.0, 0.05, depth);*/
	glEnd();
	//glFlush();
}

static void resize(int width, int height)
{

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(lftx, rhtx, lfty, rhty);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        KochCurve(Depth);
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

    glutCreateWindow("Koch Curve");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    glClearColor(1,1,1,1);

    glutMainLoop();

    return EXIT_SUCCESS;
}
