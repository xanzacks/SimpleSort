//  Zeyu Zhao
//  Assignment_3
//  Xcode on Mac
//
//  Created by zhaozeyu on 2019/2/5.
//  Copyright © 2019 zhaozeyu. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>
#include <iomanip>
#include <cstring>

using namespace std;

const double Pi = 3.141592654;

class Exception
{
public:
    Exception(const string message): message_(message){}
    Exception(const Exception &other) : message_(other.message_){}
    virtual ~Exception(){}
    
    const char *what() const
    {
        return message_.c_str();
    }
private:
    string message_;
};

class FileOpenException : public Exception
{
public:
    FileOpenException(const string message): Exception(message){}
    FileOpenException(const FileOpenException &other): Exception(other){}
    ~FileOpenException(){}
};

class InvalidThingCode : public Exception
{
public:
    InvalidThingCode(const string message): Exception(message){}
    InvalidThingCode(const FileOpenException &other): Exception(other){}
    ~InvalidThingCode(){}
};

void throwback(int n) throw (Exception, FileOpenException, InvalidThingCode)
{
    if (n == 1)
    {
        throw InvalidThingCode("Invalid Code: ???");
    }
    else if (n == 2)
    {
        throw Exception("test Exception");
    }
    else if (n == 3)
    {
        throw FileOpenException("Unable to open input file ");
    }
}

class Thing{
public:
    Thing() = default;
    string name() const;
    virtual ~Thing() = 0;
    friend ostream& operator<<(ostream&, const Thing&);
};

ostream& operator<<(ostream& out, const Thing& object)
{
    out <<setprecision(4)<<fixed<<left<<setfill(' ')<<setw(28)<<object.name();
    return out;
}

string Thing::name() const
{
    string quadname = typeid(*this).name();
#ifdef _MSC_VER       // for MS Visual Studio
    quadname = quadname.substr(6);
#elif __APPLE_CC__
    //cout<<quadname;
    quadname = quadname.substr(quadname.find_first_not_of("0123456789"));
#else                 // for other compilers
    quadname = quadname.substr(quadname.find_first_not_of("0123456789"));
#endif
    return quadname;
}

Thing::~Thing(){};

class GeometricSolid: virtual public Thing{
public:
    GeometricSolid() = default;
    virtual double volume() const= 0;
    ~GeometricSolid();
    friend ostream& operator<<(ostream&, const GeometricSolid&);
};

ostream& operator<<(ostream& out, GeometricSolid& object){
    try{
        if(object.volume() < 0){
            throwback(1);
        }
        else{
            out << static_cast<const Thing&>(object);
            out << right<<object.volume();
            out<<"  ";
        }
    }
    catch (Exception &e){
        out << "Invalid volume for ";
        out << static_cast<const Thing&>(object);
        out<<":"<<object.volume()<<" ";
    }
    return out;
}

GeometricSolid::~GeometricSolid(){}

class ColoredThing: virtual public Thing{
protected:
    char* color;
    double weight;
public:
    ColoredThing(const char*, double);
    ColoredThing() = default;
    virtual double density() const = 0;
    ~ColoredThing();
    friend ostream& operator<<(ostream&, const ColoredThing&);
    const char* returncolor() const;
};

ostream& operator<<(ostream& out, const ColoredThing& object){
    out <<left<<setfill(' ')<<setw(16)<<object.returncolor();
    out<<right<<object.density();
    return out;
}

const char* ColoredThing::returncolor() const{
    return color;
}

ColoredThing::~ColoredThing(){
    delete [] color;
}

ColoredThing::ColoredThing(const char* a, double b){
    color = new char[strlen(a) + 1];
    strcpy(color, a);
    weight = b;
}

class Circular: public GeometricSolid{
protected:
    double radius;
public:
    Circular(double r):radius(r){}
    Circular() = default;
    virtual double volume() const= 0;
};

class RectangularPrism: public GeometricSolid{
protected:
    double length, width, height;
public:
    RectangularPrism(double length, double width, double height):length(length), width(width), height(height){}
    RectangularPrism() = default;
    virtual double volume() const;
};

double RectangularPrism::volume() const{
    double v = length*width*height;
    return v;
}

class Cylinder: public Circular{
protected:
    double height;
public:
    Cylinder(double radius, double height):Circular(radius), height(height){}
    Cylinder() = default;
    virtual double volume() const;
};

double Cylinder::volume() const{
    double v = Pi*radius*height;
    return v;
}

class Sphere: public Circular{
public:
    Sphere(double radius):Circular(radius){}
    Sphere() = default;
    virtual double volume() const;
};

double Sphere::volume() const{
    double v = 4*Pi*radius*radius*radius/3;
    return v;
}

class Cube: public RectangularPrism{
public:
    Cube(double side):RectangularPrism(side, side, side){}
    Cube() = default;
};

class ColoredCylinder: public Cylinder, public ColoredThing{
public:
    ColoredCylinder(double radius, double height, double weight, const char* color):Cylinder(radius, height), ColoredThing(color, weight){}
    ColoredCylinder() = default;
    virtual double density() const;
};

double ColoredCylinder::density() const{
    double density = volume()/weight;
    return density;
}

class ColoredSphere: public Sphere, public ColoredThing{
public:
    ColoredSphere(double radius, double weight, const char* color):Sphere(radius), ColoredThing(color, weight){}
    ColoredSphere() = default;
    virtual double density() const;
};

