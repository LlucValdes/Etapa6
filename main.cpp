// Etapa1.cpp
// Fichero principal 
////////////////////////////////////////////////////

#include <iostream>
using namespace std;
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>


#include "TextureLoader.h"

int W_WIDTH = 1000; // Tama�o incial de la ventana
int W_HEIGHT = 500;
double ratio = W_WIDTH / W_HEIGHT;

int contador = 0;
unsigned int alturaImg;
unsigned int anchoImg;
GLfloat angle = 0.0;

//Aceleracion
		//keyboard controls
bool leftHeld = false, rightHeld = false;
bool upHeld = false, downHeld = false;

//vehicle controls
float steering = 0; //-1 is left, 0 is center, 1 is right
float throttle = 0; //0 is coasting, 1 is full throttle
float brakes = 0; //0 is no brakes, 1 is full brakes

float posCar[3] = {40.0, 0.0, 40.0};
float dirCotxo[3] = {1.0, 0.0, 0.0};
float perpendicular[3] = { 0.0, 0.0, 1.0 };
float normal[3] = {0.0, 1, 0.0};
float *aux;
float angleCotxo = 0;


float farola1[4] = { 0.0, 0.0, 0.0, 1.0};
float farola2[4] = { 0.0, 0.0, 0.0, 1.0 };


GLfloat dir[] = { 0.0, -1.0, 0.0 };

bool f1 = true;
bool camara = false;
bool nit = false;



float a = 0;

unsigned char* texture;

void dibuixarFarola(float x, float y) {

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	// Dibuixar base
	glScalef(1.0, 0.5, 1.0);
	glTranslatef(x, -1.5f, y);
	glutSolidCube(1.0);
	glPopMatrix();
	// Dibuixar cos
	glColor3f(1.0f, 0.2f, 1.0f);
	glPushMatrix();
	glTranslatef(x, 0.5f, y);
	glScalef(0.4, 2.5, 0.4);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2f, 0.2f, 1.0f);
	glTranslatef(0.8+x, 1.9, y);
	glScalef(2.0, 0.4, 0.4);
	glutSolidCube(1.0);
	glPopMatrix();
	//dibuixar bombilla
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(1.5+x, 1.6,y);
	if (f1) {
		farola1[0] = 1.5 + x;
		farola1[1] = 1.5;
		farola1[2] = y;
		f1 = false;
	}
	else {
		farola2[0] = 1.5 + x;
		farola2[1] = 1.5;
		farola2[2] = y;
		f1 = true;
	}
	

	

	glutSolidSphere(0.2f, 30, 30);
	glPopMatrix();
}

