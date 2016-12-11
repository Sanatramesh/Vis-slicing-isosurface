/*
 * VolumeVisDraw.cpp
 * 
 * Copyright 2015 Sanat <sanat@sanat-HP-Pavilion-dv6-Notebook-PC>
 *  
 */

#include "VolumeVis.h"

Cube cubes[90][60][24];
Vertex vertices[90][60][24];

enum {UP = 1, DOWN, };

int flag = 1;
int oldX = -13;
int oldY = -13;
int mState = UP;
int h = 0, w = 0;

float scale = 1;
float minval, maxval;
float rot[3]={0.0,0.0,0.0};
float da = 0, db = 0, dc = 1, dd = -790.5;

/*
 * initRendering: Setting up glut window and view
 * 
 */
void initRendering() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	::w = glutGet( GLUT_WINDOW_WIDTH );
	::h = glutGet( GLUT_WINDOW_HEIGHT );
	glViewport(0, 0, w, h);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();    
	gluPerspective( 45.0, w / h, 0., 10.0 );
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*
 * handleResize: Setting up glut window and view after window resize
 * 
 */
void handleResize(int wt, int ht){
	if (height  == 0)
		height = 1;
	::w = wt;
	::h = ht;
	glViewport(0,0, wt, ht);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective( 45.0, wt / ht, 0, 10.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * isoSurflegend: displaying legend used for isosurfaces
 * 
 */
void isoSurflegend(){
	float r = 0;
	float g = 0;
	float b = 0.25;
	float xval = 0, var= 0.1,sal= 0.1;
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	for(sal=maxsal;sal>=minsal;sal-=(maxsal-minsal)/5){
		g = (sal-minsal)/(maxsal-minsal);
		glColor3f(r, g, b);
		glBegin(GL_QUADS);
		glVertex3f(-0.6+xval,-1,0);
		glVertex3f(-0.6+var+xval,-1,0);
		glVertex3f(-0.6+var+xval,-1.2,0);
		glVertex3f(-0.6+xval,-1.2,0);
		glEnd();
		
		char conVal[5];
		sprintf(conVal, "%.2f", sal);
		glRasterPos3f( -0.6+xval, -1.3, 0); // location to start printing text
		for(int digit=0; digit < 5; digit++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit] ); // Print a character on the screen
		
		xval+=0.1;
	}
}

/*
 * legend: displaying legend used for colormap
 * 
 */
void legend(){
	float r = 0;
	float g = 0;
	float b = 0.25;
	float xval = 0, var= 0.1,sal= 0.1;
	g = (sal-minsal)/(maxsal-minsal);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	glBegin(GL_QUADS);
	glColor3f(r, 0, b);
	glVertex3f(-0.6+xval,-1,0);
	glColor3f(r, 1, b);
	glVertex3f(-0.6+1.1,-1,0);
	glColor3f(r, 1, b);
	glVertex3f(-0.6+1.1,-1.2,0);
	glColor3f(r, 0, b);
	glVertex3f(-0.6,-1.2,0);
	glEnd();
	
	for(sal=minsal;sal<=maxsal;sal+=(maxsal-minsal)/10){
		char conVal[5];
		sprintf(conVal, "%.2f", sal);
		glRasterPos3f( -0.6+xval, -1.3, 0); // location to start printing text
		
		for( int digit=0; digit < 5; digit++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, conVal[digit] ); // Print a character on the screen
		
		xval+=0.1;
	}
}

/*
 * drawSlice: display a slice of the volume data(cut by a plane)
 * 
 */
void drawSlice(){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int i, j, k, l;
	int maxval = 1800;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt
	(
		0., -2, 5,
		0., 0., 0,
		0, 1, 0
	);
	Vertex *v, v1;
	
	glBegin(GL_LINES);
	glColor3f(0,0,1);
	
	for (i = 0; i < length - 1; i++){
		for(j = 0; j < width - 1; j++){
			for (k = 0; k < height - 1; k++){
				for(l = 0; l < 12; l++){
					v1 = cubes[i][j][k].getEdge(l).getVertex1();
					glVertex3f(v1.getX()/length - 0.5, v1.getY()/length-0.33, v1.getZ()/maxval - 0.6);
					v1 = cubes[i][j][k].getEdge(l).getVertex2();
					glVertex3f(v1.getX()/length - 0.5, v1.getY()/length-0.33, v1.getZ()/maxval - 0.6);
				}
			}
		}
	}
	glEnd();
	
	glPushMatrix();
	glTranslatef(transx, transy, transz);
	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[1], 0, 1, 0);
	glScalef(scale, scale, scale);
	// Initializing variables
	float mod = std::sqrt(da*da + db*db + dc*dc + dd*dd);
	float a = da, b = db, c = dc, d = dd;
	a /= mod; b /= mod;
	c /= mod; d /= mod;

	int code;
	float sal;
	float verdist[8];
	int *temp_arr;
	
	for (i = 0; i < length - 1; i++){
		for(j = 0; j < width - 1; j++){
			for (k = 0; k < height - 1; k++){
				for(l = 0; l < 8; l++){
					verdist[l] = cubes[i][j][k].getVertex(l).getX()*a +
								 cubes[i][j][k].getVertex(l).getY()*b +
								 cubes[i][j][k].getVertex(l).getZ()*c + d;
				}
				code = 0;
				if (verdist[0] < 0) code |= 1;
				if (verdist[1] < 0) code |= 2;
				if (verdist[2] < 0) code |= 4;
				if (verdist[3] < 0) code |= 8;
				if (verdist[4] < 0) code |= 16;
				if (verdist[5] < 0) code |= 32;
				if (verdist[6] < 0) code |= 64;
				if (verdist[7] < 0) code |= 128;
				temp_arr = getTri(code);
				l = 0;
				while (l < 16 && temp_arr[l] != -1){
					glBegin(GL_TRIANGLES);
					v = cubes[i][j][k].getEdge(temp_arr[l]).interpolateSalinity(a,b,c,d);
					glColor3f(0, (v->getSalinity()-minsal)/(maxsal-minsal), 0.25);
					glVertex3f(v->getX()/length - 0.5, v->getY()/length - 0.33, v->getZ()/maxval - 0.6);
					free(v);
					v = cubes[i][j][k].getEdge(temp_arr[l+1]).interpolateSalinity(a,b,c,d);
					glColor3f(0, (v->getSalinity()-minsal)/(maxsal-minsal), 0.25);
					glVertex3f(v->getX()/length - 0.5, v->getY()/length - 0.33, v->getZ()/maxval - 0.6);
					free(v);
					v = cubes[i][j][k].getEdge(temp_arr[l+2]).interpolateSalinity(a,b,c,d);
					glColor3f(0, (v->getSalinity()-minsal)/(maxsal-minsal), 0.25);
					glVertex3f(v->getX()/length - 0.5, v->getY()/length - 0.33, v->getZ()/maxval - 0.6);
					free(v);
					l += 3;
					glEnd();
				}
			}
		}
	}
	glPopMatrix();
	glViewport(0,0, w, h );
	legend();
	glutSwapBuffers();
}

