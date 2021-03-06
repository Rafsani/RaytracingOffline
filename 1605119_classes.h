#include <bits/stdc++.h>
#include <windows.h>
#include <GL/glut.h>
#include<stdlib.h>
using namespace std;
extern int recursion_level;
class pixelColor
{
public:
    double R,G,B;
    void set_color(double r,double g, double b)
    {
        R=r;
        G=g;
        B=b;
    }
};

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


extern point pos,l,r,u;

double Dot_mult(point p1,point p2)
{
    return p1.x*p2.x + p1.y*p2.y + p1.z* p2.z;
}

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

class LightSource
{
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


    void draw()
    {
        glPushMatrix();
        glTranslated(p.x, p.y, p.z);
        drawSphere(2.0, 10, 10,R,G,B);
        glPopMatrix();
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
    object() {};
    virtual void draw() {} ;
    virtual double getTvalue(Ray &ray) {};
    point normalvect(point intersect) { };
    void intersectmethod(Ray &ray,double t, pixelColor color,int level) {};
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


    void intersect_method(Ray &ray,double t, pixelColor &p,int level);





};


std::vector <object*> objects;
std::vector<LightSource> lights;

void object::intersect_method(Ray &ray,double t, pixelColor &p,int level)
{
    point intersecting_point = ray.start +(ray.dir_vect * t)  ;
    point normal_atIntercectPoint = normalvect(intersecting_point);

    point reflectedRay = ray.dir_vect - normal_atIntercectPoint * (2.0 * Dot_mult(ray.dir_vect, normal_atIntercectPoint));
    reflectedRay = normalize_point(reflectedRay);

    //p.set_color(color[0] *coEfficients[0],color[1]*coEfficients[0],color[2]*coEfficients[0]);

    for(int i=0; i<lights.size(); i++)
    {
        point lightToInsectDir = lights[i].p - intersecting_point;
        lightToInsectDir = normalize_point(lightToInsectDir);
        double lambert = 0.0,phong = 0.0;
        Ray L(intersecting_point + (lightToInsectDir * 0.01), lightToInsectDir);
        point Vvect = normalize_point(intersecting_point * (-1.0));
        point Rvect = normalize_point(normal_atIntercectPoint *(Dot_mult(L.dir_vect,normal_atIntercectPoint)*2.0) - L.dir_vect);
        bool obscured = false;

        for(int j=0; j<objects.size(); j++)
        {
            double isobscured = objects[j]->getTvalue(L);
            if(isobscured > 0)
            {
                obscured = true;
                break;
            }
        }


        if(!obscured)
        {
            lambert = 1.0 * coEfficients[1] * Dot_mult(L.dir_vect,normal_atIntercectPoint);
            phong = coEfficients[2] * pow(Dot_mult(Rvect,Vvect),shine);
            lambert = max(0.0,min(lambert,1.0));
            phong = max(0.0,min(phong,1.0));
        }
        p.R =  (coEfficients[0] + phong + lambert) * color[0];
        p.G =  ( coEfficients[0] + phong + lambert) *color[1];
        p.B = ( coEfficients[0] + phong + lambert) * color[2];


    }
    p.set_color(max(0.0,min(p.R,1.0)), max(0.0,min(p.G,1.0)), max(0.0,min(p.B,1.0)));

}


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

    point normalvect(point intersect)
    {
        point normal = intersect - reference_point;
        return normalize_point(normal);
        //return normal;
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


    double getTvalue(Ray &ray)
    {
        point Rd = ray.dir_vect;
        point Ro = ray.start - reference_point;

        double discriminant = 4* Dot_mult(Rd,Ro) * Dot_mult(Rd,Ro) - 4* Dot_mult(Ro,Ro) + radius*radius;
        if(discriminant < 0 )
            return -1;
        double disc = sqrt(discriminant);
        double t1 = (- 2 * Dot_mult(Rd,Ro) + disc)/2;
        double t2 = (- 2 * Dot_mult(Rd,Ro) - disc)/2;
        //std::cout << "t = " << t1 << " " << t2 << std::endl;
        if(t1>t2)
            return t2;
        else
            return t1;
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
    double getTvalue(Ray &ray) {};
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
    double getTvalue(Ray &ray) {};
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
    double getTvalue(Ray &ray) {};
};




