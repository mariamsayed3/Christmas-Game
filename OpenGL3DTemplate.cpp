#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <iostream>
#include <windows.h>
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;
GLUquadricObj* qobj;
double PlayerX = 0.70;
double PlayerZ = 0.70;
int angleOfRotation = 0;
double snowmanAngleOfrotation = 90.0;
double LanternsAngleOfrotation = 0.0;
double presentsAngleOfrotation = 0.0;
double treesAngleOfrotation = 0.0;
double backfenceAnimation = 0.03;
double rightfenceAnimation = 0.0;
double leftfenceAnimation = 0.08;
//R == RIGHT ... L == LEFT
bool colliedPresentOneR= false;
bool colliedPresentTwoR = false;
bool colliedPresentOneL = false;
bool colliedPresentTwoL = false;
bool animationWork = false;
double oldSnowmanRotation = 90.0;

double fencefirstcolor = 0.2;
double fencesecondcolor = 0.2;
double fencethirdcolor = 0.2;

bool dynamic = true;

bool timeIsUp = false;
bool loser = false;


bool stopSoundEnd = false;
bool stopCollisionSoundRO = false;
bool stopCollisionSoundRT = false;
bool stopCollisionSoundLO = false;
bool stopCollisionSoundLR = false;
bool start = false;

int timeUp = 40;




void print(int x, int y, char* string);


void winOrNot() {
	if (loser == true) {
		colliedPresentOneR = true;
		colliedPresentTwoR = true;
		colliedPresentOneL = true;
		colliedPresentTwoL = true;
		if (stopSoundEnd == false) {
			PlaySound(TEXT("gameover.wav"), NULL, SND_ASYNC);
			stopSoundEnd = true;
		}
	}
	else if (colliedPresentOneR == true &&
		colliedPresentTwoR == true &&
		colliedPresentOneL == true &&
		colliedPresentTwoL == true && loser ==false) {
		if(stopSoundEnd == false){
			PlaySound(TEXT("win.wav"), NULL, SND_ASYNC);
			stopSoundEnd = true;
		}
	
	}
}


void countingTime() {
	if (timeUp != 0 && (colliedPresentOneR == false|| colliedPresentTwoR==false || colliedPresentOneL==false || colliedPresentTwoL==false)) {
		timeUp--;
		std::cout << timeUp << " , ";
	}
	else if (timeUp == 0 && (colliedPresentOneR == false || colliedPresentTwoR == false || colliedPresentOneL == false || colliedPresentTwoL == false)) {
		loser = true;
	}
}

void time(int val) {

	if (fencefirstcolor <= 0.8 && fencesecondcolor <= 0.8) {
		fencefirstcolor = fencefirstcolor * 2;
		fencesecondcolor = fencesecondcolor * 2;
	}
	else {
		fencefirstcolor = 0.2;
		fencesecondcolor = 0.2;
	}
	countingTime();
	glutPostRedisplay();						
	glutTimerFunc(1000, time, 0);
}

void drawWall(double thickness) {
	glColor3f(0.6f, 0.8f, 0.8f);
	glPushMatrix();
	glTranslated(0.5, 0, 0.5);
	glScaled(1.0, thickness, 1.0);
	glRotated(45, 0, 1, 0);
	glutSolidCube(1);
	glPopMatrix();
}

void drawSnowMan() {
	//3 spheres
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslated(0.4, 0, 0.6);
	glutSolidSphere(1.5, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.4, 2.4, 0.6);
	glutSolidSphere(1, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.4, 3.8, 0.6);
	glutSolidSphere(0.5, 25, 25);
	glPopMatrix();
	//al tarbosh
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslated(0.4, 4.2, 0.6);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.3, 0.5, 25, 25);
	glPopMatrix();
	//al mana8er
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslated(0.4, 3.9, 0.6);
	glRotated(-45, 0, 1, 0);
	glutSolidCone(0.3, 1, 25, 25);
	glPopMatrix();
	glPopMatrix();

}

