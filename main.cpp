// =============================================
// STEP 1: BACKGROUND
// Sky + Grass + Road + Road Markings
// =============================================

#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <mmsystem.h>      // For audio playback
#include <MMSystem.h>      // Additional audio support
#pragma comment(lib, "winmm.lib")  // Link the Windows multimedia library

#define PI 3.1416f
// ========== TRANSFORMATION VARIABLES ==========
//GLfloat carX     = 0.0f;
    // car translation


// ========== CAR POSITIONS (each car moves separately) ==========
GLfloat car1X = -0.10f;    // red sedan starts from far left
GLfloat car2X = -0.55f;    // blue SUV starts from far left
GLfloat car3X = -0.90f;    // yellow sports starts from far left
GLfloat car4X = -1.15f;    // black truck starts from far left
GLfloat car5X = -1.15f;    // green hatchback starts from far left
GLfloat sunAngle = 0.0f;     // sun rotation
GLfloat scale    = 1.0f;     // cloud zoom in/out
int     zoom     = 1;        // zoom direction
int     animFlag = 0;        // 0=stopped, 1=running
GLfloat timeOfDay = 6.0f;
GLfloat queueShift = 0.0f;    // ⭐ start at 6 AM (sunrise)
float lampPulse = 0.0f;
int lampPulseDir = 1;
GLfloat laneUpper = 0.07f;
GLfloat laneMiddle = 0.00f;
GLfloat laneLower = -0.08f;
// ========== AUDIO VARIABLES ==========
bool audioPlaying = false;     // Track if audio is currently playing
bool engineStarted = false;    // Track engine sound state
float engineVolume = 0.5f;     // Engine volume level



void drawText(float x, float y, char text[]) {
    glRasterPos2f(x, y);
    for(int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}



// ========== AUDIO FUNCTIONS ==========
void playBackgroundMusic() {
    // Play background music (WAV file)
    // Place "background.wav" in your project folder
    PlaySound(TEXT("engine_sound.wav"), NULL, SND_ASYNC | SND_LOOP);
    audioPlaying = true;
}

void stopBackgroundMusic() {
    PlaySound(NULL, NULL, 0);
    audioPlaying = false;
}

void playEngineSound() {
    // Play engine sound looped
    PlaySound(TEXT("engine_sound.wav"), NULL, SND_ASYNC | SND_LOOP);
    engineStarted = true;
}

void stopEngineSound() {
    if(engineStarted) {
        PlaySound(NULL, NULL, 0);
        engineStarted = false;
    }
}

void playCarPassSound() {
    // Play short sound when car passes
    PlaySound(TEXT("car_pass.wav"), NULL, SND_ASYNC);
}

void playHornSound() {
    PlaySound(TEXT("horn.wav"), NULL, SND_ASYNC);
}















// ========== CC HELPER — draws any filled circle ==========
void CC(GLfloat cx, GLfloat cy, GLfloat r, GLfloat red, GLfloat green, GLfloat blue) {
    int i;
    int tringle2 = 40;
    GLfloat tp2 = 2.0f * PI;

    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for(i = 0; i <= tringle2; i++)
            glVertex2f(cx + r * cos(i * tp2 / tringle2),
                       cy + r * sin(i * tp2 / tringle2));
    glEnd();
}



 // ========== KEYBOARD ==========
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
    case 'r':              // ⭐ START animation
        animFlag = 1;
        break;
    case 'q':              // ⭐ STOP animation
        animFlag = 0;
        break;




    case 'h':              // HORN
        playHornSound();
        break;
    case 'm':              // Mute/Unmute music
        if(audioPlaying) {
            stopBackgroundMusic();
            stopEngineSound();
        } else {
            playBackgroundMusic();
            if(animFlag == 1) playEngineSound();
        }
        break;



    }
    glutPostRedisplay();
}


