#include <bits/stdc++.h>


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
        std::cout << "sph";

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
        std::cout << "tri";
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
        std::cout << "general";
     }
};




