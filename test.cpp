#include <GL/glut.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glm/glm.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// struct grammar {
// 	unordered_map<string, vector<pair<float, string> > > mapping;

// 	void insert(string x, string y) {
// 		mapping[x].push_back(make_pair(1.0f, y));
// 	}

// 	string get_next(string x) {
// 		double prob = rand()%1;
// 		double sum = mapping[x][0].first;
// 		string next = mapping[x][0].second;
// 		for (auto e : mapping[x]){
// 			if (prob <= sum) {
// 				return next;
// 			}
// 			else{
// 				next = e.second;
// 				sum += e.first;
// 			}
// 		}
// 		return "";
// 	}

// 	string gen(int n, string inp) {
// 		if (n == 0) {
// 	  		return inp;
// 		}
// 		else {
// 		  	string temp="";
// 		  	for (auto ch:inp) {
// 		    	// cout << to_string(ch) << "\n";
// 		    	string t;
// 		    	t.push_back(ch);
// 		    	temp += get_next(t);
// 		  	}
// 	  	return gen(n-1, temp);
// 		}
// 	}
// };

// struct tree {
// 	std::string tree_string;
//   	std::unordered_map<char, glm::mat4 > rotations;
//  	grammar g;
//   	std::vector<glm::mat4> positions;

//   	g.insert("B","F[dF[BlBl]][aFla[BlFl]][cFl[cFlac[Bl]]][bFl[bFa[Bl]]l]");
//     g.insert("a","a");
//     g.insert("b","b");
//     g.insert("c","c");
//     g.insert("d","d");
//     g.insert("l","l");
//     g.insert("[","[");
//     g.insert("]","]");
//     g.insert("F","F");

//  //    rotations['a'] = gen_rotation(20.0*DEG2RAD, 60.0*DEG2RAD,
//  -45.0*DEG2RAD);
//  //    rotations['b'] = gen_rotation(45.0*DEG2RAD,
//  45.0*DEG2RAD, 15.0*DEG2RAD);
//  //    rotations['c'] = gen_rotation(-25.0*DEG2RAD,
//  -180.0*DEG2RAD, 45.0*DEG2RAD);
//  //    rotations['d'] = gen_rotation(0.0*DEG2RAD, 70.0*DEG2RAD,
//  -90.0*DEG2RAD);

//  //    void gen_string(std::string start, int age) {
//  //    	tree_string = g.gen(age, start);
//  //  	}

// 	// glm::mat4 get_boneMatrix(int f2) {
// 	// 	float t = 1.0;
// 	// 	for (int i=1; i<f2; i++) {
// 	// 		t*=0.8;
// 	// 	}
// 	// 	glm::mat4 sc = getScaleMatrix(t, t, t);
// 	// 	return sc;
// 	// }

// };

GLuint makeaTree;
// float x,y,z;
// Start and end of camera movement
float eyeX, eyeY, eyeZ, lookX, lookY, lookZ, fieldOfView;

void makeCylinder(float height, float base) {
    glPushAttrib(GL_LIGHTING_BIT);           // saves current lighting stuff
    GLfloat ambient[4] = {0.32, 0.21, 0.12}; // ambient reflection
    GLfloat diffuse[4] = {0.32, 0.21, 0.12}; // diffuse reflection
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    GLUquadric* obj = gluNewQuadric();
    glColor3f(0.4f, 0.16, 0.16f);
    glPushMatrix();
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(obj, base, base * 0.7, height, 20, 20);
    glPopMatrix();
    glutSwapBuffers();

    glPopAttrib();
}

void makeSphere(float radius) {
    glPushAttrib(GL_LIGHTING_BIT);           // saves current lighting stuff
    GLfloat ambient[4] = {0.32, 0.21, 0.12}; // ambient reflection
    GLfloat diffuse[4] = {0.32, 0.21, 0.12}; // diffuse reflection
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    GLUquadric* obj = gluNewQuadric();
    glColor3f(0.4f, 0.16, 0.16f);
    glPushMatrix();
    gluSphere(obj, radius, 10, 10);
    glPopMatrix();
    glutSwapBuffers();

    glPopAttrib();
}