void drawCar() {
	glPushMatrix();
	glTranslatef(posCar[0], posCar[1], posCar[2]);
	glRotatef(angleCotxo, 0, 1, 0);
	glutSolidCube(1);
	glutWireCube(1);
	glPopMatrix();
	
	glPushMatrix();
	glBegin(GL_LINE);
	glVertex3f(posCar[0] + perpendicular[0] / 1.5, -0.5, posCar[2] + perpendicular[2] / 1.5);
	glVertex3f(posCar[0] - perpendicular[0] * 1.1, -0.5, posCar[2] - perpendicular[2] * 1.1);
	glEnd();
	glPopMatrix();

	
	glPushMatrix();
	
	glTranslatef(posCar[0]+perpendicular[0]/1.5, -0.5, posCar[2] + perpendicular[2]/1.5);
	
	if (steering == 1) {
		
		glRotatef(angleCotxo-20, 0, 1, 0);
	}
	else if (steering == -1){
		
		glRotatef(angleCotxo+20, 0, 1, 0);
	}
	else {
		
		glRotatef(angleCotxo, 0, 1, 0);
	}
	
	glutSolidCylinder(0.5, 0.4, 100, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(posCar[0] - perpendicular[0]*1.1 , -0.5, posCar[2] - perpendicular[2] * 1.1);
	if (steering == 1) {
		glRotatef(angleCotxo - 20, 0, 1, 0);
	}
	else if (steering == -1) {
		glRotatef(angleCotxo + 20, 0, 1, 0);
	}
	else {
		glRotatef(angleCotxo, 0, 1, 0);
	}

	glutSolidCylinder(0.5, 0.4, 100, 1);
	glPopMatrix();

	
}



void multiplicarVectors(float A[3], float B[3]) {
	
	perpendicular[0] = (A[1] * B[2] - A[2] * B[1]);
	perpendicular[1] = (A[0] * B[2] - A[2] * B[0]);
	perpendicular[2] = (A[0] * B[1] - A[1] * B[0]);
	float modul = pow(pow(perpendicular[0],2)+ pow(perpendicular[1], 2) + pow(perpendicular[2], 2), 0.5);
	perpendicular[0] = perpendicular[0] / modul;
	perpendicular[1] = perpendicular[1] / modul;
	perpendicular[2] = perpendicular[2] / modul;

}

void normalitzarVector(void) {

	float modul = pow(pow(dirCotxo[0], 2) + pow(dirCotxo[1], 2) + pow(dirCotxo[2], 2), 0.5);
	dirCotxo[0] = dirCotxo[0] / modul;
	dirCotxo[1] = dirCotxo[1] / modul;
	dirCotxo[2] = dirCotxo[2] / modul;
}

// Funci�n que visualiza la escena OpenGL
void Display(void)
{



	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (camara) {
		gluLookAt(posCar[0] - (dirCotxo[0] ), 25, posCar[2] - (dirCotxo[2]),
			posCar[0], posCar[1] + 0.5, posCar[2],
			0.0f, 1.0f, 0.0f);
	}
	else {
		gluLookAt(posCar[0] - (dirCotxo[0] * 4), 1.5, posCar[2] - (dirCotxo[2] * 4),
			posCar[0], posCar[1] + 0.5, posCar[2],
			0.0f, 1.0f, 0.0f);
	}

	



	if (nit) {
		GLfloat light2_diffuse[] = { 1.0, 1.0, 0.0, 1.0 }; /* bright white */
		GLfloat light2_specular[] = { 1.0, 1.0, 0.0, 1.0 };
		GLfloat cut[] = {87};

		glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
		glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);

		glLightfv(GL_LIGHT2, GL_POSITION, farola1);
		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir);
		glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, cut);
		glEnable(GL_LIGHT2);

		glLightfv(GL_LIGHT3, GL_DIFFUSE, light2_diffuse);
		glLightfv(GL_LIGHT3, GL_SPECULAR, light2_specular);
		glLightfv(GL_LIGHT3, GL_POSITION, farola2);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, dir);
		glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, cut);
		glEnable(GL_LIGHT3);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		GLfloat light1_diffuse[] = { 0.0, 0.0, 0.0, 0.5 }; /* bright white */
		GLfloat light1_specular[] = { 0.0, 0.0, 0.0, 0.5 };
		GLfloat pso[] = { 1.0, 1.0, 1.0, 0.0 }; /* soft white */

		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
		glLightfv(GL_LIGHT1, GL_POSITION, pso);


		
	}
	else {
		glClearColor(0.75f, 1.0f, 1.0f, 1.0f);
		GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; /* bright white */
		GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat pso[] = { 0.5, 1.0, 0.5, 0.0 }; /* soft white */

		glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
		glLightfv(GL_LIGHT1, GL_POSITION, pso);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		
	}
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, alturaImg, anchoImg, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	

	
	for (float i = 0; i < 20;i++) {
		for (float j = 0; j < 20; j++) {
			glBegin(GL_QUADS);
			if (nit) {
				glNormal3d(0, 1, 0);
			}
			else {
				glNormal3d(0, 0, 1);
			}
			
			

			glTexCoord2f(i/20,j/20);
			glVertex3f(i*5 , -1.0, j*5 );

			glTexCoord2f(i / 20, (j+1) / 20);
			glVertex3f(i * 5, -1.0, (j * 5)+5 );

			glTexCoord2f((i + 1) / 20, (j + 1) / 20);
			glVertex3f((i * 5) +5 , -1.0, (j * 5) + 5);

			glTexCoord2f((i + 1) / 20, j / 20);
			glVertex3f((i * 5) + 5, -1.0, j * 5);

			glEnd();
		}
	}


	
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	drawCar();
	glPopMatrix();
	dibuixarFarola(58, 53);
	dibuixarFarola(20, 70);
	glutSwapBuffers();


}