double ColoredSphere::density() const{
    double density = volume()/weight;
    return density;
}

class ColoredRectangularPrism: public RectangularPrism, public ColoredThing{
public:
    ColoredRectangularPrism(double length, double width, double height, double weight, const char* color):RectangularPrism(length, width, height), ColoredThing(color, weight){}
    ColoredRectangularPrism() = default;
    virtual double density() const;
};

double ColoredRectangularPrism::density() const{
    double density = volume()/weight;
    return density;
}

class ColoredCube: public Cube, public ColoredThing{
public:
    ColoredCube(double side, double weight, const char* color):Cube(side), ColoredThing(color, weight){}
    ColoredCube() = default;
    virtual double density () const;
};

double ColoredCube::density() const{
    double density = volume()/weight;
    return density;
}

int main(int argc, const char * argv[]) {
    /*if(argc < 2){
        cerr<<"Missing command-line argument"<<endl;
        exit(1);
    }*/
    string filename;
    cout << "Enter filename => ";
    cin >> filename;
    fstream fin(filename, ios::binary|ios::in);
    try{
        if (!fin.is_open()){
            throwback(3);
        }
    }
    catch (FileOpenException &e){
        cerr << e.what() << endl;
        exit(2);
    }
    char buf[4];
    streampos pos;
    GeometricSolid* ptG;
    ColoredThing* ptC;
    Thing* ptT;
    double length, width, height, radius, weight;
    char color[8];
    cout<<"Type of Object                Volume Color        Density "<<endl;
    for(int i = 0; i < 50; i++){
        //fin.read(buf, sizeof(buf));
        char x[1];
        double y;
        fin.read(x, sizeof(x));
        fin.read(reinterpret_cast<char*>(&y), sizeof(y));
        cout << y << endl;
        fin.read(reinterpret_cast<char*>(&y), sizeof(y));
        cout << y << endl;
        unsigned a = static_cast<unsigned>(x[0]);
        int num = a << 24 >> 24;
        cout << num << endl;
        /*if(strcmp(buf, "cyc") == 0){
            fin.read(reinterpret_cast<char*>(&radius), sizeof (radius));
            fin.read(reinterpret_cast<char*>(&height), sizeof (height));
            fin.read(reinterpret_cast<char*>(&weight), sizeof (weight));
            fin.read(color, 8);
            ptT = new ColoredCylinder(radius, height, weight, color);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            ptC = dynamic_cast<ColoredThing*>(ptT);
            cout<<*ptG;
            cout<<*ptC<<endl;
        }
        else if(strcmp(buf, "spc") == 0){
            fin.read(reinterpret_cast<char*>(&radius), sizeof (radius));
            fin.read(reinterpret_cast<char*>(&weight), sizeof (weight));
            fin.read(color, 8);
            ptT = new ColoredSphere(radius, weight, color);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            ptC = dynamic_cast<ColoredThing*>(ptT);
            cout<<*ptG;
            cout<<*ptC<<endl;
        }
        else if(strcmp(buf, "rec") == 0){
            fin.read(reinterpret_cast<char*>(&length), sizeof (length));
            fin.read(reinterpret_cast<char*>(&width), sizeof (width));
            fin.read(reinterpret_cast<char*>(&height), sizeof (height));
            fin.read(reinterpret_cast<char*>(&weight), sizeof (weight));
            fin.read(color, 8);
            ptT = new ColoredRectangularPrism(length, width, height, weight, color);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            ptC = dynamic_cast<ColoredThing*>(ptT);
            cout<<*ptG;
            cout<<*ptC<<endl;
        }
        else if(strcmp(buf, "cuc") == 0){
            fin.read(reinterpret_cast<char*>(&radius), sizeof (radius));
            fin.read(reinterpret_cast<char*>(&weight), sizeof (weight));
            fin.read(color, 8);
            ptT = new ColoredCube(radius, weight, color);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            ptC = dynamic_cast<ColoredThing*>(ptT);
            cout<<*ptG;
            cout<<*ptC<<endl;
        }
        else if(strcmp(buf, "cyn") == 0){
            fin.read(reinterpret_cast<char*>(&radius), sizeof (radius));
            fin.read(reinterpret_cast<char*>(&height), sizeof (height));
            ptT = new Cylinder(radius, height);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            cout<<*ptG<<endl;
        }
        else if(strcmp(buf, "spn") == 0){
            fin.read(reinterpret_cast<char*>(&radius), sizeof (radius));
            ptT = new Sphere(radius);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            cout<<*ptG<<endl;
        }
        else if(strcmp(buf, "ren") == 0){
            fin.read(reinterpret_cast<char*>(&length), sizeof (length));
            fin.read(reinterpret_cast<char*>(&width), sizeof (width));
            fin.read(reinterpret_cast<char*>(&height), sizeof (height));
            ptT = new RectangularPrism(length, width, height);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            cout<<*ptG<<endl;
        }
        else if(strcmp(buf, "cun") == 0){
            fin.read(reinterpret_cast<char*>(&radius), sizeof (radius));
            ptT = new Cube(radius);
            ptG = dynamic_cast<GeometricSolid*>(ptT);
            cout<<*ptG<<endl;
            }
        else{
            try{
            throwback(1);
            }
            catch (InvalidThingCode &e){
                cerr << e.what() << endl;
                exit(3);
            }
        }*/
    }
    fin.close();
    return 0;
}
