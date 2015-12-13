#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <GLUT/glut.h>
#include <vector>
#include "RgbImage.h"
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

GLfloat xRotated, yRotated, zRotated;
GLuint   texture[6];
float rotAng;
bool jump=false;
int dir=0;
float eyeX = 75.89;
float eyeY = 1;
float eyeZ = 72.7;
float centerX = 75.22;
float centerY = 1;
float centerZ =  71.96;
float upX = -0.0472866;
float upY = 0.998881;
float upZ = 0.0f;
int fire=0;
int bullets=10;
int shift=0;
float shift2=-0.014;
#define DEG2RAD(a) (a * 0.0174532925)

void loadTextureFromFile(char *filename){
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
   
    RgbImage theTexMap( filename );
    
    // Pixel alignment: each row is word aligned (aligned to a 4 byte boundary)
    //    Therefore, no need to call glPixelStore( GL_UNPACK_ALIGNMENT, ... );
    if (filename[2]=='e') {
        glGenTextures(1, &texture[0]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
    }else if(filename[2]=='g'){
        glGenTextures(1, &texture[1]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[1]);
    }else if(filename[2]=='z'){
        glGenTextures(1, &texture[3]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[3]);
    }else if(filename[2]=='a'){
        glGenTextures(1, &texture[4]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[4]);
    }else if(filename[3]=='l'){
        glGenTextures(1, &texture[5]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[5]);
    }
    else{
        glGenTextures(1, &texture[2]);               // Create The Texture
        glBindTexture(GL_TEXTURE_2D, texture[2]);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    // Typical Texture Generation Using Data From The Bitmap
    
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData() );
}
class BlackHoles{
public:
    float x,y,z;
    
    void createBlackHole(float x,float y,float z){
        this->x=x;
        this->y=y;
        this->z=z;
    }
    void drawBlackHole(){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        
        glPushMatrix();
        glColor3f(1, 1, 1);
        GLUquadric *quadObj2 = gluNewQuadric();
        gluQuadricTexture(quadObj2, true);
        gluQuadricNormals(quadObj2, GLU_SMOOTH);
        glTranslatef(x, y, z);
        gluSphere(quadObj2, 1, 20, 20);
        gluDeleteQuadric(quadObj2);
        glPopMatrix();
    }
};
std::vector<BlackHoles> blackHoles;
class Aliens{
public:
    float x,y,z;
    int health;
    int blackhole;
    
    void createAlien(int blackhole){
        x=blackHoles.at(blackhole).x;
        y=blackHoles.at(blackhole).y-0.4;
        z=blackHoles.at(blackhole).z;
        health=100;
        this->blackhole=blackhole;
    }
    
    void drawAlien(){
        glPushMatrix();
        glTranslatef(x,y,z);
        glRotatef(110, 0, 1, 0);
        glScaled(0.08, 0.08, 0.08);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        
        glPushMatrix();
        glColor3f(1, 1, 1);
        GLUquadric *quadObj = gluNewQuadric();
        gluQuadricTexture(quadObj, true);
        gluQuadricNormals(quadObj, GLU_SMOOTH);
        glTranslatef(0,6,0);
        glRotatef(270, 1, 0, 0);
        gluSphere(quadObj, 3, 25, 25);
        gluDeleteQuadric(quadObj);
        glPopMatrix();
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        
        glPushMatrix();
        glColor3f(1, 1, 1);
        GLUquadric *quadObj2 = gluNewQuadric();
        gluQuadricTexture(quadObj2, true);
        gluQuadricNormals(quadObj2, GLU_SMOOTH);
        glTranslatef(0,0,0);
        glScaled(1, 3, 1);
        gluSphere(quadObj2, 3, 25, 25);
        gluDeleteQuadric(quadObj2);
        glPopMatrix();
        
        glPopMatrix();
        

        
    }
};
std::vector<Aliens> aliens;

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
void RenderHouse(){
    glDisable(GL_LIGHTING);	// Disable lighting
    glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
    glBindTexture(GL_TEXTURE_2D, texture[2]);	// Bind the ground texture
    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);	// Set quad normal direction.
    glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
    glVertex3f(0, 0, 0);
    glTexCoord2f(0, 5);
    glVertex3f(0, 5, 0);
    glTexCoord2f(5, 5);
    glVertex3f(5, 5, 0);
    glTexCoord2f(5, 0);
    glVertex3f(5, 0, 0);
    glEnd();
    glPopMatrix();
    //glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
    glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
    
}
void house(){
    //right side
    glPushMatrix();
    glColor3f(1, 1, 1);	// Dim the ground texture a bit
    glScaled(1, 0.5, 1);
    RenderHouse();
    glPopMatrix();
    //front right
    glPushMatrix();
    glColor3f(1, 1, 1);	// Dim the ground texture a bit
    glScaled(0.3, 0.5, 0.3);
    glTranslatef(16.75, 0, 5);
    glRotated(90, 0, 1, 0);
    RenderHouse();
    glPopMatrix();
    //front left
    glPushMatrix();
    glColor3f(1, 1, 1);	// Dim the ground texture a bit
    glScaled(0.3, 0.5, 0.3);
    glTranslatef(16.75, 0, 16.75);
    glRotated(90, 0, 1, 0);
    RenderHouse();
    glPopMatrix();
    //back inside
    glPushMatrix();
    glColor3f(0, 1, 1);	// Dim the ground texture a bit
    glScaled(1, 0.5, 1);
    glTranslatef(0.1, 0, 5);
    glRotated(90, 0, 1, 0);
    RenderHouse();
    glPopMatrix();
    //back outside
    glPushMatrix();
    glColor3f(1, 1, 1);	// Dim the ground texture a bit
    glScaled(1, 0.5, 1);
    glTranslatef(0, 0, 5);
    glRotated(90, 0, 1, 0);
    RenderHouse();
    glPopMatrix();
    //top
    glPushMatrix();
    glColor3f(1, 1, 1);	// Dim the ground texture a bit;
    glScaled(1, 0.5, 1);
    glTranslatef(0, 5, 0);
    glRotated(90, 1, 0, 0);
    RenderHouse();
    glPopMatrix();
    //left side
    
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(0, 0, 5);
    glScaled(1, 0.5, 1);
    RenderHouse();
    glPopMatrix();
    glColor3f(0, 0, 1);
    
}
class Player {
    
public:
    Vector3f eye, center, up;
    
