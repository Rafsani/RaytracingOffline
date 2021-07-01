#include <bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>

struct point
{
    double x,y,z;

    point operator+(const point& p)
    {
        point res;
        res.x = x + p.x;
        res.y = y + p.y;
        res.z = z + p.z;
        return res;
    }

    point operator-(const point& p)
    {
        point res;
        res.x = x - p.x;
        res.y = y - p.y;
        res.z = z - p.z;
        return res;
    }

    point operator*(const double& scl)
    {
        point res;
        res.x = x*scl;
        res.y = y*scl;
        res.z = z*scl;

        return res;
    }

    point operator*(const point& p)
    {
        point res;
        res.x = y * p.z - z * p.y;
        res.y = z * p.x - p.z * x;
        res.z = x * p.y - p.x * y;
        return res;
    }

    point operator=(const point& p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;

    }

};






class object
{
    public:
     // should have x, y, z
    //double height, width, length ;
    double color[3] ;
    double coEfficients[4] ;// reflection coefficients
    int shine ;// exponent term of specular component
    object(){};
    virtual void draw(){} ;
    void set_properties(double R,double G,double B,double ambient,double diffuse,double speculer,double reflection,double shines)
     {
         color[0]  =R;
         color[1]=G;
         color[2] = B;
         coEfficients[0] = ambient;
         coEfficients[1] = diffuse;
         coEfficients[2] = speculer;
         coEfficients[3] = reflection;
         shine = shines;
     }

};



class Sphere: public object
{
    public:
    point reference_point;
    double radius;
    Sphere()
    {
        reference_point.x = 0;
        reference_point.y = 0;
        reference_point.z = 0;
        radius = 0;
    }
     Sphere(double x,double y, double z, double rad)
     {
         reference_point.x = x;
         reference_point.y = y;
         reference_point.z = z;
         radius = rad;
     }



     void draw()
     {
        glPushMatrix();
        {

            glTranslatef(reference_point.x,reference_point.y,reference_point.z);
            drawSphere(radius,50,50,color[0],color[1],color[2]);

        }
        glPopMatrix();


     }
};


class Triangle: public object
{
    public:
    point points[3];
    double radius;
     Triangle(double x1,double y1, double z1,double x2,double y2, double z2,double x3,double y3, double z3)
     {
         points[0].x = x1;
         points[0].y = y1;
         points[0].z = z1;
         points[1].x = x2;
         points[1].y = y2;
         points[1].z = z2;
         points[2].x = x3;
         points[2].y = y3;
         points[2].z = z3;
     }

     void draw()
     {
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(points[0].x, points[0].y, points[0].z);
            glVertex3f(points[1].x, points[1].y, points[1].z);
            glVertex3f(points[2].x, points[2].y, points[2].z);
        }
        glEnd();
     }
};


class General: public object
{
    public:
    double  A, B, C, D, E, F, G, H, I, J;
    point   cube_reference_point;
    double length, width, height;
     General(double a,double b, double c, double d, double e, double f, double g, double h, double i, double j, double l, double w, double hh)
     {
         length = l ;
         width = w;
         height = hh;
         A=a;
         B=b;
         C=c;
         D=d;
         E=e;
         F=f;
         G=g;
         H=h;
         I=i;
         J=j;
     }
     void set_cube_reference_point(double a,double b, double c)
     {
         cube_reference_point.x = a;
         cube_reference_point.y = b;
         cube_reference_point.z = c;
     }

     void draw()
     {

     }
};




class Floor: public object
{
    public:
        point reference_point;
        double tileWidth;
        int no_of_tiles;
        Floor(double floorWidth, double tilesize)
        {

            reference_point.x = -floorWidth/2;
            reference_point.y = -floorWidth/2;
            reference_point.z = 0;
            tileWidth = tilesize;
            no_of_tiles = floorWidth/tilesize;
        }

        void draw()
        {
            //std::cout << "floor";
            glBegin(GL_QUADS);
                {
                    for (int i = 0; i < no_of_tiles; i++)
                        for (int j = 0; j < no_of_tiles; j++)
                        {
                            bool c = (i + j) % 2;
                            glColor3f(c, c, c);

                            glVertex3f(reference_point.x + tileWidth * i, reference_point.y + tileWidth * j, reference_point.z);
                            glVertex3f(reference_point.x + tileWidth * (i + 1), reference_point.y + tileWidth * j, reference_point.z);
                            glVertex3f(reference_point.x + tileWidth * (i + 1), reference_point.y + tileWidth * (j + 1), reference_point.z);
                            glVertex3f(reference_point.x + tileWidth * i, reference_point.y + tileWidth * (j + 1), reference_point.z);
                        }
                }
                glEnd();
        }
};


class LightSource {
public:
    point p;
	LightSource() = default;
    double R,G,B;
	LightSource(double x, double y, double z)
	{
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

point normalize_point (point arg)
{
    double val = sqrt(arg.x*arg.x + arg.y*arg.y + arg.z*arg.z);

    point p;
    p.x = arg.x/val;
    p.y = arg.y/val;
    p.z = arg.z/val;
    return p;
}

class Ray
{

  public:
    point start;
    point dir_vect;

    Ray(point st, point dir)
    {
        start = st;
        dir_vect = normalize_point(dir);
    }
};


