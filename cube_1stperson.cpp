#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>
#include <deque>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


SDL_Window* gWindow = NULL;
SDL_GLContext gContext;

float cameraX = 0.0f;
float cameraY = 0.0f;
float cameraZ = -5.0f;
float yaw = 0.0f;


void initSDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    gWindow = SDL_CreateWindow("QCMs First Person Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    gContext = SDL_GL_CreateContext(gWindow);
    SDL_GL_SetSwapInterval(1);

    //glEnable(GL_DEPTH_TEST); // Enable depth testing
    //glDepthFunc(GL_LESS);    // Use less-than depth test function
    //glEnable(GL_CULL_FACE);
}

void closeSDL()
{
    SDL_GL_DeleteContext(gContext);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}



void renderCube()
{
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);

    // Draw the red faces of the cube
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glBegin(GL_QUADS);
    // Front face
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Back face
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Left face
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Right face
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Top face
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    // Draw black lines across the cube
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    glBegin(GL_LINES);
    // Front face
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Back face
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Connect front and back faces
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    glPopMatrix();
}



void renderPlane()
{
    //glDisable(GL_CULL_FACE); //disable for the plane
    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();

    // Draw grid pattern
    glColor3f(0.0f, 0.0f, 0.0f); // Black color
    const float gridSize = 1.0f;
    const float gridExtent = 10.0f;
    glBegin(GL_LINES);
    for (float i = -gridExtent; i <= gridExtent; i += gridSize)
    {
        glVertex3f(i, 0.0f, -gridExtent);
        glVertex3f(i, 0.0f, gridExtent);
        glVertex3f(-gridExtent, 0.0f, i);
        glVertex3f(gridExtent, 0.0f, i);
    }
    glEnd();

    glPopMatrix();
    //glEnable(GL_CULL_FACE); //re enable after drawing plane
}


void render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, cameraX + sin(yaw), cameraY, cameraZ + cos(yaw), 0.0f, 1.0f, 0.0f); //for mouse
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderPlane();
    renderCube();

    SDL_GL_SwapWindow(gWindow);
}

void handleKeyboardEvent(const SDL_Event& event)
{
    switch (event.key.keysym.sym)
    {
        case SDLK_w:
            if (SDL_GetModState() & KMOD_SHIFT)
            {
                cameraX += 0.1f * sin(yaw);
                cameraZ += 0.1f * cos(yaw);
            }
            else
            {
                cameraX += 0.1f * sin(yaw);
                cameraZ += 0.1f * cos(yaw);
            }
            break;
        case SDLK_s:
            if (SDL_GetModState() & KMOD_SHIFT)
            {
                cameraX -= 0.1f * sin(yaw);
                cameraZ -= 0.1f * cos(yaw);
            }
            else
            {
                cameraX -= 0.1f * sin(yaw);
                cameraZ -= 0.1f * cos(yaw);
            }
            break;
        case SDLK_a:
            if (SDL_GetModState() & KMOD_SHIFT)
                yaw += 0.5f;
            else
                yaw += 0.1f;
            break;
        case SDLK_d:
            if (SDL_GetModState() & KMOD_SHIFT)
                yaw -= 0.5f;
            else
                yaw -= 0.1f;
            break;
    }
}

float getCameraYaw(SDL_Event& event)
{
    static int prevMouseX = 0;
    float sensitivity = 0.005f;

    if (event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        int deltaX = event.motion.x - prevMouseX;
        yaw += deltaX * sensitivity;
        prevMouseX = event.motion.x;
    }

    return yaw;
}

void updateFaceCulling(const glm::vec3& objectPosition, float cameraX, float cameraY, float cameraZ, float yaw)
{
    // Calculate the camera position in world space
    glm::vec3 cameraPosition(cameraX, cameraY, cameraZ);

    // Calculate the vector from the camera to the object
    glm::vec3 cameraToObject = objectPosition - cameraPosition;

    // Calculate the angle between the camera's forward direction and the vector to the object
    float angle = atan2(cameraToObject.z, cameraToObject.x) - yaw;

    // Calculate the dot product of the camera-to-object vector and the camera's forward vector
    float dotProduct = glm::dot(cameraToObject, glm::vec3(sin(angle), 0.0f, cos(angle)));

    // Invert the dot product by multiplying it with -1.0
    dotProduct *= -1.0f;

    // Determine the face culling mode based on the inverted dot product
    if (dotProduct > 0.0f)
    {
        // Object is behind the camera, enable back face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {
        // Object is in front of the camera, enable front face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }
}



int main(int argc, char* args[])
{
    initSDL();

    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0) // Use `event` instead of `e`
        {
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN)
                handleKeyboardEvent(event);
        }
        
        // Get the center coordinates of the window
        int centerX = SCREEN_WIDTH / 2;
        int centerY = SCREEN_HEIGHT / 2;

        // Get the yaw angle from your camera
        float yaw = getCameraYaw(event);

        glm::vec3 objectPosition(0.0f, 0.0f, 0.0f); // Replace with the actual object position
        //updateFaceCulling(objectPosition, cameraX, cameraY, cameraZ, yaw);
        render();
    
    }

    closeSDL();

    return 0;
}