    Player() {
        eye = Vector3f(eyeX, eyeY, eyeZ);
        center = Vector3f(centerX, centerY, centerZ);
        up = Vector3f(upX, upY, upZ);
    }
    void moveX(float d) {
        Vector3f right = up.cross(center - eye).unit();
        right.y=0;
        eye = eye + right * d;
        center = center + right * d;
    }
    void moveY(float d) {
        eye = eye + up.unit() * d;
        center = center + up.unit() * d;
    }
    void moveZ(float d) {
        Vector3f view = (center - eye).unit();
        view.y=0;
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
        gluLookAt(eye.x, eye.y, eye.z,center.x, center.y, center.z,up.x, up.y, up.z);
    }
};
Player player;
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
    
    player.look();
    
}
void Keyboard(unsigned char key, int x, int y) {
    float d = 2;
    switch (key) {
        case 'w':
            player.rotateX(d);
            break;
        case 's':
            player.rotateX(-d);
            break;
        case 'a':
            player.rotateY(d);
            break;
        case 'd':
            player.rotateY(-d);
            break;
        case ' ':
            jump=true;
            dir=1;
            break;
    }
    glutPostRedisplay();
}
void Special(int key, int x, int y) {
    float a = 0.5;
    switch (key) {
        case GLUT_KEY_UP:
            if (player.center.x>-80 && player.center.z<79 && player.center.x<80 && player.center.z>-79) {
//                if (player.eye.x>70 && player.eye.x<86.5 && player.eye.z<70 && player.eye.z>65) {
//                    if (player.center.x>70 && player.center.z<75 && player.center.x<86.5 && player.center.z>70) {
                        player.moveZ(a);
//                    }
//                }
            }
            break;
       case GLUT_KEY_DOWN:
            if (player.center.x>-80 && player.center.z<79 && player.center.x<80 && player.center.z>-79) {
            player.moveZ(-a);
            }
            break;
        case GLUT_KEY_LEFT:
            if (player.center.x>-80 && player.center.z<79 && player.center.x<80 && player.center.z>-79 ) {
                player.moveX(a);
            }
            break;
        case GLUT_KEY_RIGHT:
            if (player.center.x>-80 && player.center.z<79 && player.center.x<80 && player.center.z>-79) {
                player.moveX(-a);
            }
            break;
    }
    glutPostRedisplay();
}
void Mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (bullets>0) {
                bullets--;
                std::cout << bullets;
                //KILL ALIENS HERE
            }
            break;
            
        case GLUT_RIGHT_BUTTON:
            if (player.center.x>70 && player.center.z<75 && player.center.x<86.5 && player.center.z>70) {
                std::cout << "Reloaded!!";
                bullets=10;
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
void Timer(int value) {
    for (int i=0; i<blackHoles.size(); i++){
        Aliens a;
        a.createAlien(i);
        aliens.push_back(a);
    }
        glutPostRedisplay();
    glutTimerFunc(5 * 1000, Timer, 0);
}
void Timer2(int value) {
    BlackHoles b;
    b.createBlackHole(-5, 1, -5+shift);
    blackHoles.push_back(b);
    glutPostRedisplay();
    shift+=10;
    glutTimerFunc(60 * 1000, Timer2, 0);
}
void sky(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPushMatrix();
    glColor3f(1, 1, 1);
    GLUquadric *quadObj = gluNewQuadric();
    gluQuadricTexture(quadObj, true);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    glTranslatef(0.866025, 0.866025, 0.866025);
    glScaled(16, 16, 16);
    gluSphere(quadObj, 10, 20, 20);
    gluDeleteQuadric(quadObj);
    glPopMatrix();
}
void aim(){
    glPushMatrix();
    glTranslatef(player.center.x, player.center.y, player.center.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glScaled(0.001, 0.001, 0.001);
    glutSolidSphere(5, 25, 25);
    glPopMatrix();
    glPushMatrix();
    RenderGround();
    glPopMatrix();
}
void drawString (void * font, std::string s, float x, float y, float z){
    unsigned int i;
    glRasterPos3f(x, y, z);
    
    for (i = 0; i < s.length(); i++)
        glutBitmapCharacter (font, s[i]);
}
void Display(void) {
    setupCamera();
    
    setupLights();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // glLoadIdentity();
    
    
//    drawString(GLUT_BITMAP_HELVETICA_18, "Bullets ", -2, 0, -6);
//    s = std::to_string(bs);
//    drawString(GLUT_BITMAP_HELVETICA_18, s, -0.18, 0, -6);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    sky();
    aim();
    
    for (int i=0; i<blackHoles.size(); i++) {
        blackHoles.at(i).drawBlackHole();
    }

    
    for (int i=0; i<aliens.size(); i++) {
        aliens.at(i).drawAlien();
    }
    

    
    glPushMatrix();
    glTranslatef(75, 0, 75);
    glRotated(135, 0, 1, 0);
    house();
    glPopMatrix();
    
    
    
    glFlush();

}
void Anim() {
    if (fire>100) {
        fire=0;
    }
    fire++;
    if(jump){
        if(dir ==1){
            player.moveY(0.02);
            if (player.eye.y>=2.4) {
                dir=2;
            }
        }else if (dir == 2){
            player.moveY(-0.02);
            if (player.eye.y<=1){
                dir=0;
                jump=false;
            }
            
        }
    }
    for (int i=0; i<aliens.size(); i++) {
        aliens.at(i).x+=0.1;
        aliens.at(i).z+=0.1+ (shift2*aliens.at(i).blackhole);
    }
    glutPostRedisplay();
}
int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(150, 150);
    
    glutCreateWindow("Alien Shooter 2.0");
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutSpecialFunc(Special);
    glutTimerFunc(0, Timer2, 0);
    glutTimerFunc(0, Timer, 0);
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
    char* filename1 = "./earth-ConvertImage.bmp";
    char* filename2 = "./ground.bmp";
    char* filename3 = "./bricks.bmp";
    loadTextureFromFile( filename1 );
    loadTextureFromFile( filename2 );
    loadTextureFromFile( filename3 );
    char* filename4 = "./zlien.bmp";
    char* filename5 = "./alien.bmp";
    char* filename6 = "./blackwholebmp.bmp";
    loadTextureFromFile( filename4 );
    loadTextureFromFile( filename5 );
    loadTextureFromFile( filename6 );
    glutMainLoop();
    return 0;

    
    }



