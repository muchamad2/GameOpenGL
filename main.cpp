#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#define Pi 3.14159265f
/*
*/
using namespace std;
char title[] = "Game OpenGL";
//ukuran layar window
int windowWidth = 480;
int windowHeight = 560;
//posisi window
int windowPosX = 0;
int windowPosY = 0;

int refreshMilis = 30;
//screen are
GLfloat clipAreaXLeft, clipAreaXRight,clipAreaYbuttom,clipAreaYtop;
//ball variable
GLfloat ballradius = 0.1f;
GLfloat ballX = 0.0f,ballX2 = 0.0f;
GLfloat ballXmax, ballYmax, ballXmin, ballYmin;
GLfloat ySpeed2 = 2.5f, ySpeed = 2.0f;

//gravitasi
GLfloat gravity = -9.8;
const GLfloat e = 0.8f;
//movement
GLfloat moveX,moveY;
int counter,counter2;
float pos = 0.0f;
float pos2 = 0.7f;
bool running = true;
int position[] = {7,0,-7};
void init()
{
    glClearColor(1.0f,0.0f,1.0f,1.0f);
}
void print(string _string,GLfloat x,GLfloat y)
{
    glRasterPos2f(x,y);
    int i;
    int len = _string.length();
    glColor3f(0,1,0);
    for(i=0;i<len;i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,_string[i]);
    }
}
int score, life = 4;
void collision()
{

    float deltaX = ballX - moveX;
    deltaX *= deltaX;
    float deltaY = ySpeed - moveY;
    deltaY *= deltaY;
    float deltaX2 = ballX2 - moveX;
    deltaX2 *= deltaX2;
    float deltaY2 = ySpeed2 - moveY;
    deltaY2 *= deltaY2;
    float radian = ballradius + ballradius;
    radian *= radian;
    if(deltaX + deltaY <= radian){
        counter+=1;
        if(counter >= 4)
        {
            life-=1;
            counter=0;
        }
    }
    if(deltaX2 + deltaY2 <= radian){
        counter2 += 1;
        if(counter2 >= 4)
        {
            life-=1;
            counter2 = 0;
        }
    }
}
void keyboard(unsigned char key,int x,int y)
{
    if(key == 'a')
        moveX += -0.7f;
    else if(key == 'd')
        moveX += 0.7f;
}
bool first = true;
void CreateBall(float x)
{
    glTranslatef(x,ySpeed,0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    int numSegment = 100;
    GLfloat angle;
    for(int i = 0; i<=numSegment; i++)
    {
        angle = i * 2.0f * Pi / numSegment;
        glVertex2f(cos(angle)*ballradius,sin(angle)*ballradius);
    }
    glEnd();

    ballX = x;
    float last = pos;
    if(ySpeed < ballYmin){
        int ran = rand() % 3;
        pos = position[ran]*0.1;
        while(last == pos){
            ran = rand() % 3;
            pos = position[ran]*0.1;
        }
        ySpeed = 2.0f;
        score+=1;
    }
}
void CreateBall2(float x)
{
    glTranslatef(x,ySpeed2,0);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    int numSegment = 100;
    GLfloat angle;
    for(int i = 0; i<=numSegment; i++)
    {
        angle = i * 2.0f * Pi / numSegment;
        glVertex2f(cos(angle)*ballradius,sin(angle)*ballradius);
    }
    glEnd();
    ballX2 = x;
    float last = pos;
    if(ySpeed2 < ballYmin){
        int ran = rand() % 3;
        pos2 = position[ran]*0.1;
        while(last == pos2){
            ran = rand() % 3;
            pos2 = position[ran]*0.1;
        }
        ySpeed2 = 2.5f;
        score += 1;
    }
}
void createBalok(float positiony)
{

    glTranslatef(moveX,positiony,0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f,0.0f,0.0f);
    glVertex2f(0.0f,0.0f);
    int numSegment = 100;
    GLfloat angle;
    for(int i = 0; i<=numSegment; i++)
    {
        angle = i * 2.0f * Pi / numSegment;
        glVertex2f(cos(angle)*ballradius,sin(angle)*ballradius);
    }
    glEnd();
    moveY = positiony;
    if(moveX < ballXmin){
        moveX = -0.7;
    }
    else if(moveX > ballXmax){
        moveX = 0.7;
    }
}
string numberTostring(int number){
    ostringstream stream;
    stream << number;
    return stream.str();
}
void finishCondition()
{
    if(life == 0){
        print("Game Finish",0,0);
        ofstream myfile;
        myfile.open("HighScore.txt");
        myfile<<"Score : ";
        myfile<<score;
        myfile.close();
        exit(0);

    }
}
void display()
{


    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    createBalok(-0.7);
    glPopMatrix();

    glPushMatrix();
    CreateBall(pos);
    glPopMatrix();

    if(pos2 == pos){
        if(pos == 0.7){
            glPushMatrix();
            CreateBall2(pos2-0.7);
            glPopMatrix();
        }
        else
        {
            glPushMatrix();
            CreateBall2(pos2+0.7);
            glPopMatrix();
        }
    }else{
        glPushMatrix();
        CreateBall2(pos2);
        glPopMatrix();
    }
    collision();
    string scr = "Score :"+numberTostring(score);
    string lf = "Life : "+numberTostring(life);
    print(scr,0.5,1);
    print(lf,-1,1);
    finishCondition();
    glutSwapBuffers();

}
void resharp(GLsizei width, GLsizei height)
{
    if(height == 0)
        height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(width >= height)
    {
        clipAreaXLeft = -1.0f * aspect;
        clipAreaXRight = 1.0f * aspect;
        clipAreaYbuttom = -1.0;
        clipAreaYtop = 1.0;
    }
    else{
        clipAreaXLeft = -1.0f;
        clipAreaXRight = 1.0f;
        clipAreaYbuttom = -1.0 / aspect;
        clipAreaYtop = 1.0/ aspect;
    }
    gluOrtho2D(clipAreaXLeft,clipAreaXRight,clipAreaYbuttom,clipAreaYtop);
    ballXmin = clipAreaXLeft + ballradius;
    ballXmax = clipAreaXRight - ballradius;
    ballYmin = clipAreaYbuttom + ballradius;
    ballYmax = clipAreaYtop - ballradius;
}
void Timer(int value)
{
    glutPostRedisplay();
    ySpeed += gravity * 1 /100;
    ySpeed2 += gravity * 1/100;
    glutTimerFunc(refreshMilis,Timer,0);
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(windowPosX,windowPosY);
    glutCreateWindow(title);
    if(running){
        glutDisplayFunc(display);
        glutTimerFunc(0,Timer,0);
        glutReshapeFunc(resharp);
        glutKeyboardFunc(keyboard);
        init();
        glutMainLoop();
    }
    return 0;
}