void drawTree(double x, double y, double z) {
	glPushMatrix();
	//1st tree
	glPushMatrix();
	glColor3f(0.0f ,0.5f ,0.0f);
	glTranslated(x, -y, z);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 25, 25);
	glPopMatrix();
	//2nd tree
	glPushMatrix();
	glColor3f(0.0f, 0.5f, 0.0f);
	glTranslated(x,-(y-0.13), 0.0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 25, 25);
	glPopMatrix();
	//3rd tree
	glPushMatrix();
	glColor3f(0.0f, 0.5f, 0.0f);
	glTranslated(0.1, -(y-0.23), 0.0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.1, 0.2, 25, 25);
	glPopMatrix();
	// first wired 
	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.0f);
	glTranslated(x, -y, z);
	glRotated(-90, 1, 0, 0);
	glutWireCone(0.1, 0.2, 20, 20);
	glPopMatrix();
	//second wired
	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.0f);
	glTranslated(x, -(y - 0.13), 0.0);
	glRotated(-90, 1, 0, 0);
	glutWireCone(0.1, 0.2, 20, 20);
	glPopMatrix();
	//third wired
	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.0f);
	glTranslated(0.1, -(y - 0.23), 0.0);
	glRotated(-90, 1, 0, 0);
	glutWireCone(0.1, 0.2, 20, 20);
	glPopMatrix();
	glPopMatrix();
}

void drawPresent() {
	glPushMatrix();
	// 1st cube
	glPushMatrix();
	glColor3f(1.0f, 0.0f , 0.0f);
	glRotated(45, 0, 1, 0);
	glutSolidCube(0.15);
	glPopMatrix();
	//2nd
	glPushMatrix();
	glColor3f(0.3f, 0.1f , 0.6f);
	glutSolidCube(0.14);
	glPopMatrix();
	//3rd
	glPushMatrix();
	glColor3f(0.3f, 0.1f , 0.7f);
	glRotated(30, 0, 1, 0);
	glutSolidCube(0.145);
	glPopMatrix();
	//button
	glPushMatrix();
	glTranslated(0.0f, 0.07f, 0.0f);
	glColor3f(0.3f, 0.1f, 0.6f);
	glutSolidSphere(0.02, 25, 25);
	glPopMatrix();
	glPopMatrix();
}

void drawLanterns() {
	glPushMatrix();
	//1st cylinder
	glPushMatrix();
	glColor3f(0.8f, 0.8f, 0.1f);
	glRotated(-90, 1,0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.05, 0.05, 0.30, 8, 8);
	glPopMatrix();

	//2nd cylinder
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslated(0.0,-0.04,0.0);
	glRotated(-90, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.04, 0.04, 0.38, 8, 8);
	glPopMatrix();

	//wired one bta3 al osayr
	glPushMatrix();
	glColor3f(0.7f, 0.0f, 0.0f);
	glRotated(-90, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.05, 0.05, 0.30, 8, 8);
	//wired one bta3 al tawel
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.0f);
	glTranslated(0.0, -0.04, 0.0);
	glRotated(-90, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.04, 0.04, 0.38, 8, 8);
	glPopMatrix();

	glPopMatrix();
}