/*
 * drawIsosurface: Extract and render isosurfaces corresponding to different 
 * 				   isosurface values(using Marching Cubes).
 * 
 */
void drawIsosurface(){
	int i, j, k, l;
	int code;
	int *temp_arr;
	int maxval = 1800;
	float isosurf = 33.125;
	float lval = 0.3, scle = 0.5, transp = 0.3, tp = 0;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt
	(
		0., -2, 5,
		0., 0., 0,
		0, 1, 0
	);
	glPushMatrix();
	glTranslatef(transx, transy, transz);
	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[1], 0, 1, 0);
	glScalef(scale, scale, scale);
	
	Vertex *v;
	
	for (isosurf = maxsal; isosurf >= minsal; isosurf -= (maxsal - minsal)/5){
		glColor4f(0, (isosurf - minsal)/(maxsal - minsal), 0.25,(isosurf - minsal)/(maxsal - minsal));
		for(i = 0; i < length - 1; i++){
			for(j = 0; j < width - 1; j++){
				for(k = 0; k < height - 1; k++){
					code = 0;
					lval = (float)k/length;
					if (cubes[i][j][k].getVertex(0).getSalinity() < isosurf) code |= 1;
					if (cubes[i][j][k].getVertex(1).getSalinity() < isosurf) code |= 2;
					if (cubes[i][j][k].getVertex(2).getSalinity() < isosurf) code |= 4;
					if (cubes[i][j][k].getVertex(3).getSalinity() < isosurf) code |= 8;
					if (cubes[i][j][k].getVertex(4).getSalinity() < isosurf) code |= 16;
					if (cubes[i][j][k].getVertex(5).getSalinity() < isosurf) code |= 32;
					if (cubes[i][j][k].getVertex(6).getSalinity() < isosurf) code |= 64;
					if (cubes[i][j][k].getVertex(7).getSalinity() < isosurf) code |= 128;
					temp_arr = getTri(code);
					l = 0;
					while (l < 16 && temp_arr[l] != -1){
						glBegin(GL_TRIANGLES);
						v = cubes[i][j][k].getEdge(temp_arr[l]).interpolateP(isosurf);
						glVertex3f(v->getX()/length - 0.5, v->getY()/length - 0.33, v->getZ()/maxval - 0.6);
						free(v);
						v = cubes[i][j][k].getEdge(temp_arr[l+1]).interpolateP(isosurf);
						glVertex3f(v->getX()/length - 0.5, v->getY()/length - 0.33, v->getZ()/maxval - 0.6);
						free(v);
						v = cubes[i][j][k].getEdge(temp_arr[l+2]).interpolateP(isosurf);
						glVertex3f(v->getX()/length - 0.5, v->getY()/length - 0.33, v->getZ()/maxval - 0.6);
						free(v);
						l += 3;
						glEnd();
					}		
				}
			}
		}
	}
	glPopMatrix();
	glViewport(0,0, w, h );
	isoSurflegend();
	glutSwapBuffers();
}

