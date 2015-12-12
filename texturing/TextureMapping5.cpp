#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <GLUT/glut.h>
#include "RgbImage.h"
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

GLfloat xRotated, yRotated, zRotated;
GLuint   texture[2];
float rotAng;
bool jump=false;
int dir=0;
float eyeX = 12.6336;
bool flag=false;
float eyeY = 16.6755;
float eyeZ = 0.4;
float centerX = 11.6347;
float centerY = 16.6283;
float centerZ =  0.4;
float upX = -0.0472866;
float upY = 0.998881;
float upZ = 0.0f;
#define DEG2RAD(a) (a * 0.0174532925)
void loadTextureFromFile(char *filename)
{
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
   
    RgbImage theTexMap( filename );
    
    // Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
    //    Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );
    if (filename[2]=='e') {
        glGenTextures(1, &texture[0]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
    }else{
        glGenTextures(1, &texture[1]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[1]);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // Typical Texture Generation Using Data From The Bitmap
    
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );
}
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
    
    
    
    Camera() {
        
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
        
            
            eye = eye + -d;
            
            center = center + -d;
        

        
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
    
    //glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();
    
    gluPerspective(60, 1200 / 700, 0.001, 100);
    
    
    
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    
    camera.look();
    
}
void Keyboard(unsigned char key, int x, int y) {
    
    float d = 2;
    switch (key) {
        case 'w':
            camera.rotateX(d);
            break;
            
        case 's':
            camera.rotateX(-d);
            break;
            
        case 'a':
            camera.rotateY(d);
            break;
            
        case 'd':
            camera.rotateY(-d);
            break;
            
        case ' ':
            jump=true;
            dir=1;
            break;
    }
    glutPostRedisplay();
}
void Special(int key, int x, int y) {
    
    float a = 2.0;
    switch (key) {
         
            
        case GLUT_KEY_UP:
            if (camera.center.x>-50) {
                camera.moveZ(a);
                break;
            }
           
            
        case GLUT_KEY_DOWN:
            if (camera.center.x<50) {
            camera.moveZ(-a);
            break;
            }
        case GLUT_KEY_LEFT:
            if (camera.center.z<79) {
                camera.moveX(a);
                break;
            }
            
        case GLUT_KEY_RIGHT:
            if (camera.center.z>-79) {
                camera.moveX(-a);
                break;
            }
            break;
          
            
    }
    glutPostRedisplay();
}
void resizeWindow(int x, int y){
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0
    
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}
void RenderGround(){
    glDisable(GL_LIGHTING);	// Disable lighting
    glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
    glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
    glBindTexture(GL_TEXTURE_2D, texture[1]);	// Bind the ground texture
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);	// Set quad normal direction.
    glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
    glVertex3f(-80, 0, -80);
    glTexCoord2f(80, 0);
    glVertex3f(80, 0, -80);
    glTexCoord2f(80, 80);
    glVertex3f(80, 0, 80);
    glTexCoord2f(0, 80);
    glVertex3f(-80, 0, 80);
    glScaled(9, 9, 9);
    glEnd();
    glPopMatrix();
    //glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
    glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
    
}
void keyboard (unsigned char key, int x, int y){
    switch (key) {
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}
void Display(void) {
    setupCamera();
    
    setupLights();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glLoadIdentity();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    
    //    glRotatef(yRotated, 0, 1, 0);
    //    glRotatef(zRotated, 0, 0, 1);
    //    glutSolidTeapot(1);
    //    glRotated(90, 1, 0, 0);
    glPushMatrix();
    glColor3f(1, 1, 1);
    GLUquadric *quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, true);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    glTranslatef(0.866025, 0.866025, 0.866025);
    glScaled(20, 20, 20);
    gluSphere(quadObj, 10, 20, 20);
    gluDeleteQuadric(quadObj);
    glPopMatrix();
    
    
    
    //    glTranslatef(2,0.0,-5);
    //    glutSolidSphere(1, 10, 20);
    glPushMatrix();
    glTranslatef(camera.center.x, camera.center.y, camera.center.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glScaled(0.001, 0.001, 0.001);
    glutSolidSphere(5, 25, 25);
    glPopMatrix();
    glPushMatrix();
    RenderGround();
    glPopMatrix();
    glFlush();
//    glFlush();
}
void Anim() {
   
    if(jump){
        if(dir ==1){
            camera.moveY(0.1);
            if (camera.eye.y>=24.666) {
                dir=2;
            }
        }else if (dir == 2){
            camera.moveY(-0.1);
            if (camera.eye.y<=16.6755){
                dir=0;
                jump=false;
            }
            
        }
    }
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
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
    char* filename = "./earth-ConvertImage.bmp";
    char* filenamm = "./ground.bmp";
    loadTextureFromFile( filename );
    loadTextureFromFile( filenamm );
    glutMainLoop();
    return 0;

    
    }