void drawPlayer() {
	glPushMatrix();

	//gesmo
	glPushMatrix();
	glColor3f(0.9f, 0.0f, 0.0f);
	glTranslated(0.0, 0.07, 0.0);
	glRotated(-90, 1, 0, 0);
	//glScaled(0.5, 0.5, 0.5);
	glutSolidCone(0.1, 0.2, 25, 25);
	glPopMatrix();

	////raso
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslated(0.0, 0.25, 0.0);
	glutSolidSphere(0.05, 25, 25);
	glPopMatrix();

	//reglo elly 3ala al ymen
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslated(0.05, 0.01, 0.0);
	glRotated(-90, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.01, 0.01, 0.07, 8, 8);
	glPopMatrix();
	//reglo elly 3ala al shmal
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslated(0.0, 0.01, 0.05);
	glRotated(-90, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.01, 0.01, 0.07, 8, 8);
	glPopMatrix();
	//3eno el ymen
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslated(0.02, 0.27, 0.05);
	glutSolidSphere(0.005, 25, 25);
	glPopMatrix();
	//3eno al shemal
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslated(0.04, 0.27, 0.03);
	glutSolidSphere(0.005, 25, 25);
	glPopMatrix();

	//edo al shemal 
	glPushMatrix();
	glColor3f(0.9f, 0.0f, 0.0f);
	glTranslated(-0.02, 0.20, 0.014);
	glRotated(30, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.01, 0.01, 0.15, 8, 8);
	glPopMatrix();
	//edo al emen
	glPushMatrix();
	glColor3f(0.9f, 0.0f, 0.0f);
	glTranslated(0.01, 0.21, -0.01);
	glRotated(90, 1, 0, 1);
	glRotated(45, 1, 0, 0);
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.01, 0.01, 0.15, 8, 8);
	glPopMatrix();
	//tarbosho
	glPushMatrix();
	glColor3f(0.9f, 0.0f, 0.0f);
	glTranslated(0.0, 0.28, 0.0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.05, 0.09, 25, 25);
	glPopMatrix();

	glPopMatrix();
}

void drawOneBackFence(double x , double y , double z) {
	
	glPushMatrix();
	glColor3f(fencefirstcolor, fencesecondcolor, fencethirdcolor);
	glTranslated(x,y,z);
	glScaled(2, 16, 2);
	glRotated(45, 0, 1, 0);
	glutSolidCube(0.01);
	glPopMatrix();
}


void drawOneRightFence(double x, double y, double z) {

	glPushMatrix();
	glColor3f(fencefirstcolor, fencesecondcolor, fencethirdcolor);
	//glTranslated(0.45f, 0.09f, -0.23f);
	glTranslated(x, y, z);
	glScaled(2, 16, 2);
	glRotated(45, 0, 1, 0);
	glutSolidCube(0.01);
	glPopMatrix();

}

void drawOneLeftFence(double x, double y, double z) {
	
	glPushMatrix();
	glColor3f(fencefirstcolor, fencesecondcolor, fencethirdcolor);
	//glTranslated(-0.27f, 0.0f, 0.35f);
	glTranslated(x, y, z);
	glScaled(2, 18, 2);
	glRotated(45, 0, 1, 0);
	glutSolidCube(0.01);
	glPopMatrix();

}

void drawWholeBackFence(double startX , double constantY ,double startZ) {
	int i;
	for (i = 0; i < 10; i++,(startX-=0.07),(startZ+=0.07)) {
		drawOneBackFence(startX, constantY, startZ);
	}
}

void drawWholeRightFence(double startX, double constantY , double startZ) {
	int i;
	for (i = 0; i < 10; i++, (startX -= 0.07), (startZ -= 0.07)) {
		drawOneRightFence(startX, constantY, startZ);
	}
}

void drawWholeLeftFence(double startX, double constantY, double startZ) {
	int i;
	for (i = 0; i < 11; i++, (startX -= 0.065), (startZ -= 0.065)) {
		drawOneLeftFence(startX, constantY, startZ);
	}
}



void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void collision() {

	if (0.85 <= PlayerX && PlayerX <= 0.95 && 0.45 <= PlayerZ && PlayerZ <= 0.55) {
	
		if (stopCollisionSoundRO == false) {
			PlaySound(TEXT("collectpresent.wav"), NULL, SND_ASYNC);
			stopCollisionSoundRO = true;
		}
		colliedPresentOneR = true;

	}
	else if (0.451 <= PlayerX && PlayerX <= 0.551 && 0.811 <= PlayerZ && PlayerZ <= 0.951) {
		
		if (stopCollisionSoundLO == false) {
			PlaySound(TEXT("collectpresent.wav"), NULL, SND_ASYNC);
			stopCollisionSoundLO = true;
		}
		colliedPresentOneL = true;
	}
	else if (0.651 <= PlayerX && PlayerX <= 0.811 && 0.30 < PlayerZ && PlayerZ < 0.42) {

		if (stopCollisionSoundRT == false) {
			PlaySound(TEXT("collectpresent.wav"), NULL, SND_ASYNC);
			stopCollisionSoundRT = true;
		}
		colliedPresentTwoR = true;
	}
	else if (0.211 <= PlayerX && PlayerX <= 0.351 && 0.651 <= PlayerZ && PlayerZ <= 0.751) {
	
		if (stopCollisionSoundLR == false) {
			PlaySound(TEXT("collectpresent.wav"), NULL, SND_ASYNC);
			stopCollisionSoundLR = true;
		}
		colliedPresentTwoL = true;
	}
}

