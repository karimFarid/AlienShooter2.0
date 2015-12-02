#include <GLUT/glut.h>
#include <math.h>
#include <iostream>
#define DEG2RAD(a) (a * 0.0174532925)
float rotAng;

#define DEG2RAD(a) (a * 0.0174532925)


class Vector3f {
    
public:
    
    float x, y, z;
    
    
    
    Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
        
        x = _x;
        
        y = _y;
        
        z = _z;
        
    }
    
    
    
    Vector3f operator+(const Vector3f &v) const {
        
        return Vector3f(x + v.x, y + v.y, z + v.z);
        
    }
    
    
    
    Vector3f operator-(Vector3f &v) {
        
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
    
    
    
    Camera(float eyeX = 13.3429, float eyeY = 1.69232, float eyeZ = 0.4, float centerX = 12.344, float centerY = 1.64503, float centerZ =  0.4, float upX = -0.0472866, float upY = 0.998881, float upZ = 0.0f) {
        
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
    
    gluPerspective(60, 1200 / 700, 0.001, 100);
    
    
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    
    camera.look();
    
}



void Display(void) {
    setupCamera();
    
    setupLights();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glPushMatrix();
    glTranslatef(0.866025, 0.866025, 0.866025);
    glRotatef(90, 1, 0, 0);
    glColor3f(0.5f, 0.5f, 0.5f);
    glutWireSphere(30, 25, 25);
    glPopMatrix();
    
    //    glPushMatrix();
    //    glTranslatef(0.866025, 0.866025, 0.866025);
    //    glRotatef(90, 1, 0, 0);
    //    glColor3f(0.0f, 0.0f, 0.0f);
    //    glutSolidSphere(15, 25, 25);
    //    glPopMatrix();
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslated(0, -30, 0);
    glutSolidCube(70);
    glPopMatrix();
    
    glFlush();
}

void Anim() {
    //rotAng += 1;
    
    glutPostRedisplay();
}
void Keyboard(unsigned char key, int x, int y) {
    
    float d = 1;
    
    
    
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
            
        case ' ':
            std::cout << " eye x:  ";
            std::cout << camera.eye.x;
            std::cout << " eye y: ";
            std::cout << camera.eye.y;
            std::cout << " eye z: ";
            std::cout << camera.eye.z;
            std::cout << " center x: ";
            std::cout << camera.center.x;
            std::cout << " center y: ";
            std::cout << camera.center.y;
            std::cout << " center z: ";
            std::cout << camera.center.z;
            std::cout << " up x: ";
            std::cout << camera.up.x;
            std::cout << " up y: ";
            std::cout << camera.up.y;
            std::cout << " up z: ";
            std::cout << camera.up.z;
            
            break;
            
            
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



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(150, 150);
    
    glutCreateWindow("Alien Shooter 2.0");
    glutKeyboardFunc(Keyboard);
    
    glutSpecialFunc(Special);
    glutDisplayFunc(Display);
    glutIdleFunc(Anim);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1200 / 700, 0.1f, 300.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    glutMainLoop();
}
