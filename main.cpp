#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>
#include <windows.h>
#include <GL/glut.h>
#include "1605119_classes.h"
#define pi (2*acos(0.0))
using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double w_q_angle;
double rotVect;
double a_s_angle;
double e_r_angle;
double d_f_angle;
int rotOfLeft=0;
double const_K = 2.0;
double const_angle = 0.01;


point pos,l,u,r;


/// constant inputs







int recursion_level;
int bmp_image_dim;
vector <object*> objects;




void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 1000,0,0);
            glVertex3f(-1000,0,0);

            glVertex3f(0,-1000,0);
            glVertex3f(0, 1000,0);

            glVertex3f(0,0, 1000);
            glVertex3f(0,0,-1000);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }
    glEnd();
}








void drawSphere(double radius,int slices,int stacks,double R, double G, double B)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	glColor3f(R,G,B);
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}

}





void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}


void LookLeft()
{
    r = r*cos(const_angle) + (u*r)*sin(const_angle);
    l = l*cos(const_angle) + (u*l)*sin(const_angle);
}

void LookRight()
{
    r = r*cos(const_angle) - (u*r)*sin(const_angle);
    l = l*cos(const_angle) - (u*l)*sin(const_angle);
}

void LookUp()
{
    u = u*cos(const_angle) + (r*u)*sin(const_angle);
    l = l*cos(const_angle) + (r*l)*sin(const_angle);
}
void LookDown()
{
    u = u*cos(const_angle) - (r*u)*sin(const_angle);
    l = l*cos(const_angle) - (r*l)*sin(const_angle);
}
void TiltLeft()
{
    u = u*cos(const_angle) + (l*u)*sin(const_angle);
    r = r*cos(const_angle) + (l*r)*sin(const_angle);
}
void TiltRight()
{
    u = u*cos(const_angle) - (l*u)*sin(const_angle);
    r = r*cos(const_angle) - (l*r)*sin(const_angle);
}


void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        //drawgrid=1-drawgrid;
        LookLeft();
        break;
    case '2':
        LookRight();
        break;
    case '3':
        LookUp();
        break;
    case '4':
        LookDown();
        break;
    case '5':
        TiltRight();
        break;
    case '6':
        TiltLeft();
        break;



    default:
        break;
    }
}





void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        cameraHeight -= 3.0;
        pos.x-= const_K*l.x;
        pos.y-= const_K*l.y;
        pos.z-= const_K*l.z;

        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;
        pos.x+= const_K*l.x;
        pos.y+= const_K*l.y;
        pos.z+= const_K*l.z;
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        pos.x+= const_K*r.x;
        pos.y+= const_K*r.y;
        pos.z+= const_K*r.z;


        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        pos.x-= const_K*r.x;
        pos.y-= const_K*r.y;
        pos.z-= const_K*r.z;

        break;

    case GLUT_KEY_PAGE_UP:
        pos.x+= const_K*u.x;
        pos.y+= const_K*u.y;
        pos.z+= const_K*u.z;

        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x-= const_K*u.x;
        pos.y-= const_K*u.y;
        pos.z-= const_K*u.z;

        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}


class LightSource {
public:
    point p;
	LightSource() = default;
    double R,G,B;
	LightSource(double x, double y, double z) {
        p.x = x;
        p.y = y;
        p.z = z;
	}
	void set_color(double r, double g, double b)
	{
	    R=r;
	    G=g;
	    B=b;
	}


	void draw() {
		glPushMatrix();
		glTranslated(p.x, p.y, p.z);
		drawSphere(2.0, 10, 10,R,G,B);
		glPopMatrix();
	}
};


vector<LightSource> lights;

void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    //gluLookAt(0,0,200,	0,0,0,	0,1,0);
    gluLookAt(pos.x,pos.y,pos.z, pos.x + l.x, pos.y + l.y,pos.z + l.z, u.x,u.y,u.z);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();

   // glColor3f(0.1,0.2,0.2);

    drawSphere(20,50,50,0.1,0.2,0.2);
    for(int i=0; i<lights.size(); i++)
    {
        lights[i].draw();
    }




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    w_q_angle = 0.0;
    e_r_angle = 0.0;
    a_s_angle = 0.0;
    d_f_angle = 0.0;
    angle=0;

    // initializing camera position

    pos.x = 100;
    pos.y = 100;
    pos.z = 0;

    u.x = 0;
    u.y = 0;
    u.z = 1;

    r.x = -1.0/sqrt(2);
    r.y = 1.0/sqrt(2);
    r.z = 0;

    l.x = -1.0/sqrt(2);
    l.y = -1.0/sqrt(2);
    l.z = 0;




    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(100,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}





void load_data()
{
    ifstream file ;

    file.open("D:/rafsani/Study/L4 T1/CSE 410 Graphics/RayTracing/scene.txt");
    string str;
    getline(file,str);
    recursion_level = stoi(str);
    getline(file,str);
    bmp_image_dim = stoi(str);
    getline(file,str);
    getline(file,str);
    int no_of_objects;
    no_of_objects = stoi(str);
    cout << no_of_objects << " " << recursion_level << " " << bmp_image_dim << endl;
    while(no_of_objects--)
    {
        file >> str;
        if(str == "sphere")
        {
           double x, y, z, radius, R, G, B, ambient, diffuse, specular, reflection;
           int shininess;
           file >> x >> y >> z >> radius >> R >> G >> B >> ambient >> diffuse >> specular >> reflection >> shininess;
           Sphere &sp = *new Sphere(x,y,z,radius);
           sp.set_properties(R,G,B,ambient,diffuse,specular,reflection,shininess);
           objects.push_back(&sp);
        }
        else if(str == "triangle")
        {
            double x1, y1, z1,x2, y2, z2,x3, y3, z3, R, G, B, ambient, diffuse, specular, reflection;
            int shininess;
            file >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3 >> R >> G >> B >> ambient >> diffuse >> specular >> reflection >> shininess;
            Triangle &sp = *new Triangle(x1, y1, z1,x2, y2, z2,x3, y3, z3);
            sp.set_properties(R,G,B,ambient,diffuse,specular,reflection,shininess);
            objects.push_back(&sp);
        }
        else if(str == "general")
        {
            double  a,b,c,d,e,f,g,h,i,j, len, width, height,px,py,pz, R, G, B, ambient, diffuse, specular, reflection;
            int shininess;
            file >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> px >> py >> pz >> len >> width >> height >> R >> G >> B >> ambient >> diffuse >> specular >> reflection >> shininess;
            General &sp = *new General(a,b,c,d,e,f,g,h,i,j,len,width,height);
            sp.set_cube_reference_point(px,py,pz);
            sp.set_properties(R,G,B,ambient,diffuse,specular,reflection,shininess);
            objects.push_back(&sp);
        }
    }
    int no_of_lights;
    file >> no_of_lights;
    while(no_of_lights--)
    {
         double x,y,z,r,g,b;
         file >> x >> y >>z >> r >> g >> b;
         LightSource lt(x,y,z);
         lt.set_color(r,g,b);
         lights.push_back(lt);
    }

    for(int o=0 ; o < objects.size() ; o++)
    {
        objects[o]->draw();
    }
}

int main(int argc, char **argv)
{

    load_data();

    ///  Input file reading ///


    glutInit(&argc,argv);
    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Ray tracing");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}