void animation() {
	if (animationWork == true) {
		PlaySound(TEXT("Animation.wav"), NULL, SND_ASYNC);
		snowmanAngleOfrotation = snowmanAngleOfrotation + 30.0;
		LanternsAngleOfrotation = LanternsAngleOfrotation + 30.0;
		presentsAngleOfrotation = presentsAngleOfrotation + 30.0;
		treesAngleOfrotation = treesAngleOfrotation + 30.0;
		if (backfenceAnimation < 0.95 && rightfenceAnimation < 0.95 && leftfenceAnimation < 0.95) {
			backfenceAnimation += 0.05;
			rightfenceAnimation += 0.05;
			leftfenceAnimation += 0.05;
		}
		else {
			backfenceAnimation = 0.03;
			rightfenceAnimation = 0.0;
			leftfenceAnimation = 0.08;
		}

	}
}

void Display() {
	setupCamera();
	setupLights();



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	winOrNot();

	
	//logic of animation
	collision();
	animation();
	


	//draw the Backfence
	glPushMatrix();
	glTranslated(-0.31, backfenceAnimation, -1.01);
	drawWholeBackFence(0.80f, 0.06f, 0.87f);
	glPopMatrix();

	
	//draw the Rightfence

	glPushMatrix();
	glTranslated(0.71, rightfenceAnimation, 0.71);
	drawWholeRightFence(0.45f, 0.09f, -0.23f);
	glPopMatrix();

	//draw the Leftfence

	glPushMatrix();
	glTranslated(0.78, leftfenceAnimation, 0.78);
	drawWholeLeftFence(-0.30f, 0.0f, 0.38f);
	glPopMatrix();

	//draw the player
	glPushMatrix();
	glTranslated(PlayerX, 0.0, PlayerZ);
	glRotated(angleOfRotation,0,1,0);
	drawPlayer();
	glPopMatrix();


	//Lanterns
	//1st lantern
	glPushMatrix();
	glTranslated(0.7, 0.30, 0.4);
	glRotated(LanternsAngleOfrotation, 0, 1, 0);
	glScaled(0.3, 0.3, 0.3);
	drawLanterns();
	glPopMatrix();
	//2nd lantern
	glPushMatrix();
	glTranslated(0.45, 0.30, 0.75);
	glRotated(LanternsAngleOfrotation, 0, 1, 0);
	glScaled(0.3, 0.3, 0.3);
	drawLanterns();
	glPopMatrix();
	//3rd lantern
	glPushMatrix();
	glTranslated(0.45, 0.30, 0.45);
	glRotated(LanternsAngleOfrotation, 0, 1, 0);
	glScaled(0.3, 0.3, 0.3);
	drawLanterns();
	glPopMatrix();

	//snowman
	glPushMatrix();
	glTranslated(0.3, 0.10, 0.3);
	glRotated(snowmanAngleOfrotation, 0, 1, 0);
	glScaled(0.07, 0.07, 0.07);
	drawSnowMan();
	glPopMatrix();

	//1st tree
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(0.9, 0.35, 0.29);
	glRotated(treesAngleOfrotation, 0, 1, 0);
	drawTree(0.1, 0.33, 0.0);
	glPopMatrix();

	//2nd tree 
	glPushMatrix();
	glScaled(0.6, 0.6, 0.6);
	glTranslated(0.15, 0.35, 1);
	glRotated(treesAngleOfrotation, 0, 1, 0);
	drawTree(0.1, 0.33, 0.0);
	glPopMatrix();
	// draw presents 
	//1st present 

	if (colliedPresentOneR == false) {
		glPushMatrix();
		glTranslated(0.9, 0.05, 0.5);
		glRotated(presentsAngleOfrotation, 0, 1, 0);
		glScaled(0.6, 0.6, 0.6);
		drawPresent();
		glPopMatrix();
	}
	

	//2nd present
	if(colliedPresentOneL == false){
		glPushMatrix();
		glTranslated(0.5, 0.05, 0.9);
		glRotated(presentsAngleOfrotation, 0, 1, 0);
		glScaled(0.6, 0.6, 0.6);
		drawPresent();
		glPopMatrix();
	}


	//3rd present
	if (colliedPresentTwoL == false) {
		glPushMatrix();
		glTranslated(0.3, 0.05, 0.7);
		glRotated(presentsAngleOfrotation, 0, 1, 0);
		glScaled(0.6, 0.6, 0.6);
		drawPresent();
		glPopMatrix();
	}


	//4th present 
	if (colliedPresentTwoR == false) {
		glPushMatrix();
		glTranslated(0.7, 0.05, 0.3);
		glRotated(presentsAngleOfrotation, 0, 1, 0);
		glScaled(0.6, 0.6, 0.6);
		drawPresent();
		glPopMatrix();
	}

	
	glPushMatrix();
	glTranslated(0.4, 0.0, 0.4);
	//drawTable(0.6, 0.02, 0.02, 0.3);
	glPopMatrix();
	drawWall(0.02);
	glPushMatrix();
	glRotated(90, 0, 0, 1.0);
	//drawWall(0.02);
	glPopMatrix();
	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	//drawWall(0.02);
	glPopMatrix();
	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 'r':
		camera.eye = Vector3f(1+0.5, 0.36, -0.5);
		camera.center = Vector3f(0.80, 0.50,0.20);
		camera.up = Vector3f(0, 1, 0);
		break;

	case 't':
		camera.eye = Vector3f(1,2,1);
		camera.center = Vector3f(0.66, 1.12, 0.64);

		break;
	case 'z':
		camera.eye = Vector3f(1.18,0.54, 1.18);
		camera.center = Vector3f(0.61, 0.10, 0.61);
		camera.up = Vector3f(-0.30, 0.80,-0.30);
		
		
		break;
	case 'k':
		if (PlayerZ - 0.866*PlayerX > -0.435) {
			PlayerX = PlayerX + 0.05;
			PlayerZ = PlayerZ - 0.05;
			angleOfRotation = 90;
		}
		break;
	case 'j':
	if (PlayerZ - 1.024*PlayerX< 0.5867) {
			PlayerX = PlayerX - 0.05;
			PlayerZ = PlayerZ + 0.05;
			angleOfRotation = -90;
		
		}
		break;
	case'n':
		if (PlayerX +PlayerZ < 1.7) {
			if (angleOfRotation != 0) {
				angleOfRotation = 0;
			}
			PlayerX = PlayerX + 0.05;
			PlayerZ = PlayerZ + 0.05;
			
		}
		break;
	case'm':
		if (PlayerX + PlayerZ >= 0.3) {
			angleOfRotation = -180;
			PlayerX = PlayerX - 0.05;
			PlayerZ = PlayerZ - 0.05;
		
		}
		break;
	case '1':
		animationWork = true;
		break;
	case '2':
		animationWork = false;
		snowmanAngleOfrotation = oldSnowmanRotation;
		LanternsAngleOfrotation = 0.0;
		presentsAngleOfrotation = 0.0;
		treesAngleOfrotation = 0.0;
		 backfenceAnimation = 0.03;
		 rightfenceAnimation = 0.0;
		 leftfenceAnimation = 0.08;
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}


void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutTimerFunc(0, time, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}

void print(int x, int y, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}