void makeLeaf(float radius) {
    glPushAttrib(GL_LIGHTING_BIT);           // saves current lighting stuff
    GLfloat ambient[4] = {0.12, 0.41, 0.12}; // ambient reflection
    GLfloat diffuse[4] = {0.12, 0.41, 0.12}; // diffuse reflection
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    GLUquadric* obj = gluNewQuadric();
    glColor3f(0.1f, 1.0, 0.2f);
    glPushMatrix();
    gluDisk(obj, 0, 0.25, 10, 10);
    glPopMatrix();
    glutSwapBuffers();

    glPopAttrib();
}

void addLimb(double height, double base) {
    makeSphere(base * 1.04);
    makeCylinder(height, base);
}

void makeTree(string treeString) {
    double len = 5, rad = 0.5;
    double angle, angle1, angle2, choose;
    vector<char> stack;
    for (int i = 0; i < strlen(treeString.c_str()); i++) {
        // angle1 = rand()%50 + 5;
        // angle2 = rand()%50 - 55;
        // choose = ((double) rand() / (RAND_MAX));
        // angle = choose > 0.5 ? angle1:angle2;
        angle = rand() % 100 - 50;
        switch (treeString[i]) {
        case 'F':
            addLimb(len, rad);
            glTranslatef(0.0, len, 0.0);
            break;
        case '[':
            glPushMatrix();
            len *= 0.8;
            rad *= 0.7;
            break;
        case 'a':
            glRotatef(angle, 1, 1, 0);
            break;
        case 'b':
            glRotatef(angle, 1, 1, 1);
            break;
        case 'c':
            glRotatef(angle, 0, 1, 1);
            break;
        case 'd':
            glRotatef(angle, 1, 0, 1);
            break;
        case ']':
            glPopMatrix();
            len /= 0.8;
            rad /= 0.7;
            break;
        case 'l':
            makeLeaf(rad);
            break;
        case 'B':
            break;
        }
    }
}

string expand(string base) {
    string expanded = "";
    for (int i = 0; i < strlen(base.c_str()); i++) {
        if (base[i] == 'B') {
            double rando = ((double)rand() / (RAND_MAX));
            if (rando > 0.2) {
                expanded +=
                    "F[dF[BlBl]][aFla[BlFl]][cFl[cFlac[Bl]]][bFl[bFa[Bl]]l]";
            }
            else {
                expanded += 'l';
            }
        }
        else {
            expanded += base[i];
        }
    }
    return expanded;
}

string max_expand(string base, int d) {
    string res = base;
    for (int i = 0; i < d; i++) {
        res = expand(res);
    }
    string cleaned = "";
    for (int i = 0; i < strlen(res.c_str()); i++) {
        if (res[i] == 'B' /*|| res[i] == 'l'*/) {
            cleaned += "[FlFl]";
        }
        // else if (res[i] = 'l') {
        // 	cleaned += "ll";
        // }
        else {
            cleaned += res[i];
        }
    }
    return res;
}

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    makeaTree = glGenLists(1);
    glNewList(makeaTree, GL_COMPILE);
    string root = "F[dF[BlBl]][aFla[BlFl]][cFl[cFlac[Bl]]][bFl[bFa[Bl]]l]";
    string expanded = max_expand(root, 5);
    makeTree(expanded);
    glEndList();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 122: // z - Reduce the field of view of the camera
        fieldOfView -= 5;
        glutPostRedisplay();
        break;
    case 120: // x - Increase the field of view of the camera
        fieldOfView += 5;
        glutPostRedisplay();
        break;
    case 100: // d - Increase camera X-coordinate
        eyeX++;
        break;
    case 102: // f - Decrease camera X-coordinate
        eyeX--;
        break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfView, 1.0, 1, 2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 1, 0);

    glCallList(makeaTree);
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLfloat)w / (GLfloat)h, 0.001, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, -8.0, -50.0);
}

int main(int argc, char** argv) {
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Tree - recursive");

    fieldOfView = 45;
    eyeX = 250;
    eyeY = 100;
    eyeZ = 100;
    lookX = 0;
    lookY = 15;
    lookZ = 0;

    // set the lighting
    glShadeModel(GL_SMOOTH);
    GLfloat lightP[4] = {0.0, 250.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightP);

    // set the ambient light colour
    GLfloat lightA[4] = {0.99, 0.99, 0.99, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightA);

    // set the specular light colour
    GLfloat lightS[4] = {0.99, 0.99, 0.99, 1.0};
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightS);

    // set the diffuse light colour
    GLfloat lightD[4] = {0.99, 0.99, 0.99, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightD);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
}