void update(int value) {
    if(animFlag == 1) {
        // 1) Cars on road
        //if(animFlag == 1) {
        // ⭐ Each car has its OWN speed
      // cars using different road lanes
car1X = car1X + 0.005f;     // upper lane
car2X = car2X + 0.007f;     // lower lane
car3X = car3X + 0.009f;     // middle lane
car4X = car4X + 0.002f;     // lower lane
car5X = car5X + 0.001f;     // upper lane

if(car1X >= 1.5f) car1X = -0.10f;
if(car2X >= 1.5f) car2X = -0.55f;
if(car3X >= 1.5f) car3X = -0.90f;
if(car4X >= 1.5f) car4X = -1.15f;
if(car5X >= 1.5f) car5X = -1.90f;
        // ... rest of your update (sun, clouds, time, queue) stays same ...

    //carX = carX + 0.005f;


       // if(carX >= 1.5f) carX = -0.5f;

        // 2) Sun
        //sunAngle = sunAngle + 1.0f;
        if(sunAngle >= 360.0f) sunAngle = 0.0f;

        // 3) Clouds
        if(zoom == 1) {
            scale = scale + 0.003f;
            if(scale >= 1.15f) { zoom = 0; scale = 1.15f; }
        } else {
            scale = scale - 0.003f;
            if(scale <= 0.90f) { zoom = 1; scale = 0.90f; }
        }

        // 4) Time
        timeOfDay = timeOfDay + 0.02f;
        if(timeOfDay >= 24.0f) timeOfDay = 0.0f;

        // 5) ⭐ QUEUE shifts slowly forward
        queueShift = queueShift + 0.0001f;
        if(queueShift >= 0.18f)
            queueShift = 0.0f;
        // mild lamp pulse animation
if (lampPulseDir == 1) {
    lampPulse = lampPulse + 0.0015f;
    if (lampPulse >= 0.025f) {
        lampPulse = 0.025f;
        lampPulseDir = 0;
    }
}
else {
    lampPulse = lampPulse - 0.0015f;
    if (lampPulse <= 0.0f) {
        lampPulse = 0.0f;
        lampPulseDir = 1;
    }
}
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}















void display() {
    // ========== ALL VARIABLES AT TOP ==========
    int i;
    int tringle2 = 40;
    GLfloat tp2 = 2.0f * PI;
    GLfloat cx, cy, r;
    GLfloat dashX;






    glClear(GL_COLOR_BUFFER_BIT);
     glLoadIdentity();

    // ========== SKY (light blue) ==========
    glColor3f(0.30f, 0.70f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f,  0.0f);
        glVertex2f( 1.0f,  0.0f);
        glVertex2f( 1.0f,  1.0f);
        glVertex2f(-1.0f,  1.0f);
    glEnd();

    // ========== GRASS top (green strip) ==========
    glColor3f(0.40f, 0.78f, 0.30f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, -0.50f);
        glVertex2f( 1.0f, -0.50f);
        glVertex2f( 1.0f,  0.0f);
        glVertex2f(-1.0f,  0.0f);
    glEnd();

    // ========== ROAD (dark grey) ==========
    glColor3f(0.20f, 0.20f, 0.22f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, -0.85f);
        glVertex2f( 1.0f, -0.85f);
        glVertex2f( 1.0f, -0.50f);
        glVertex2f(-1.0f, -0.50f);
    glEnd();

    // ========== GRASS bottom (green) ==========
    glColor3f(0.40f, 0.78f, 0.30f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, -1.0f);
        glVertex2f( 1.0f, -1.0f);
        glVertex2f( 1.0f, -0.92f);
        glVertex2f(-1.0f, -0.92f);
    glEnd();

    // ========== Footpath strip (grey, between road and grass) ==========
    glColor3f(0.62f, 0.62f, 0.62f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, -0.92f);
        glVertex2f( 1.0f, -0.92f);
        glVertex2f( 1.0f, -0.85f);
        glVertex2f(-1.0f, -0.85f);
    glEnd();

    // ========== ROAD MARKINGS (yellow dashes) ==========
    glColor3f(1.0f, 0.85f, 0.0f);
    for(dashX = -0.95f; dashX < 1.0f; dashX += 0.20f) {
        glBegin(GL_POLYGON);
            glVertex2f(dashX,        -0.71f);
            glVertex2f(dashX + 0.10f,-0.71f);
            glVertex2f(dashX + 0.10f,-0.69f);
            glVertex2f(dashX,        -0.69f);
        glEnd();
    }



     glColor3f(0.30f, 0.70f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);  glVertex2f(-1.0f, 1.0f);
    glEnd();



    // ========== SKY (changes by time of day) ==========
    GLfloat skyR, skyG, skyB;

    if(timeOfDay < 5.0f) {
        // Night (midnight to 5 AM)
        skyR = 0.05f; skyG = 0.05f; skyB = 0.20f;
    }
    else if(timeOfDay < 7.0f) {
        // Sunrise (5 AM to 7 AM) — orange/pink
        skyR = 0.95f; skyG = 0.55f; skyB = 0.25f;
    }
    else if(timeOfDay < 17.0f) {
        // Day (7 AM to 5 PM) — bright blue
        skyR = 0.30f; skyG = 0.70f; skyB = 1.00f;
    }
    else if(timeOfDay < 19.0f) {
        // Sunset (5 PM to 7 PM) — orange/red
        skyR = 0.95f; skyG = 0.40f; skyB = 0.20f;
    }
    else {
        // Night again (7 PM to 12 AM)
        skyR = 0.05f; skyG = 0.05f; skyB = 0.20f;
    }

    glColor3f(skyR, skyG, skyB);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
        glVertex2f(1.0f, 1.0f);  glVertex2f(-1.0f, 1.0f);
    glEnd();

    // ========== SUN (only during day, follows arc) ==========
    if(timeOfDay >= 5.0f && timeOfDay <= 19.0f) {
        // sun moves in arc from left to right
        GLfloat sunPos = (timeOfDay - 5.0f) / 14.0f;   // 0 to 1
        cx = -0.8f + sunPos * 1.6f;                     // x: left → right
        cy = 0.4f + sin(sunPos * PI) * 0.45f;           // y: arc up
        r  = 0.10f;

        // sun color changes — orange at sunrise/sunset, yellow at noon
        if(timeOfDay < 7.0f || timeOfDay > 17.0f)
            glColor3f(1.0f, 0.55f, 0.10f);    // orange (sunrise/sunset)
        else
            glColor3f(1.0f, 0.92f, 0.10f);    // yellow (day)

        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(cx, cy);
            for(i = 0; i <= tringle2; i++)
                glVertex2f(cx + r*cos(i*tp2/tringle2),
                           cy + r*sin(i*tp2/tringle2));
        glEnd();
    }

    // ========== MOON (only at night, follows arc) ==========
    if(timeOfDay < 5.0f || timeOfDay > 19.0f) {
        GLfloat moonT;
        if(timeOfDay < 5.0f)
            moonT = (timeOfDay + 5.0f) / 10.0f;
        else
            moonT = (timeOfDay - 19.0f) / 10.0f;

        cx = -0.8f + moonT * 1.6f;
        cy = 0.4f + sin(moonT * PI) * 0.45f;
        r  = 0.08f;

        // moon (white)
        glColor3f(0.95f, 0.95f, 0.85f);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(cx, cy);
            for(i = 0; i <= tringle2; i++)
                glVertex2f(cx + r*cos(i*tp2/tringle2),
                           cy + r*sin(i*tp2/tringle2));
        glEnd();

        // STARS (only at night) — sprinkled white dots
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);
            glVertex2f(-0.85f, 0.85f); glVertex2f(-0.55f, 0.92f);
            glVertex2f(-0.20f, 0.78f); glVertex2f( 0.10f, 0.88f);
            glVertex2f( 0.45f, 0.72f); glVertex2f( 0.75f, 0.90f);
            glVertex2f(-0.65f, 0.65f); glVertex2f( 0.00f, 0.68f);
            glVertex2f( 0.55f, 0.55f); glVertex2f(-0.30f, 0.55f);
        glEnd();
        glPointSize(1.0f);
    }

    // ========== CLOUDS ==========
    glColor3f(1.0f, 1.0f, 1.0f);
    r = 0.06f;
    cy = 0.82f;
    cx = -0.85f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.78f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.71f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    cy = 0.78f; cx = -0.30f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.22f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.14f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    cy = 0.80f; cx = 0.20f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = 0.27f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    cy = 0.78f; cx = 0.85f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = 0.92f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    // ========== BUILDINGS (city skyline) ==========
   // =====================================================
// CLEANER CITY BUILDINGS - Bangladesh style variation
// Put this after clouds and before grass
// =========================================================
// MORE REALISTIC & DETAILED BANGLADESH CITY BUILDINGS
// Paste after clouds and before grass
// =========================================================


// =========================================================
// BUILDING 1 : LEFT APARTMENT
// =========================================================
glColor3f(0.76f, 0.69f, 0.54f);
glBegin(GL_POLYGON);
    glVertex2f(-1.00f, 0.00f);
    glVertex2f(-0.86f, 0.00f);
    glVertex2f(-0.86f, 0.34f);
    glVertex2f(-1.00f, 0.34f);
glEnd();

// right side shadow
glColor3f(0.62f, 0.56f, 0.43f);
glBegin(GL_POLYGON);
    glVertex2f(-0.90f, 0.00f);
    glVertex2f(-0.86f, 0.00f);
    glVertex2f(-0.86f, 0.34f);
    glVertex2f(-0.90f, 0.34f);
glEnd();

// roof/parapet
glColor3f(0.58f, 0.50f, 0.36f);
glBegin(GL_POLYGON);
    glVertex2f(-1.00f, 0.34f);
    glVertex2f(-0.86f, 0.34f);
    glVertex2f(-0.86f, 0.37f);
    glVertex2f(-1.00f, 0.37f);
glEnd();

// balcony strips
glColor3f(0.55f, 0.47f, 0.34f);
glBegin(GL_POLYGON);
    glVertex2f(-1.00f, 0.22f);
    glVertex2f(-0.86f, 0.22f);
    glVertex2f(-0.86f, 0.24f);
    glVertex2f(-1.00f, 0.24f);
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(-1.00f, 0.10f);
    glVertex2f(-0.86f, 0.10f);
    glVertex2f(-0.86f, 0.12f);
    glVertex2f(-1.00f, 0.12f);
glEnd();

if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else
glColor3f(0.60f, 0.88f, 1.0f);
glBegin(GL_POLYGON); glVertex2f(-0.98f,0.25f); glVertex2f(-0.95f,0.25f); glVertex2f(-0.95f,0.30f); glVertex2f(-0.98f,0.30f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.93f,0.25f); glVertex2f(-0.90f,0.25f); glVertex2f(-0.90f,0.30f); glVertex2f(-0.93f,0.30f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.98f,0.13f); glVertex2f(-0.95f,0.13f); glVertex2f(-0.95f,0.18f); glVertex2f(-0.98f,0.18f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.93f,0.13f); glVertex2f(-0.90f,0.13f); glVertex2f(-0.90f,0.18f); glVertex2f(-0.93f,0.18f); glEnd();

// door
glColor3f(0.26f, 0.18f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(-0.965f, 0.00f);
    glVertex2f(-0.935f, 0.00f);
    glVertex2f(-0.935f, 0.07f);
    glVertex2f(-0.965f, 0.07f);
glEnd();


// =========================================================
// BUILDING 2 : TALL CITY BUILDING
// =========================================================
glColor3f(0.63f, 0.66f, 0.71f);
glBegin(GL_POLYGON);
    glVertex2f(-0.84f, 0.00f);
    glVertex2f(-0.69f, 0.00f);
    glVertex2f(-0.69f, 0.54f);
    glVertex2f(-0.84f, 0.54f);
glEnd();

// right shadow
glColor3f(0.48f, 0.50f, 0.55f);
glBegin(GL_POLYGON);
    glVertex2f(-0.73f, 0.00f);
    glVertex2f(-0.69f, 0.00f);
    glVertex2f(-0.69f, 0.54f);
    glVertex2f(-0.73f, 0.54f);
glEnd();

// roof room
glColor3f(0.42f, 0.44f, 0.48f);
glBegin(GL_POLYGON);
    glVertex2f(-0.80f, 0.54f);
    glVertex2f(-0.73f, 0.54f);
    glVertex2f(-0.73f, 0.62f);
    glVertex2f(-0.80f, 0.62f);
glEnd();

// antenna
glColor3f(0.15f, 0.15f, 0.15f);
glBegin(GL_POLYGON);
    glVertex2f(-0.768f, 0.62f);
    glVertex2f(-0.762f, 0.62f);
    glVertex2f(-0.762f, 0.74f);
    glVertex2f(-0.768f, 0.74f);
glEnd();

glLineWidth(2.0f);
glBegin(GL_LINES);
    glVertex2f(-0.765f, 0.70f);
    glVertex2f(-0.79f, 0.66f);

    glVertex2f(-0.765f, 0.70f);
    glVertex2f(-0.74f, 0.66f);
glEnd();

// floor bands
glColor3f(0.54f, 0.56f, 0.60f);
glBegin(GL_POLYGON); glVertex2f(-0.84f,0.40f); glVertex2f(-0.69f,0.40f); glVertex2f(-0.69f,0.42f); glVertex2f(-0.84f,0.42f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.84f,0.28f); glVertex2f(-0.69f,0.28f); glVertex2f(-0.69f,0.30f); glVertex2f(-0.84f,0.30f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.84f,0.16f); glVertex2f(-0.69f,0.16f); glVertex2f(-0.69f,0.18f); glVertex2f(-0.84f,0.18f); glEnd();

// windows
if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else
glColor3f(0.68f, 0.90f, 1.0f);
glBegin(GL_POLYGON); glVertex2f(-0.82f,0.44f); glVertex2f(-0.79f,0.44f); glVertex2f(-0.79f,0.50f); glVertex2f(-0.82f,0.50f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.77f,0.44f); glVertex2f(-0.74f,0.44f); glVertex2f(-0.74f,0.50f); glVertex2f(-0.77f,0.50f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.82f,0.32f); glVertex2f(-0.79f,0.32f); glVertex2f(-0.79f,0.38f); glVertex2f(-0.82f,0.38f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.77f,0.32f); glVertex2f(-0.74f,0.32f); glVertex2f(-0.74f,0.38f); glVertex2f(-0.77f,0.38f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.82f,0.20f); glVertex2f(-0.79f,0.20f); glVertex2f(-0.79f,0.26f); glVertex2f(-0.82f,0.26f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.77f,0.20f); glVertex2f(-0.74f,0.20f); glVertex2f(-0.74f,0.26f); glVertex2f(-0.77f,0.26f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.82f,0.08f); glVertex2f(-0.79f,0.08f); glVertex2f(-0.79f,0.14f); glVertex2f(-0.82f,0.14f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.77f,0.08f); glVertex2f(-0.74f,0.08f); glVertex2f(-0.74f,0.14f); glVertex2f(-0.77f,0.14f); glEnd();


// =========================================================
// BUILDING 3 : UNDER-CONSTRUCTION BUILDING
// =========================================================
glColor3f(0.82f, 0.76f, 0.60f);
glBegin(GL_POLYGON);
    glVertex2f(-0.66f, 0.00f);
    glVertex2f(-0.50f, 0.00f);
    glVertex2f(-0.50f, 0.48f);
    glVertex2f(-0.66f, 0.48f);
glEnd();

// right shadow
glColor3f(0.66f, 0.60f, 0.47f);
glBegin(GL_POLYGON);
    glVertex2f(-0.54f, 0.00f);
    glVertex2f(-0.50f, 0.00f);
    glVertex2f(-0.50f, 0.48f);
    glVertex2f(-0.54f, 0.48f);
glEnd();

// unfinished rods
glColor3f(0.18f, 0.18f, 0.18f);
glBegin(GL_POLYGON); glVertex2f(-0.64f,0.48f); glVertex2f(-0.635f,0.48f); glVertex2f(-0.635f,0.60f); glVertex2f(-0.64f,0.60f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.60f,0.48f); glVertex2f(-0.595f,0.48f); glVertex2f(-0.595f,0.62f); glVertex2f(-0.60f,0.62f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.56f,0.48f); glVertex2f(-0.555f,0.48f); glVertex2f(-0.555f,0.58f); glVertex2f(-0.56f,0.58f); glEnd();

// balcony/floor strips
glColor3f(0.62f, 0.54f, 0.40f);
glBegin(GL_POLYGON); glVertex2f(-0.66f,0.34f); glVertex2f(-0.50f,0.34f); glVertex2f(-0.50f,0.36f); glVertex2f(-0.66f,0.36f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.66f,0.22f); glVertex2f(-0.50f,0.22f); glVertex2f(-0.50f,0.24f); glVertex2f(-0.66f,0.24f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.66f,0.10f); glVertex2f(-0.50f,0.10f); glVertex2f(-0.50f,0.12f); glVertex2f(-0.66f,0.12f); glEnd();

// windows
if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else

glColor3f(0.62f, 0.88f, 1.0f);
glBegin(GL_POLYGON); glVertex2f(-0.63f,0.38f); glVertex2f(-0.60f,0.38f); glVertex2f(-0.60f,0.43f); glVertex2f(-0.63f,0.43f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.57f,0.38f); glVertex2f(-0.54f,0.38f); glVertex2f(-0.54f,0.43f); glVertex2f(-0.57f,0.43f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.63f,0.26f); glVertex2f(-0.60f,0.26f); glVertex2f(-0.60f,0.31f); glVertex2f(-0.63f,0.31f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.57f,0.26f); glVertex2f(-0.54f,0.26f); glVertex2f(-0.54f,0.31f); glVertex2f(-0.57f,0.31f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.63f,0.14f); glVertex2f(-0.60f,0.14f); glVertex2f(-0.60f,0.19f); glVertex2f(-0.63f,0.19f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.57f,0.14f); glVertex2f(-0.54f,0.14f); glVertex2f(-0.54f,0.19f); glVertex2f(-0.57f,0.19f); glEnd();


// =========================================================
// BUILDING 4 : BLUE GLASS OFFICE
// =========================================================
glColor3f(0.38f, 0.52f, 0.68f);
glBegin(GL_POLYGON);
    glVertex2f(-0.47f, 0.00f);
    glVertex2f(-0.30f, 0.00f);
    glVertex2f(-0.30f, 0.56f);
    glVertex2f(-0.47f, 0.56f);
glEnd();

// side shadow
glColor3f(0.26f, 0.38f, 0.52f);
glBegin(GL_POLYGON);
    glVertex2f(-0.34f, 0.00f);
    glVertex2f(-0.30f, 0.00f);
    glVertex2f(-0.30f, 0.56f);
    glVertex2f(-0.34f, 0.56f);
glEnd();

// water tank
glColor3f(0.10f, 0.10f, 0.12f);
glBegin(GL_POLYGON);
    glVertex2f(-0.43f, 0.56f);
    glVertex2f(-0.36f, 0.56f);
    glVertex2f(-0.36f, 0.64f);
    glVertex2f(-0.43f, 0.64f);
glEnd();

glBegin(GL_LINES);
    glVertex2f(-0.43f, 0.56f);
    glVertex2f(-0.45f, 0.52f);

    glVertex2f(-0.36f, 0.56f);
    glVertex2f(-0.34f, 0.52f);
glEnd();

// window rows
if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else

glColor3f(0.64f, 0.92f, 1.0f);
glBegin(GL_POLYGON); glVertex2f(-0.45f,0.45f); glVertex2f(-0.42f,0.45f); glVertex2f(-0.42f,0.51f); glVertex2f(-0.45f,0.51f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.40f,0.45f); glVertex2f(-0.37f,0.45f); glVertex2f(-0.37f,0.51f); glVertex2f(-0.40f,0.51f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.45f,0.34f); glVertex2f(-0.42f,0.34f); glVertex2f(-0.42f,0.40f); glVertex2f(-0.45f,0.40f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.40f,0.34f); glVertex2f(-0.37f,0.34f); glVertex2f(-0.37f,0.40f); glVertex2f(-0.40f,0.40f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.45f,0.23f); glVertex2f(-0.42f,0.23f); glVertex2f(-0.42f,0.29f); glVertex2f(-0.45f,0.29f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.40f,0.23f); glVertex2f(-0.37f,0.23f); glVertex2f(-0.37f,0.29f); glVertex2f(-0.40f,0.29f); glEnd();

glBegin(GL_POLYGON); glVertex2f(-0.45f,0.12f); glVertex2f(-0.42f,0.12f); glVertex2f(-0.42f,0.18f); glVertex2f(-0.45f,0.18f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.40f,0.12f); glVertex2f(-0.37f,0.12f); glVertex2f(-0.37f,0.18f); glVertex2f(-0.40f,0.18f); glEnd();


// =========================================================
// BUILDING 5 : SMALL SHOP / MIXED BUILDING
// =========================================================
glColor3f(0.66f, 0.54f, 0.44f);
glBegin(GL_POLYGON);
    glVertex2f(-0.26f, 0.00f);
    glVertex2f(-0.08f, 0.00f);
    glVertex2f(-0.08f, 0.34f);
    glVertex2f(-0.26f, 0.34f);
glEnd();

// shade strip
glColor3f(0.50f, 0.40f, 0.30f);
glBegin(GL_POLYGON);
    glVertex2f(-0.26f, 0.34f);
    glVertex2f(-0.08f, 0.34f);
    glVertex2f(-0.08f, 0.37f);
    glVertex2f(-0.26f, 0.37f);
glEnd();

// red shop sign
glColor3f(0.85f, 0.18f, 0.12f);
glBegin(GL_POLYGON);
    glVertex2f(-0.26f, 0.19f);
    glVertex2f(-0.08f, 0.19f);
    glVertex2f(-0.08f, 0.24f);
    glVertex2f(-0.26f, 0.24f);
glEnd();

// upper windows
if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else
glColor3f(0.66f, 0.90f, 1.0f);
glBegin(GL_POLYGON); glVertex2f(-0.23f,0.26f); glVertex2f(-0.20f,0.26f); glVertex2f(-0.20f,0.31f); glVertex2f(-0.23f,0.31f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.16f,0.26f); glVertex2f(-0.13f,0.26f); glVertex2f(-0.13f,0.31f); glVertex2f(-0.16f,0.31f); glEnd();

// shutter
glColor3f(0.45f, 0.45f, 0.48f);
glBegin(GL_POLYGON);
    glVertex2f(-0.23f, 0.00f);
    glVertex2f(-0.11f, 0.00f);
    glVertex2f(-0.11f, 0.16f);
    glVertex2f(-0.23f, 0.16f);
glEnd();


// =========================================================
// BUILDING 6 : MID-RIGHT APARTMENT
// =========================================================
glColor3f(0.72f, 0.66f, 0.54f);
glBegin(GL_POLYGON);
    glVertex2f(0.12f, 0.00f);
    glVertex2f(0.31f, 0.00f);
    glVertex2f(0.31f, 0.44f);
    glVertex2f(0.12f, 0.44f);
glEnd();

// side shadow
glColor3f(0.56f, 0.50f, 0.40f);
glBegin(GL_POLYGON);
    glVertex2f(0.27f, 0.00f);
    glVertex2f(0.31f, 0.00f);
    glVertex2f(0.31f, 0.44f);
    glVertex2f(0.27f, 0.44f);
glEnd();

// balcony lines
glColor3f(0.48f, 0.42f, 0.34f);
glBegin(GL_POLYGON); glVertex2f(0.12f,0.32f); glVertex2f(0.31f,0.32f); glVertex2f(0.31f,0.34f); glVertex2f(0.12f,0.34f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.12f,0.20f); glVertex2f(0.31f,0.20f); glVertex2f(0.31f,0.22f); glVertex2f(0.12f,0.22f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.12f,0.08f); glVertex2f(0.31f,0.08f); glVertex2f(0.31f,0.10f); glVertex2f(0.12f,0.10f); glEnd();

// windows
if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else
glColor3f(0.66f, 0.90f, 1.0f);
glBegin(GL_POLYGON); glVertex2f(0.15f,0.35f); glVertex2f(0.18f,0.35f); glVertex2f(0.18f,0.40f); glVertex2f(0.15f,0.40f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.23f,0.35f); glVertex2f(0.26f,0.35f); glVertex2f(0.26f,0.40f); glVertex2f(0.23f,0.40f); glEnd();

glBegin(GL_POLYGON); glVertex2f(0.15f,0.23f); glVertex2f(0.18f,0.23f); glVertex2f(0.18f,0.28f); glVertex2f(0.15f,0.28f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.23f,0.23f); glVertex2f(0.26f,0.23f); glVertex2f(0.26f,0.28f); glVertex2f(0.23f,0.28f); glEnd();

glBegin(GL_POLYGON); glVertex2f(0.15f,0.11f); glVertex2f(0.18f,0.11f); glVertex2f(0.18f,0.16f); glVertex2f(0.15f,0.16f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.23f,0.11f); glVertex2f(0.26f,0.11f); glVertex2f(0.26f,0.16f); glVertex2f(0.23f,0.16f); glEnd();


// =========================================================
// BUILDING 7 : FAR RIGHT BIG BUILDING
// =========================================================
glColor3f(0.58f, 0.60f, 0.64f);
glBegin(GL_POLYGON);
    glVertex2f(0.72f, 0.00f);
    glVertex2f(0.98f, 0.00f);
    glVertex2f(0.98f, 0.50f);
    glVertex2f(0.72f, 0.50f);
glEnd();

// right shadow
glColor3f(0.44f, 0.46f, 0.50f);
glBegin(GL_POLYGON);
    glVertex2f(0.92f, 0.00f);
    glVertex2f(0.98f, 0.00f);
    glVertex2f(0.98f, 0.50f);
    glVertex2f(0.92f, 0.50f);
glEnd();

// rooftop tank
glColor3f(0.10f, 0.10f, 0.12f);
glBegin(GL_POLYGON);
    glVertex2f(0.80f, 0.50f);
    glVertex2f(0.90f, 0.50f);
    glVertex2f(0.90f, 0.58f);
    glVertex2f(0.80f, 0.58f);
glEnd();

// window rows
if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else
glColor3f(0.64f, 0.90f, 1.0f);
glBegin(GL_POLYGON); glVertex2f(0.75f,0.39f); glVertex2f(0.78f,0.39f); glVertex2f(0.78f,0.45f); glVertex2f(0.75f,0.45f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.82f,0.39f); glVertex2f(0.85f,0.39f); glVertex2f(0.85f,0.45f); glVertex2f(0.82f,0.45f); glEnd();

glBegin(GL_POLYGON); glVertex2f(0.75f,0.28f); glVertex2f(0.78f,0.28f); glVertex2f(0.78f,0.34f); glVertex2f(0.75f,0.34f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.82f,0.28f); glVertex2f(0.85f,0.28f); glVertex2f(0.85f,0.34f); glVertex2f(0.82f,0.34f); glEnd();

glBegin(GL_POLYGON); glVertex2f(0.75f,0.17f); glVertex2f(0.78f,0.17f); glVertex2f(0.78f,0.23f); glVertex2f(0.75f,0.23f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.82f,0.17f); glVertex2f(0.85f,0.17f); glVertex2f(0.85f,0.23f); glVertex2f(0.82f,0.23f); glEnd();

glBegin(GL_POLYGON); glVertex2f(0.75f,0.06f); glVertex2f(0.78f,0.06f); glVertex2f(0.78f,0.12f); glVertex2f(0.75f,0.12f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.82f,0.06f); glVertex2f(0.85f,0.06f); glVertex2f(0.85f,0.12f); glVertex2f(0.82f,0.12f); glEnd();


// =========================================================
// SMALL STREET TREE IN FRONT OF BUILDINGS
// =========================================================
glColor3f(0.25f, 0.18f, 0.12f);
glBegin(GL_POLYGON);
    glVertex2f(0.40f, 0.00f);
    glVertex2f(0.42f, 0.00f);
    glVertex2f(0.42f, 0.16f);
    glVertex2f(0.40f, 0.16f);
glEnd();

glColor3f(0.10f, 0.55f, 0.18f);
glBegin(GL_TRIANGLES);
    glVertex2f(0.35f, 0.12f);
    glVertex2f(0.47f, 0.12f);
    glVertex2f(0.41f, 0.26f);
glEnd();

glBegin(GL_TRIANGLES);
    glVertex2f(0.36f, 0.06f);
    glVertex2f(0.46f, 0.06f);
    glVertex2f(0.41f, 0.18f);
glEnd();





    // ========== BUILDINGS ==========

    // ========== GRASS ==========
    glColor3f(0.40f, 0.78f, 0.30f);
    glBegin(GL_POLYGON); glVertex2f(-1.0f,-0.50f); glVertex2f(1.0f,-0.50f); glVertex2f(1.0f,0.0f); glVertex2f(-1.0f,0.0f); glEnd();

    // =============================================
    // FUEL STATION  ⭐ NEW IN STEP 4
    // =============================================
    // Canopy support poles (brown)
    glColor3f(0.40f, 0.28f, 0.18f);
    glBegin(GL_POLYGON); glVertex2f(0.55f,-0.50f); glVertex2f(0.58f,-0.50f); glVertex2f(0.58f,-0.15f); glVertex2f(0.55f,-0.15f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(0.78f,-0.50f); glVertex2f(0.81f,-0.50f); glVertex2f(0.81f,-0.15f); glVertex2f(0.78f,-0.15f); glEnd();

    // Canopy roof (blue)
    glColor3f(0.10f, 0.32f, 0.78f);
    glBegin(GL_POLYGON);
        glVertex2f(0.50f,-0.15f); glVertex2f(0.85f,-0.15f);
        glVertex2f(0.85f,-0.05f); glVertex2f(0.50f,-0.05f);
    glEnd();

    // Pump 1 — RED (octane)
    glColor3f(0.85f, 0.10f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(0.58f,-0.50f); glVertex2f(0.63f,-0.50f);
        glVertex2f(0.63f,-0.20f); glVertex2f(0.58f,-0.20f);
    glEnd();
    // pump 1 screen
    glColor3f(0.10f, 0.10f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(0.59f,-0.30f); glVertex2f(0.62f,-0.30f);
        glVertex2f(0.62f,-0.25f); glVertex2f(0.59f,-0.25f);
    glEnd();

    // Pump 2 — BLUE (diesel)
    glColor3f(0.10f, 0.30f, 0.85f);
    glBegin(GL_POLYGON);
        glVertex2f(0.65f,-0.50f); glVertex2f(0.70f,-0.50f);
        glVertex2f(0.70f,-0.20f); glVertex2f(0.65f,-0.20f);
    glEnd();
    // pump 2 screen
    glColor3f(0.10f, 0.10f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(0.66f,-0.30f); glVertex2f(0.69f,-0.30f);
        glVertex2f(0.69f,-0.25f); glVertex2f(0.66f,-0.25f);
    glEnd();

    // Pump 3 — GREEN (CNG)
    glColor3f(0.15f, 0.70f, 0.20f);
    glBegin(GL_POLYGON);
        glVertex2f(0.72f,-0.50f); glVertex2f(0.77f,-0.50f);
        glVertex2f(0.77f,-0.20f); glVertex2f(0.72f,-0.20f);
    glEnd();
    // pump 3 screen
    glColor3f(0.10f, 0.10f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(0.73f,-0.30f); glVertex2f(0.76f,-0.30f);
        glVertex2f(0.76f,-0.25f); glVertex2f(0.73f,-0.25f);
    glEnd();

    // ========== STATION SHOP (orange building, right) ==========
    glColor3f(1.0f, 0.65f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(0.85f,-0.50f); glVertex2f(1.00f,-0.50f);
        glVertex2f(1.00f, 0.00f); glVertex2f(0.85f, 0.00f);
    glEnd();
    // shop window
    glColor3f(0.55f, 0.85f, 0.95f);
    glBegin(GL_POLYGON);
        glVertex2f(0.87f,-0.20f); glVertex2f(0.94f,-0.20f);
        glVertex2f(0.94f,-0.05f); glVertex2f(0.87f,-0.05f);
    glEnd();
    // shop arch (decoration)
    glColor3f(0.10f, 0.30f, 0.78f);
    glBegin(GL_TRIANGLE_FAN);
        cx = 0.905f; cy = -0.36f; r = 0.04f;
        glVertex2f(cx, cy);
        for(i = 0; i <= 20; i++)
            glVertex2f(cx + r*cos(i*PI/20), cy + r*sin(i*PI/20));
    glEnd();
    // shop door
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_POLYGON);
        glVertex2f(0.96f,-0.50f); glVertex2f(0.99f,-0.50f);
        glVertex2f(0.99f,-0.30f); glVertex2f(0.96f,-0.30f);
    glEnd();
    // door knob
    glColor3f(0.95f, 0.85f, 0.10f);
    glBegin(GL_TRIANGLE_FAN);
        cx = 0.97f; cy = -0.40f; r = 0.005f;
        glVertex2f(cx, cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2));
    glEnd();












    // ========== ROAD ==========
    glColor3f(0.20f, 0.20f, 0.22f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f,-0.85f); glVertex2f(1.0f,-0.85f);
        glVertex2f(1.0f,-0.50f);  glVertex2f(-1.0f,-0.50f);
    glEnd();

    // ========== GRASS BOTTOM ==========
    glColor3f(0.40f, 0.78f, 0.30f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f,-1.0f);  glVertex2f(1.0f,-1.0f);
        glVertex2f(1.0f,-0.92f);  glVertex2f(-1.0f,-0.92f);
    glEnd();

    // ========== FOOTPATH ==========
    glColor3f(0.62f, 0.62f, 0.62f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f,-0.92f); glVertex2f(1.0f,-0.92f);
        glVertex2f(1.0f,-0.85f);  glVertex2f(-1.0f,-0.85f);
    glEnd();

    // ========== ROAD MARKINGS ==========
    glColor3f(1.0f, 0.85f, 0.0f);
    for(dashX = -0.95f; dashX < 1.0f; dashX += 0.20f) {
        glBegin(GL_POLYGON);
            glVertex2f(dashX,        -0.71f);
            glVertex2f(dashX + 0.10f,-0.71f);
            glVertex2f(dashX + 0.10f,-0.69f);
            glVertex2f(dashX,        -0.69f);
        glEnd();
    }





    // ========== CLOUDS ==========
    glColor3f(1.0f, 1.0f, 1.0f);
    r = 0.06f;
    cy = 0.82f; cx = -0.85f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.78f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.71f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    cy = 0.78f; cx = -0.30f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.22f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.14f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    cy = 0.80f; cx = 0.20f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = 0.27f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    cy = 0.78f; cx = 0.85f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = 0.92f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2),cy+r*sin(i*tp2/tringle2)); glEnd();

    // ========== BUILDINGS ==========



    // ========== GRASS ==========
    glColor3f(0.40f, 0.78f, 0.30f);
    glBegin(GL_POLYGON); glVertex2f(-1.0f,-0.50f); glVertex2f(1.0f,-0.50f); glVertex2f(1.0f,0.0f); glVertex2f(-1.0f,0.0f); glEnd();

    // ========== FUEL STATION ==========
    glColor3f(0.40f, 0.28f, 0.18f);
    glBegin(GL_POLYGON); glVertex2f(0.55f,-0.50f); glVertex2f(0.58f,-0.50f); glVertex2f(0.58f,-0.15f); glVertex2f(0.55f,-0.15f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(0.78f,-0.50f); glVertex2f(0.81f,-0.50f); glVertex2f(0.81f,-0.15f); glVertex2f(0.78f,-0.15f); glEnd();
    glColor3f(0.10f, 0.32f, 0.78f);
    glBegin(GL_POLYGON); glVertex2f(0.50f,-0.15f); glVertex2f(0.85f,-0.15f); glVertex2f(0.85f,-0.05f); glVertex2f(0.50f,-0.05f); glEnd();
    glColor3f(0.85f, 0.10f, 0.10f);
    glBegin(GL_POLYGON); glVertex2f(0.58f,-0.50f); glVertex2f(0.63f,-0.50f); glVertex2f(0.63f,-0.20f); glVertex2f(0.58f,-0.20f); glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    glBegin(GL_POLYGON); glVertex2f(0.59f,-0.30f); glVertex2f(0.62f,-0.30f); glVertex2f(0.62f,-0.25f); glVertex2f(0.59f,-0.25f); glEnd();
    glColor3f(0.10f, 0.30f, 0.85f);
    glBegin(GL_POLYGON); glVertex2f(0.65f,-0.50f); glVertex2f(0.70f,-0.50f); glVertex2f(0.70f,-0.20f); glVertex2f(0.65f,-0.20f); glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    glBegin(GL_POLYGON); glVertex2f(0.66f,-0.30f); glVertex2f(0.69f,-0.30f); glVertex2f(0.69f,-0.25f); glVertex2f(0.66f,-0.25f); glEnd();
    glColor3f(0.15f, 0.70f, 0.20f);
    glBegin(GL_POLYGON); glVertex2f(0.72f,-0.50f); glVertex2f(0.77f,-0.50f); glVertex2f(0.77f,-0.20f); glVertex2f(0.72f,-0.20f); glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    glBegin(GL_POLYGON); glVertex2f(0.73f,-0.30f); glVertex2f(0.76f,-0.30f); glVertex2f(0.76f,-0.25f); glVertex2f(0.73f,-0.25f); glEnd();

    // station shop
    glColor3f(1.0f, 0.65f, 0.10f);
    glBegin(GL_POLYGON); glVertex2f(0.85f,-0.50f); glVertex2f(1.00f,-0.50f); glVertex2f(1.00f,0.00f); glVertex2f(0.85f,0.00f); glEnd();
    glColor3f(0.55f, 0.85f, 0.95f);
    glBegin(GL_POLYGON); glVertex2f(0.87f,-0.20f); glVertex2f(0.94f,-0.20f); glVertex2f(0.94f,-0.05f); glVertex2f(0.87f,-0.05f); glEnd();
    glColor3f(0.10f, 0.30f, 0.78f);
    glBegin(GL_TRIANGLE_FAN);
        cx = 0.905f; cy = -0.36f; r = 0.04f;
        glVertex2f(cx, cy);
        for(i=0;i<=20;i++) glVertex2f(cx+r*cos(i*PI/20), cy+r*sin(i*PI/20));
    glEnd();
    glColor3f(0.40f, 0.25f, 0.15f);
    glBegin(GL_POLYGON); glVertex2f(0.96f,-0.50f); glVertex2f(0.99f,-0.50f); glVertex2f(0.99f,-0.30f); glVertex2f(0.96f,-0.30f); glEnd();

    // ========== ROAD ==========






    // ========== GRASS BOTTOM ==========
    glColor3f(0.40f, 0.78f, 0.30f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f,-1.0f);  glVertex2f(1.0f,-1.0f);
        glVertex2f(1.0f,-0.92f);  glVertex2f(-1.0f,-0.92f);
    glEnd();

    // ========== FOOTPATH ==========
    glColor3f(0.62f, 0.62f, 0.62f);
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f,-0.92f); glVertex2f(1.0f,-0.92f);
        glVertex2f(1.0f,-0.85f);  glVertex2f(-1.0f,-0.85f);
    glEnd();

    // ========== ROAD MARKINGS ==========
    glColor3f(1.0f, 0.85f, 0.0f);
    for(dashX = -0.95f; dashX < 1.0f; dashX += 0.20f) {
        glBegin(GL_POLYGON);
            glVertex2f(dashX,        -0.78f);
            glVertex2f(dashX + 0.10f,-0.78f);
            glVertex2f(dashX + 0.10f,-0.76f);
            glVertex2f(dashX,        -0.76f);
        glEnd();
    }


    // =============================================
    // ⭐ CARS WAITING ON GRASS — fuel queue!
    // These are parked on grass beside the road
    // (above the road, on green grass area)
    // =============================================
glPushMatrix();                         // ⭐ ADD THIS
    glTranslatef(queueShift, 0.0f, 0.0f);
    // ===== WAITING CAR 1 — ORANGE =====
    glColor3f(1.0f, 0.55f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.85f, -0.40f); glVertex2f(-0.70f, -0.40f);
        glVertex2f(-0.70f, -0.33f); glVertex2f(-0.85f, -0.33f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.82f, -0.33f); glVertex2f(-0.73f, -0.33f);
        glVertex2f(-0.75f, -0.27f); glVertex2f(-0.80f, -0.27f);
    glEnd();
    glColor3f(0.25f, 0.45f, 0.65f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.81f, -0.32f); glVertex2f(-0.74f, -0.32f);
        glVertex2f(-0.75f, -0.28f); glVertex2f(-0.80f, -0.28f);
    glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    cx = -0.82f; cy = -0.42f; r = 0.022f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.72f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();

    // ===== WAITING CAR 2 — PURPLE =====
    glColor3f(0.55f, 0.15f, 0.75f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.62f, -0.40f); glVertex2f(-0.47f, -0.40f);
        glVertex2f(-0.47f, -0.33f); glVertex2f(-0.62f, -0.33f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.59f, -0.33f); glVertex2f(-0.50f, -0.33f);
        glVertex2f(-0.52f, -0.27f); glVertex2f(-0.57f, -0.27f);
    glEnd();
    glColor3f(0.25f, 0.45f, 0.65f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.58f, -0.32f); glVertex2f(-0.51f, -0.32f);
        glVertex2f(-0.52f, -0.28f); glVertex2f(-0.57f, -0.28f);
    glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    cx = -0.59f; cy = -0.42f; r = 0.022f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.49f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();

    // ===== WAITING CAR 3 — TEAL =====
    glColor3f(0.10f, 0.65f, 0.75f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.39f, -0.40f); glVertex2f(-0.24f, -0.40f);
        glVertex2f(-0.24f, -0.33f); glVertex2f(-0.39f, -0.33f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.36f, -0.33f); glVertex2f(-0.27f, -0.33f);
        glVertex2f(-0.29f, -0.27f); glVertex2f(-0.34f, -0.27f);
    glEnd();
    glColor3f(0.25f, 0.45f, 0.65f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.35f, -0.32f); glVertex2f(-0.28f, -0.32f);
        glVertex2f(-0.29f, -0.28f); glVertex2f(-0.34f, -0.28f);
    glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    cx = -0.36f; cy = -0.42f; r = 0.022f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.26f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();

    // ===== WAITING CAR 4 — PINK =====
    glColor3f(0.95f, 0.40f, 0.60f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.16f, -0.40f); glVertex2f(-0.01f, -0.40f);
        glVertex2f(-0.01f, -0.33f); glVertex2f(-0.16f, -0.33f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(-0.13f, -0.33f); glVertex2f(-0.04f, -0.33f);
        glVertex2f(-0.06f, -0.27f); glVertex2f(-0.11f, -0.27f);
    glEnd();
    glColor3f(0.25f, 0.45f, 0.65f);
    glBegin(GL_POLYGON);
        glVertex2f(-0.12f, -0.32f); glVertex2f(-0.05f, -0.32f);
        glVertex2f(-0.06f, -0.28f); glVertex2f(-0.11f, -0.28f);
    glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    cx = -0.13f; cy = -0.42f; r = 0.022f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = -0.03f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();

    // ===== WAITING CAR 5 — DARK BLUE =====
    glColor3f(0.05f, 0.15f, 0.55f);
    glBegin(GL_POLYGON);
        glVertex2f(0.07f, -0.40f); glVertex2f(0.22f, -0.40f);
        glVertex2f(0.22f, -0.33f); glVertex2f(0.07f, -0.33f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.10f, -0.33f); glVertex2f(0.19f, -0.33f);
        glVertex2f(0.17f, -0.27f); glVertex2f(0.12f, -0.27f);
    glEnd();
    glColor3f(0.25f, 0.45f, 0.65f);
    glBegin(GL_POLYGON);
        glVertex2f(0.11f, -0.32f); glVertex2f(0.18f, -0.32f);
        glVertex2f(0.17f, -0.28f); glVertex2f(0.12f, -0.28f);
    glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    cx = 0.10f; cy = -0.42f; r = 0.022f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = 0.20f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();

    // ===== WAITING CAR 6 — BROWN =====
    glColor3f(0.50f, 0.30f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(0.30f, -0.40f); glVertex2f(0.45f, -0.40f);
        glVertex2f(0.45f, -0.33f); glVertex2f(0.30f, -0.33f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex2f(0.33f, -0.33f); glVertex2f(0.42f, -0.33f);
        glVertex2f(0.40f, -0.27f); glVertex2f(0.35f, -0.27f);
    glEnd();
    glColor3f(0.25f, 0.45f, 0.65f);
    glBegin(GL_POLYGON);
        glVertex2f(0.34f, -0.32f); glVertex2f(0.41f, -0.32f);
        glVertex2f(0.40f, -0.28f); glVertex2f(0.35f, -0.28f);
    glEnd();
    glColor3f(0.10f, 0.10f, 0.10f);
    cx = 0.33f; cy = -0.42f; r = 0.022f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    cx = 0.43f;
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx, cy);
    for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();


      glPopMatrix();


  // ========== NO FUEL BOARD ==========

  if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else
glColor3f(0.90f, 0.05f, 0.05f);   // Red board
glBegin(GL_POLYGON);
    glVertex2f(0.52f, -0.02f);
    glVertex2f(0.72f, -0.02f);
    glVertex2f(0.72f,  0.08f);
    glVertex2f(0.52f,  0.08f);
glEnd();

// board stand
glColor3f(0.20f, 0.20f, 0.20f);
glBegin(GL_POLYGON);
    glVertex2f(0.61f, -0.20f);
    glVertex2f(0.63f, -0.20f);
    glVertex2f(0.63f, -0.02f);
    glVertex2f(0.61f, -0.02f);
glEnd();

glColor3f(1.0f, 1.0f, 1.0f);
drawText(0.545f, 0.015f, "NO FUEL");


 // =============================================
// STREET LAMPS ON BOTH SIDES OF ROAD
// Mild glow + reflection
// No helper function version
// =============================================

// light only at evening/night
if(timeOfDay < 6.5f || timeOfDay > 17.5f) {





    // ==============================
    // SOFT ROAD REFLECTIONS
    // simple flat polygons on street
    // ==============================
    glColor3f(0.55f, 0.48f, 0.22f);

    // upper side reflections
    glBegin(GL_POLYGON); glVertex2f(-0.90f,-0.58f); glVertex2f(-0.70f,-0.58f); glVertex2f(-0.74f,-0.62f); glVertex2f(-0.86f,-0.62f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(-0.50f,-0.58f); glVertex2f(-0.30f,-0.58f); glVertex2f(-0.34f,-0.62f); glVertex2f(-0.46f,-0.62f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(-0.10f,-0.58f); glVertex2f( 0.10f,-0.58f); glVertex2f( 0.06f,-0.62f); glVertex2f(-0.06f,-0.62f); glEnd();
    glBegin(GL_POLYGON); glVertex2f( 0.30f,-0.58f); glVertex2f( 0.50f,-0.58f); glVertex2f( 0.46f,-0.62f); glVertex2f( 0.34f,-0.62f); glEnd();
    glBegin(GL_POLYGON); glVertex2f( 0.70f,-0.58f); glVertex2f( 0.90f,-0.58f); glVertex2f( 0.86f,-0.62f); glVertex2f( 0.74f,-0.62f); glEnd();

    // lower side reflections
    glBegin(GL_POLYGON); glVertex2f(-0.90f,-0.78f); glVertex2f(-0.70f,-0.78f); glVertex2f(-0.74f,-0.82f); glVertex2f(-0.86f,-0.82f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(-0.50f,-0.78f); glVertex2f(-0.30f,-0.78f); glVertex2f(-0.34f,-0.82f); glVertex2f(-0.46f,-0.82f); glEnd();
    glBegin(GL_POLYGON); glVertex2f(-0.10f,-0.78f); glVertex2f( 0.10f,-0.78f); glVertex2f( 0.06f,-0.82f); glVertex2f(-0.06f,-0.82f); glEnd();
    glBegin(GL_POLYGON); glVertex2f( 0.30f,-0.78f); glVertex2f( 0.50f,-0.78f); glVertex2f( 0.46f,-0.82f); glVertex2f( 0.34f,-0.82f); glEnd();
    glBegin(GL_POLYGON); glVertex2f( 0.70f,-0.78f); glVertex2f( 0.90f,-0.78f); glVertex2f( 0.86f,-0.82f); glVertex2f( 0.74f,-0.82f); glEnd();
}


// =============================================
// UPPER SIDE LAMP POSTS
// =============================================

// lamp 1 upper
glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON); glVertex2f(-0.81f,-0.50f); glVertex2f(-0.79f,-0.50f); glVertex2f(-0.79f,-0.25f); glVertex2f(-0.81f,-0.25f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.85f,-0.50f); glVertex2f(-0.75f,-0.50f); glVertex2f(-0.75f,-0.47f); glVertex2f(-0.85f,-0.47f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.86f,-0.25f); glVertex2f(-0.74f,-0.25f); glVertex2f(-0.76f,-0.30f); glVertex2f(-0.84f,-0.30f); glEnd();

// lamp 2 upper
glBegin(GL_POLYGON); glVertex2f(-0.41f,-0.50f); glVertex2f(-0.39f,-0.50f); glVertex2f(-0.39f,-0.25f); glVertex2f(-0.41f,-0.25f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.45f,-0.50f); glVertex2f(-0.35f,-0.50f); glVertex2f(-0.35f,-0.47f); glVertex2f(-0.45f,-0.47f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.46f,-0.25f); glVertex2f(-0.34f,-0.25f); glVertex2f(-0.36f,-0.30f); glVertex2f(-0.44f,-0.30f); glEnd();

// lamp 3 upper
glBegin(GL_POLYGON); glVertex2f(-0.01f,-0.50f); glVertex2f( 0.01f,-0.50f); glVertex2f( 0.01f,-0.25f); glVertex2f(-0.01f,-0.25f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.05f,-0.50f); glVertex2f( 0.05f,-0.50f); glVertex2f( 0.05f,-0.47f); glVertex2f(-0.05f,-0.47f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.06f,-0.25f); glVertex2f( 0.06f,-0.25f); glVertex2f( 0.04f,-0.30f); glVertex2f(-0.04f,-0.30f); glEnd();

// lamp 4 upper
glBegin(GL_POLYGON); glVertex2f(0.39f,-0.50f); glVertex2f(0.41f,-0.50f); glVertex2f(0.41f,-0.25f); glVertex2f(0.39f,-0.25f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.35f,-0.50f); glVertex2f(0.45f,-0.50f); glVertex2f(0.45f,-0.47f); glVertex2f(0.35f,-0.47f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.34f,-0.25f); glVertex2f(0.46f,-0.25f); glVertex2f(0.44f,-0.30f); glVertex2f(0.36f,-0.30f); glEnd();

// lamp 5 upper
glBegin(GL_POLYGON); glVertex2f(0.79f,-0.50f); glVertex2f(0.81f,-0.50f); glVertex2f(0.81f,-0.25f); glVertex2f(0.79f,-0.25f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.75f,-0.50f); glVertex2f(0.85f,-0.50f); glVertex2f(0.85f,-0.47f); glVertex2f(0.75f,-0.47f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.74f,-0.25f); glVertex2f(0.86f,-0.25f); glVertex2f(0.84f,-0.30f); glVertex2f(0.76f,-0.30f); glEnd();


// =============================================
// LOWER SIDE LAMP POSTS
// =============================================

// lower lamps are shorter because they are near bottom side
// lamp 1 lower
glBegin(GL_POLYGON); glVertex2f(-0.81f,-0.95f); glVertex2f(-0.79f,-0.95f); glVertex2f(-0.79f,-0.72f); glVertex2f(-0.81f,-0.72f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.85f,-0.95f); glVertex2f(-0.75f,-0.95f); glVertex2f(-0.75f,-0.92f); glVertex2f(-0.85f,-0.92f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.86f,-0.72f); glVertex2f(-0.74f,-0.72f); glVertex2f(-0.76f,-0.77f); glVertex2f(-0.84f,-0.77f); glEnd();

// lamp 2 lower
glBegin(GL_POLYGON); glVertex2f(-0.41f,-0.95f); glVertex2f(-0.39f,-0.95f); glVertex2f(-0.39f,-0.72f); glVertex2f(-0.41f,-0.72f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.45f,-0.95f); glVertex2f(-0.35f,-0.95f); glVertex2f(-0.35f,-0.92f); glVertex2f(-0.45f,-0.92f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.46f,-0.72f); glVertex2f(-0.34f,-0.72f); glVertex2f(-0.36f,-0.77f); glVertex2f(-0.44f,-0.77f); glEnd();

// lamp 3 lower
glBegin(GL_POLYGON); glVertex2f(-0.01f,-0.95f); glVertex2f( 0.01f,-0.95f); glVertex2f( 0.01f,-0.72f); glVertex2f(-0.01f,-0.72f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.05f,-0.95f); glVertex2f( 0.05f,-0.95f); glVertex2f( 0.05f,-0.92f); glVertex2f(-0.05f,-0.92f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.06f,-0.72f); glVertex2f( 0.06f,-0.72f); glVertex2f( 0.04f,-0.77f); glVertex2f(-0.04f,-0.77f); glEnd();

// lamp 4 lower
glBegin(GL_POLYGON); glVertex2f(0.39f,-0.95f); glVertex2f(0.41f,-0.95f); glVertex2f(0.41f,-0.72f); glVertex2f(0.39f,-0.72f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.35f,-0.95f); glVertex2f(0.45f,-0.95f); glVertex2f(0.45f,-0.92f); glVertex2f(0.35f,-0.92f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.34f,-0.72f); glVertex2f(0.46f,-0.72f); glVertex2f(0.44f,-0.77f); glVertex2f(0.36f,-0.77f); glEnd();

// lamp 5 lower
glBegin(GL_POLYGON); glVertex2f(0.79f,-0.95f); glVertex2f(0.81f,-0.95f); glVertex2f(0.81f,-0.72f); glVertex2f(0.79f,-0.72f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.75f,-0.95f); glVertex2f(0.85f,-0.95f); glVertex2f(0.85f,-0.92f); glVertex2f(0.75f,-0.92f); glEnd();
glBegin(GL_POLYGON); glVertex2f(0.74f,-0.72f); glVertex2f(0.86f,-0.72f); glVertex2f(0.84f,-0.77f); glVertex2f(0.76f,-0.77f); glEnd();


// =============================================
// LAMP GLASS / BULBS
// =============================================
if(timeOfDay < 6.5f || timeOfDay > 17.5f)
    glColor3f(1.0f, 0.95f, 0.45f);
else
    glColor3f(0.75f, 0.75f, 0.68f);

// upper bulbs
glBegin(GL_POLYGON); glVertex2f(-0.83f,-0.30f); glVertex2f(-0.77f,-0.30f); glVertex2f(-0.78f,-0.34f); glVertex2f(-0.82f,-0.34f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.43f,-0.30f); glVertex2f(-0.37f,-0.30f); glVertex2f(-0.38f,-0.34f); glVertex2f(-0.42f,-0.34f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.03f,-0.30f); glVertex2f( 0.03f,-0.30f); glVertex2f( 0.02f,-0.34f); glVertex2f(-0.02f,-0.34f); glEnd();
glBegin(GL_POLYGON); glVertex2f( 0.37f,-0.30f); glVertex2f( 0.43f,-0.30f); glVertex2f( 0.42f,-0.34f); glVertex2f( 0.38f,-0.34f); glEnd();
glBegin(GL_POLYGON); glVertex2f( 0.77f,-0.30f); glVertex2f( 0.83f,-0.30f); glVertex2f( 0.82f,-0.34f); glVertex2f( 0.78f,-0.34f); glEnd();

// lower bulbs
glBegin(GL_POLYGON); glVertex2f(-0.83f,-0.77f); glVertex2f(-0.77f,-0.77f); glVertex2f(-0.78f,-0.81f); glVertex2f(-0.82f,-0.81f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.43f,-0.77f); glVertex2f(-0.37f,-0.77f); glVertex2f(-0.38f,-0.81f); glVertex2f(-0.42f,-0.81f); glEnd();
glBegin(GL_POLYGON); glVertex2f(-0.03f,-0.77f); glVertex2f( 0.03f,-0.77f); glVertex2f( 0.02f,-0.81f); glVertex2f(-0.02f,-0.81f); glEnd();
glBegin(GL_POLYGON); glVertex2f( 0.37f,-0.77f); glVertex2f( 0.43f,-0.77f); glVertex2f( 0.42f,-0.81f); glVertex2f( 0.38f,-0.81f); glEnd();
glBegin(GL_POLYGON); glVertex2f( 0.77f,-0.77f); glVertex2f( 0.83f,-0.77f); glVertex2f( 0.82f,-0.81f); glVertex2f( 0.78f,-0.81f); glEnd();



   // =============================================
    // ⭐ CARS ON ROAD — each moves independently
    // =============================================


    // ===== CAR 1 — RED SEDAN (low and sleek) =====
    glPushMatrix();
    glTranslatef(car1X, laneUpper , 0.0f);
        // body bottom
        glColor3f(0.85f, 0.10f, 0.10f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.96f,-0.68f); glVertex2f(-0.76f,-0.68f);
            glVertex2f(-0.76f,-0.61f); glVertex2f(-0.96f,-0.61f);
        glEnd();
        // sleek cabin (curvy)
        glBegin(GL_POLYGON);
            glVertex2f(-0.90f,-0.61f); glVertex2f(-0.81f,-0.61f);
            glVertex2f(-0.83f,-0.55f); glVertex2f(-0.88f,-0.55f);
        glEnd();
        // hood (front lower)
        glBegin(GL_POLYGON);
            glVertex2f(-0.93f,-0.61f); glVertex2f(-0.91f,-0.61f);
            glVertex2f(-0.91f,-0.59f); glVertex2f(-0.93f,-0.59f);
        glEnd();
        // window
        glColor3f(0.30f, 0.55f, 0.75f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.89f,-0.60f); glVertex2f(-0.82f,-0.60f);
            glVertex2f(-0.83f,-0.56f); glVertex2f(-0.88f,-0.56f);
        glEnd();
        // door line
        glColor3f(0.55f, 0.05f, 0.05f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.86f,-0.68f); glVertex2f(-0.855f,-0.68f);
            glVertex2f(-0.855f,-0.61f); glVertex2f(-0.86f,-0.61f);
        glEnd();
        // headlight (yellow)
        glColor3f(1.0f, 0.95f, 0.40f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.76f,-0.66f); glVertex2f(-0.74f,-0.66f);
            glVertex2f(-0.74f,-0.63f); glVertex2f(-0.76f,-0.63f);
        glEnd();
        // tail light (red)
        glColor3f(1.0f, 0.20f, 0.20f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.96f,-0.66f); glVertex2f(-0.95f,-0.66f);
            glVertex2f(-0.95f,-0.63f); glVertex2f(-0.96f,-0.63f);
        glEnd();
        // mirror
        glColor3f(0.50f, 0.05f, 0.05f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.81f,-0.59f); glVertex2f(-0.79f,-0.59f);
            glVertex2f(-0.79f,-0.575f); glVertex2f(-0.81f,-0.575f);
        glEnd();
        // wheels
        glColor3f(0.10f, 0.10f, 0.10f);
        cx = -0.92f; cy = -0.70f; r = 0.028f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.80f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        // wheel rims (silver)
        glColor3f(0.65f, 0.65f, 0.70f);
        cx = -0.92f; cy = -0.70f; r = 0.012f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.80f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    glPopMatrix();

    // ===== CAR 2 — BLUE SUV (tall, boxy) =====
    glPushMatrix();
    glTranslatef(car2X, laneLower, 0.0f);
        // body bottom (taller)
        glColor3f(0.10f, 0.30f, 0.85f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.66f,-0.68f); glVertex2f(-0.46f,-0.68f);
            glVertex2f(-0.46f,-0.60f); glVertex2f(-0.66f,-0.60f);
        glEnd();
        // BIG cabin (boxy, taller)
        glBegin(GL_POLYGON);
            glVertex2f(-0.64f,-0.60f); glVertex2f(-0.48f,-0.60f);
            glVertex2f(-0.48f,-0.51f); glVertex2f(-0.64f,-0.51f);
        glEnd();
        // big windows (2 separate)
        glColor3f(0.30f, 0.55f, 0.75f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.63f,-0.59f); glVertex2f(-0.56f,-0.59f);
            glVertex2f(-0.56f,-0.52f); glVertex2f(-0.63f,-0.52f);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex2f(-0.55f,-0.59f); glVertex2f(-0.49f,-0.59f);
            glVertex2f(-0.49f,-0.52f); glVertex2f(-0.55f,-0.52f);
        glEnd();
        // roof rack (black)
        glColor3f(0.10f, 0.10f, 0.10f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.62f,-0.51f); glVertex2f(-0.50f,-0.51f);
            glVertex2f(-0.50f,-0.495f); glVertex2f(-0.62f,-0.495f);
        glEnd();
        // headlight
        glColor3f(1.0f, 0.95f, 0.40f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.46f,-0.66f); glVertex2f(-0.44f,-0.66f);
            glVertex2f(-0.44f,-0.62f); glVertex2f(-0.46f,-0.62f);
        glEnd();
        // door handle
        glColor3f(0.70f, 0.70f, 0.70f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.58f,-0.625f); glVertex2f(-0.55f,-0.625f);
            glVertex2f(-0.55f,-0.615f); glVertex2f(-0.58f,-0.615f);
        glEnd();
        // BIG wheels (SUV)
        glColor3f(0.10f, 0.10f, 0.10f);
        cx = -0.62f; cy = -0.70f; r = 0.035f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.50f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        // rims
        glColor3f(0.70f, 0.70f, 0.75f);
        cx = -0.62f; cy = -0.70f; r = 0.015f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.50f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    glPopMatrix();

    // ===== CAR 3 — YELLOW SPORTS CAR (very low, sleek) =====
    glPushMatrix();
    glTranslatef(car3X, laneMiddle, 0.0f);
        // VERY LOW body
        glColor3f(1.0f, 0.85f, 0.0f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.36f,-0.68f); glVertex2f(-0.16f,-0.68f);
            glVertex2f(-0.16f,-0.63f); glVertex2f(-0.36f,-0.63f);
        glEnd();
        // sleek aerodynamic top
        glBegin(GL_POLYGON);
            glVertex2f(-0.32f,-0.63f); glVertex2f(-0.20f,-0.63f);
            glVertex2f(-0.23f,-0.58f); glVertex2f(-0.29f,-0.58f);
        glEnd();
        // pointed nose (front)
        glBegin(GL_POLYGON);
            glVertex2f(-0.16f,-0.68f); glVertex2f(-0.13f,-0.66f);
            glVertex2f(-0.13f,-0.65f); glVertex2f(-0.16f,-0.63f);
        glEnd();
        // sleek window (low and wide)
        glColor3f(0.20f, 0.20f, 0.30f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.31f,-0.625f); glVertex2f(-0.21f,-0.625f);
            glVertex2f(-0.23f,-0.585f); glVertex2f(-0.29f,-0.585f);
        glEnd();
        // racing stripe (black)
        glColor3f(0.10f, 0.10f, 0.10f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.27f,-0.68f); glVertex2f(-0.25f,-0.68f);
            glVertex2f(-0.25f,-0.58f); glVertex2f(-0.27f,-0.58f);
        glEnd();
        // headlight (bright)
        glColor3f(1.0f, 1.0f, 0.70f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.16f,-0.665f); glVertex2f(-0.14f,-0.665f);
            glVertex2f(-0.14f,-0.640f); glVertex2f(-0.16f,-0.640f);
        glEnd();
        // exhaust pipe (back)
        glColor3f(0.55f, 0.55f, 0.55f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.37f,-0.67f); glVertex2f(-0.36f,-0.67f);
            glVertex2f(-0.36f,-0.65f); glVertex2f(-0.37f,-0.65f);
        glEnd();
        // SPORTS wheels (medium)
        glColor3f(0.05f, 0.05f, 0.05f);
        cx = -0.32f; cy = -0.70f; r = 0.030f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.20f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        // gold racing rims!
        glColor3f(0.95f, 0.78f, 0.20f);
        cx = -0.32f; cy = -0.70f; r = 0.014f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.20f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    glPopMatrix();

    // ===== CAR 4 — BLACK TRUCK (big, boxy, with cargo) =====
    glPushMatrix();
    glTranslatef(car4X, laneLower, 0.0f);
        // truck cab (front, smaller)
        glColor3f(0.15f, 0.15f, 0.18f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.05f,-0.68f); glVertex2f(0.10f,-0.68f);
            glVertex2f(0.10f,-0.55f); glVertex2f(-0.05f,-0.55f);
        glEnd();
        // truck cargo box (BIG back)
        glColor3f(0.55f, 0.45f, 0.30f);    // brown cargo
        glBegin(GL_POLYGON);
            glVertex2f(-0.18f,-0.68f); glVertex2f(-0.05f,-0.68f);
            glVertex2f(-0.05f,-0.50f); glVertex2f(-0.18f,-0.50f);
        glEnd();
        // cargo lines (boxes inside)
        glColor3f(0.40f, 0.30f, 0.20f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.17f,-0.59f); glVertex2f(-0.06f,-0.59f);
            glVertex2f(-0.06f,-0.585f); glVertex2f(-0.17f,-0.585f);
        glEnd();
        // truck cabin window
        glColor3f(0.30f, 0.55f, 0.75f);
        glBegin(GL_POLYGON);
            glVertex2f(-0.04f,-0.61f); glVertex2f(0.06f,-0.61f);
            glVertex2f(0.06f,-0.56f); glVertex2f(-0.04f,-0.56f);
        glEnd();
        // headlight
        glColor3f(1.0f, 0.95f, 0.40f);
        glBegin(GL_POLYGON);
            glVertex2f(0.10f,-0.66f); glVertex2f(0.12f,-0.66f);
            glVertex2f(0.12f,-0.62f); glVertex2f(0.10f,-0.62f);
        glEnd();
        // BIG truck wheels (3 wheels)
        glColor3f(0.05f, 0.05f, 0.05f);
        cx = -0.14f; cy = -0.70f; r = 0.035f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.07f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = 0.06f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        // rims
        glColor3f(0.50f, 0.50f, 0.50f);
        cx = -0.14f; cy = -0.70f; r = 0.015f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.07f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = 0.06f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        // smoke from exhaust
        glColor3f(0.70f, 0.70f, 0.70f);
        cx = -0.20f; cy = -0.60f; r = 0.018f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = -0.23f; cy = -0.56f; r = 0.022f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    glPopMatrix();

    // ===== CAR 5 — GREEN HATCHBACK (small, cute) =====
    glPushMatrix();
    glTranslatef(car5X, laneUpper, 0.0f);
        // body
        glColor3f(0.15f, 0.70f, 0.20f);
        glBegin(GL_POLYGON);
            glVertex2f(0.31f,-0.68f); glVertex2f(0.46f,-0.68f);
            glVertex2f(0.46f,-0.60f); glVertex2f(0.31f,-0.60f);
        glEnd();
        // ROUND cabin (hatchback shape — slopes back)
        glBegin(GL_POLYGON);
            glVertex2f(0.32f,-0.60f); glVertex2f(0.45f,-0.60f);
            glVertex2f(0.43f,-0.53f); glVertex2f(0.34f,-0.53f);
        glEnd();
        // back hatch slope
        glBegin(GL_POLYGON);
            glVertex2f(0.31f,-0.60f); glVertex2f(0.32f,-0.60f);
            glVertex2f(0.34f,-0.53f); glVertex2f(0.31f,-0.55f);
        glEnd();
        // window
        glColor3f(0.30f, 0.55f, 0.75f);
        glBegin(GL_POLYGON);
            glVertex2f(0.33f,-0.59f); glVertex2f(0.44f,-0.59f);
            glVertex2f(0.43f,-0.54f); glVertex2f(0.35f,-0.54f);
        glEnd();
        // door line
        glColor3f(0.08f, 0.45f, 0.12f);
        glBegin(GL_POLYGON);
            glVertex2f(0.385f,-0.68f); glVertex2f(0.39f,-0.68f);
            glVertex2f(0.39f,-0.60f); glVertex2f(0.385f,-0.60f);
        glEnd();
        // headlight (round)
        glColor3f(1.0f, 0.95f, 0.40f);
        cx = 0.455f; cy = -0.64f; r = 0.012f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        // mirror
        glColor3f(0.08f, 0.45f, 0.12f);
        glBegin(GL_POLYGON);
            glVertex2f(0.44f,-0.59f); glVertex2f(0.46f,-0.59f);
            glVertex2f(0.46f,-0.575f); glVertex2f(0.44f,-0.575f);
        glEnd();
        // wheels (small)
        glColor3f(0.10f, 0.10f, 0.10f);
        cx = 0.34f; cy = -0.70f; r = 0.025f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = 0.43f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        // rims
        glColor3f(0.65f, 0.65f, 0.70f);
        cx = 0.34f; cy = -0.70f; r = 0.011f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
        cx = 0.43f;
        glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
        for(i=0;i<=tringle2;i++) glVertex2f(cx+r*cos(i*tp2/tringle2), cy+r*sin(i*tp2/tringle2)); glEnd();
    glPopMatrix();








// =============================================
// CROWD ON THE GRASS NEAR THE PUMP
// Paste after pump and grass, before road/cars
// =============================================


// ================= PERSON 1 =================
glColor3f(0.84f, 0.64f, 0.45f);   // head
cx = 0.48f; cy = -0.28f; r = 0.018f;
glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=tringle2; i++)
        glVertex2f(cx + r*cos(i*tp2/tringle2), cy + r*sin(i*tp2/tringle2));
glEnd();

glColor3f(0.90f, 0.15f, 0.12f);   // shirt
glBegin(GL_POLYGON);
    glVertex2f(0.455f,-0.31f);
    glVertex2f(0.505f,-0.31f);
    glVertex2f(0.500f,-0.40f);
    glVertex2f(0.460f,-0.40f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);   // legs
glBegin(GL_POLYGON);
    glVertex2f(0.466f,-0.40f);
    glVertex2f(0.478f,-0.40f);
    glVertex2f(0.476f,-0.47f);
    glVertex2f(0.464f,-0.47f);
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(0.484f,-0.40f);
    glVertex2f(0.496f,-0.40f);
    glVertex2f(0.498f,-0.47f);
    glVertex2f(0.486f,-0.47f);
glEnd();


// ================= PERSON 2 =================
glColor3f(0.80f, 0.58f, 0.40f);
cx = 0.56f; cy = -0.26f; r = 0.018f;
glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=tringle2; i++)
        glVertex2f(cx + r*cos(i*tp2/tringle2), cy + r*sin(i*tp2/tringle2));
glEnd();

glColor3f(0.10f, 0.35f, 0.90f);
glBegin(GL_POLYGON);
    glVertex2f(0.535f,-0.29f);
    glVertex2f(0.585f,-0.29f);
    glVertex2f(0.580f,-0.38f);
    glVertex2f(0.540f,-0.38f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.546f,-0.38f);
    glVertex2f(0.558f,-0.38f);
    glVertex2f(0.556f,-0.45f);
    glVertex2f(0.544f,-0.45f);
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(0.564f,-0.38f);
    glVertex2f(0.576f,-0.38f);
    glVertex2f(0.578f,-0.45f);
    glVertex2f(0.566f,-0.45f);
glEnd();


// ================= PERSON 3 =================
glColor3f(0.82f, 0.60f, 0.42f);
cx = 0.64f; cy = -0.28f; r = 0.018f;
glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=tringle2; i++)
        glVertex2f(cx + r*cos(i*tp2/tringle2), cy + r*sin(i*tp2/tringle2));
glEnd();

glColor3f(0.12f, 0.70f, 0.22f);
glBegin(GL_POLYGON);
    glVertex2f(0.615f,-0.31f);
    glVertex2f(0.665f,-0.31f);
    glVertex2f(0.660f,-0.40f);
    glVertex2f(0.620f,-0.40f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.626f,-0.40f);
    glVertex2f(0.638f,-0.40f);
    glVertex2f(0.636f,-0.47f);
    glVertex2f(0.624f,-0.47f);
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(0.644f,-0.40f);
    glVertex2f(0.656f,-0.40f);
    glVertex2f(0.658f,-0.47f);
    glVertex2f(0.646f,-0.47f);
glEnd();


// ================= PERSON 4 =================
glColor3f(0.79f, 0.56f, 0.38f);
cx = 0.73f; cy = -0.27f; r = 0.018f;
glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=tringle2; i++)
        glVertex2f(cx + r*cos(i*tp2/tringle2), cy + r*sin(i*tp2/tringle2));
glEnd();

glColor3f(0.92f, 0.65f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.705f,-0.30f);
    glVertex2f(0.755f,-0.30f);
    glVertex2f(0.750f,-0.39f);
    glVertex2f(0.710f,-0.39f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.716f,-0.39f);
    glVertex2f(0.728f,-0.39f);
    glVertex2f(0.726f,-0.46f);
    glVertex2f(0.714f,-0.46f);
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(0.734f,-0.39f);
    glVertex2f(0.746f,-0.39f);
    glVertex2f(0.748f,-0.46f);
    glVertex2f(0.736f,-0.46f);
glEnd();


// ================= PERSON 5 =================
glColor3f(0.84f, 0.62f, 0.44f);
cx = 0.53f; cy = -0.40f; r = 0.017f;
glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=tringle2; i++)
        glVertex2f(cx + r*cos(i*tp2/tringle2), cy + r*sin(i*tp2/tringle2));
glEnd();

glColor3f(0.58f, 0.18f, 0.75f);
glBegin(GL_POLYGON);
    glVertex2f(0.507f,-0.43f);
    glVertex2f(0.553f,-0.43f);
    glVertex2f(0.550f,-0.51f);
    glVertex2f(0.510f,-0.51f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.516f,-0.51f);
    glVertex2f(0.528f,-0.51f);
    glVertex2f(0.526f,-0.56f);
    glVertex2f(0.514f,-0.56f);
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(0.534f,-0.51f);
    glVertex2f(0.546f,-0.51f);
    glVertex2f(0.548f,-0.56f);
    glVertex2f(0.536f,-0.56f);
glEnd();


// ================= PERSON 6 =================
glColor3f(0.80f, 0.58f, 0.40f);
cx = 0.67f; cy = -0.41f; r = 0.017f;
glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(i=0; i<=tringle2; i++)
        glVertex2f(cx + r*cos(i*tp2/tringle2), cy + r*sin(i*tp2/tringle2));
glEnd();

glColor3f(0.15f, 0.55f, 0.85f);
glBegin(GL_POLYGON);
    glVertex2f(0.647f,-0.44f);
    glVertex2f(0.693f,-0.44f);
    glVertex2f(0.690f,-0.52f);
    glVertex2f(0.650f,-0.52f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.656f,-0.52f);
    glVertex2f(0.668f,-0.52f);
    glVertex2f(0.666f,-0.57f);
    glVertex2f(0.654f,-0.57f);
glEnd();

glBegin(GL_POLYGON);
    glVertex2f(0.674f,-0.52f);
    glVertex2f(0.686f,-0.52f);
    glVertex2f(0.688f,-0.57f);
    glVertex2f(0.676f,-0.57f);
glEnd();


// =============================================
// SMALL FUEL CANS ON THE GRASS
// =============================================

// can 1
glColor3f(0.82f, 0.08f, 0.08f);
glBegin(GL_POLYGON);
    glVertex2f(0.59f,-0.56f);
    glVertex2f(0.63f,-0.56f);
    glVertex2f(0.63f,-0.49f);
    glVertex2f(0.59f,-0.49f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.60f,-0.49f);
    glVertex2f(0.62f,-0.49f);
    glVertex2f(0.62f,-0.47f);
    glVertex2f(0.60f,-0.47f);
glEnd();

// can 2
glColor3f(0.10f, 0.32f, 0.90f);
glBegin(GL_POLYGON);
    glVertex2f(0.73f,-0.55f);
    glVertex2f(0.77f,-0.55f);
    glVertex2f(0.77f,-0.48f);
    glVertex2f(0.73f,-0.48f);
glEnd();

glColor3f(0.10f, 0.10f, 0.10f);
glBegin(GL_POLYGON);
    glVertex2f(0.74f,-0.48f);
    glVertex2f(0.76f,-0.48f);
    glVertex2f(0.76f,-0.46f);
    glVertex2f(0.74f,-0.46f);
glEnd();










    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("Step 1 - Background");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