void processNormalKeys(unsigned char key, int xx, int yy) {
	switch (key) {
	case 97:
		leftHeld = true;
		break;
	case 100:
		rightHeld = true;
		break;
	case 119:
		upHeld = true;
		break;
	case 115:
		downHeld = true;
		break;
	default: //no match found
		return; //return so handled dosnt get set
	}
	return;
}

void onKeyUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 97:
		leftHeld = false;
		break;
	case 100:
		rightHeld = false;
		break;
	case 119:
		upHeld = false;
		break;
	case 115:
		downHeld = false;
		break;
	case 120:
		camara = false;
		break;
	case 122:
		camara = true;
		break;
	case 49:
		nit = false;
		break;
	case 50:
		nit = true;
		break;
	default: //no match found
		return; //return so handled dosnt get set
	}
	return;


}

void ProcessInput()
{
	if (leftHeld){
		steering = -1;
	}
	else if (rightHeld){
		steering = 1;
	}
	else{
		steering = 0;
	}
	if (upHeld){
		throttle = 1;
	}
	else{ throttle = 0; }
		

	if (downHeld)
	{ brakes = 1; 
	}
	else{ brakes = 0; }
	return;
}

void reshape(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0) {
		h = 1;
	}
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

}
void acceleracio(void) {
	
	if (throttle) {
		a += 0.00002;
	}
	else if (brakes) {
		a -= 0.00002;
	}
	else if (a > 0)
	{
		a -= 0.00001;
	}
	else if (a < 0) {
		a += 0.00001;
	}

	if (a > 0.02) {
		a = 0.02;
	}
	else if (a < -0.02){
		a = -0.02;
	}
}

void actualitzarPos(void) {
	
	if (steering == -1) {
		aux = dirCotxo;
		multiplicarVectors(dirCotxo, normal);
		dirCotxo[0] -= ((perpendicular[0] / 10) *a);
		dirCotxo[1] -= ((perpendicular[1] / 10) * a);
		dirCotxo[2] -= ((perpendicular[2] / 10) * a);
		normalitzarVector();	
		angleCotxo += 5.75*a;
	}
	else if (steering == 1) {
		
		multiplicarVectors(dirCotxo, normal);
		dirCotxo[0] += ((perpendicular[0] / 10) * a);
		dirCotxo[1] += ((perpendicular[1] / 10) * a);
		dirCotxo[2] += ((perpendicular[2] / 10) * a);
		normalitzarVector();
		angleCotxo -= 5.75*a;
		

	}
	if (abs(angleCotxo) > 360) {
		angleCotxo = 0;
	}
	posCar[0] += dirCotxo[0] * a;
	posCar[1] += dirCotxo[1] * a;
	posCar[2] += dirCotxo[2] * a;
}

// Funci�n que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
	ProcessInput();
	acceleracio();
	actualitzarPos();
	
	

	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}






// Funci�n principal
int main(int argc, char** argv)
{
	// Inicializamos la librer�a GLUT
	glutInit(&argc, argv);
	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Creamos la nueva ventana
	glutCreateWindow("Mi primera Ventana");
	
	texture = loadBMPRaw("Circuit.bmp",alturaImg,anchoImg,true);


	// Indicamos cuales son las funciones de redibujado e idle
	
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutIdleFunc(Idle);
	glutKeyboardFunc(processNormalKeys);
	glutKeyboardUpFunc(onKeyUp);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHT1);
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);



	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)

	
	glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 1;
}