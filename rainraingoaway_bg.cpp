#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

#define M_PI 3.14159265358979323846

struct RainDrop {
    float x, y;
    float length;
    float speed;
    float alpha;
};

const int NUM_RAINDROPS = 180;
std::vector<RainDrop> raindrops;

// -------------------- Helpers --------------------
void circle(float cx, float cy, float r, int segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * M_PI * i / segments;
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();
}


void drawWall() {
    glColor3f(0.95f, 0.97f, 0.99f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

void drawWindowFrame() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.60f, -0.22f);
    glVertex2f(0.60f, -0.22f);
    glVertex2f(0.60f, 0.67f);
    glVertex2f(-0.60f, 0.67f);
    glEnd();

    glColor3f(0.92f, 0.94f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(-0.57f, -0.20f);
    glVertex2f(0.57f, -0.20f);
    glVertex2f(0.57f, 0.64f);
    glVertex2f(-0.57f, 0.64f);
    glEnd();
}

void drawCurtains() {
    glColor3f(0.98f, 0.92f, 0.92f);
    // left curtain
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, 0.74f);
    glVertex2f(-0.70f, 0.74f);
    glVertex2f(-0.57f, 0.64f);
    glVertex2f(-0.57f, -0.20f);
    glVertex2f(-1.0f, -0.22f);
    glEnd();
    // right curtain
    glBegin(GL_POLYGON);
    glVertex2f(1.0f, 0.74f);
    glVertex2f(0.70f, 0.74f);
    glVertex2f(0.57f, 0.64f);
    glVertex2f(0.57f, -0.20f);
    glVertex2f(1.0f, -0.22f);
    glEnd();
}

void drawOutsideScene() {
    // sky
    glColor3f(0.76f, 0.86f, 0.95f);
    glBegin(GL_QUADS);
    glVertex2f(-0.57f, -0.20f);
    glVertex2f(0.57f, -0.20f);
    glVertex2f(0.57f, 0.64f);
    glVertex2f(-0.57f, 0.64f);
    glEnd();

    // biggest hill (smooth parabola)
    glColor3f(0.52f, 0.77f, 0.50f);
    glBegin(GL_POLYGON);
    for (float x = -0.57f; x <= 0.57f; x += 0.01f) {
        float y = -0.20f + 0.35f * (1.0f - pow(x / 0.57f, 2));
        glVertex2f(x, y);
    }
    glVertex2f(0.57f, -0.20f);
    glVertex2f(-0.57f, -0.20f);
    glEnd();

    // left hill
    glColor3f(0.44f, 0.70f, 0.42f);
    glBegin(GL_POLYGON);
    for (float a = 0; a <= M_PI; a += 0.02f) {
        glVertex2f(-0.30f + 0.25f * cosf(a), -0.19f + 0.20f * sinf(a));
    }
    glEnd();

    // right hill
    glBegin(GL_POLYGON);
    for (float a = 0; a <= M_PI; a += 0.02f) {
        glVertex2f(0.22f + 0.20f * cosf(a), -0.19f + 0.16f * sinf(a));
    }
    glEnd();

    // clouds
    glColor3f(0.93f, 0.95f, 0.97f);
    circle(-0.45f, 0.52f, 0.09f, 28);
    circle(-0.53f, 0.51f, 0.07f, 22);
    circle(-0.38f, 0.50f, 0.07f, 22);

    circle(0.0f, 0.56f, 0.11f, 28);
    circle(-0.09f, 0.54f, 0.08f, 20);
    circle(0.10f, 0.55f, 0.08f, 20);

    circle(0.47f, 0.50f, 0.08f, 26);
    circle(0.39f, 0.51f, 0.06f, 20);
    circle(0.52f, 0.50f, 0.06f, 20);
}

void drawWindowSill() {
    glColor3f(0.72f, 0.50f, 0.30f);
    glBegin(GL_QUADS);
    glVertex2f(-0.63f, -0.26f);
    glVertex2f(0.63f, -0.26f);
    glVertex2f(0.63f, -0.36f);
    glVertex2f(-0.63f, -0.36f);
    glEnd();

    glColor3f(0.84f, 0.64f, 0.40f);
    glBegin(GL_QUADS);
    glVertex2f(-0.63f, -0.26f);
    glVertex2f(0.63f, -0.26f);
    glVertex2f(0.63f, -0.24f);
    glVertex2f(-0.63f, -0.24f);
    glEnd();
}

// rain rain go away
void initRaindrops() {
    raindrops.resize(NUM_RAINDROPS);
    for (int i = 0; i < NUM_RAINDROPS; i++) {
        raindrops[i] = {
            -0.57f + static_cast<float>(rand()) / RAND_MAX * 1.14f,
            -0.20f + static_cast<float>(rand()) / RAND_MAX * 0.84f,
            0.06f + static_cast<float>(rand()) / RAND_MAX * 0.08f,
            0.004f + static_cast<float>(rand()) / RAND_MAX * 0.018f,
            0.45f + static_cast<float>(rand()) / RAND_MAX * 0.45f
        };
    }
}

void updateRain() {
    for (auto& r : raindrops) {
        r.y -= r.speed;
        r.x += 0.0004f * sinf(r.y * 60.0f);
        if (r.y < -0.20f) {
            r.x = -0.57f + static_cast<float>(rand()) / RAND_MAX * 1.14f;
            r.y = 0.64f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
        }
    }
}

void drawRain() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2.0f);
    for (const auto& r : raindrops) {
        glColor4f(0.65f, 0.75f, 0.88f, r.alpha);
        glBegin(GL_LINES);
        glVertex2f(r.x, r.y);
        glVertex2f(r.x + 0.01f, r.y - r.length);
        glEnd();
    }
    glDisable(GL_BLEND);
}

// display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawWall();
    drawCurtains();
    drawWindowFrame();
    drawOutsideScene();
    drawRain();
    drawWindowSill();
    glutSwapBuffers();
}

void timer(int v) {
    updateRain();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    srand((unsigned)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Rainy");
    glutFullScreen();
    glClearColor(0.95f, 0.97f, 0.99f, 1.0f);

    initRaindrops();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();
    return 0;
}