/*
 * drawScene: Render the visualization(Volume slice or Isosurfaces).
 * 
 */
void drawScene(){
	if (flag)
		drawIsosurface();
	else{
		drawSlice();
	}
}

/*
 * keyboard: add keys for interaction
 */
void keyboard(unsigned char key, int x, int y){
	if (key == char(27) || key == 'q'){
		std::cout<<"Quiting ..."<<std::endl;
		exit(-1);
	}
	else if (key  == 'z'){
		scale += 0.1;
	}
	else if (key == 'x'){
		scale -= 0.1;
	}
	else if (key == 'c')
		flag = (flag+1)%2;
	else if (key == 'w')
		da += 0.5;
	else if (key == 'e')
		db += 0.5;
	else if (key == 'r')
		dc += 0.5;
	else if (key == 't')
		dd += 0.5;
	else if (key == 's')
		da -= 0.5;
	else if (key == 'd')
		db -= 0.5;
	else if (key == 'f')
		dc -= 0.5;
	else if (key == 'g')
		dd -= 0.5;
	else if (key == 'i')
		std::cin>>da>>db>>dc>>dd;
	
	glutPostRedisplay();
}

/*
 * glutMotion: add mouse drag for interaction
 */
void glutMotion(int x, int y)
{
    if (mState == DOWN)
    {
      if (oldY - y < 0){
        rot[0]+=0.5;
      }
      else{
        rot[0]-=0.5;
      }
      if (oldX - x < 0){
        rot[1] += 0.5;
      }

      else{
        rot[1] -= 0.5;
      }
       
        glutPostRedisplay ();
    }
    oldX = x;
    oldY = y;
}

/*
 * glutMouse: add mouse click for interaction
 */
void glutMouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
            case GLUT_RIGHT_BUTTON:
                mState = DOWN;
                oldX = x;
                oldY = y;
            break;
        }
    } else if (state == GLUT_UP)
        mState = UP;
}

/*
 * SpecialKeys: add special keys for interaction
 */
void SpecialKeys(int key, int x, int y){
	if (key == GLUT_KEY_LEFT){
		transx+=0.1;
	}
	else if (key == GLUT_KEY_UP){
		transy-=0.1;
	}
	else if (key == GLUT_KEY_RIGHT){
		transx-=0.1;
	}
	else if (key == GLUT_KEY_DOWN){
		transy+=0.1;
	}
	glutPostRedisplay();
}

/*
 * main: initialize glut window, keyboard, mouse, etc and start Mainloop
 */
int main(int argc, char **argv){
	
	if (argc <= 1){
		std::cout<<argc<<"Usage: ./2dColorMap <file path>"<<std::endl; 
		exit(-1);
	}
	readData(argv);
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400); //Set the window size
	glutInitWindowPosition(50,50);
	//Create the window
	glutCreateWindow("Color Map");
	initRendering(); //Initialize rendering
	//Set handler function for drawing
	glutDisplayFunc(drawScene);
	//Set handler function for window resize
	glutReshapeFunc(handleResize);
	//Set handler function for keypress
	glutKeyboardFunc(keyboard);
	//Set handler function for Special keypress
	glutSpecialFunc(SpecialKeys);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutMainLoop(); //Start the main loop.
	return 0; 
}
