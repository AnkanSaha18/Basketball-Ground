//
//  main.cpp
//  Basketball3
//
//  Created by Ankan Saha on 29/10/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "SpotLight.h"
#include "cube.h"
#include "triangle.h"
#include "stb_image.h"
#include "sphere.h"
#include "print_text.h"
#include "curve.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void bed(Shader& lightingShader, glm::mat4 alTogether, Cube& cube);


//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;

//// modelling transform
//float rotateAngle_X = 0.0;
//float rotateAngle_Y = 0.0;
//float rotateAngle_Z = 0.0;
//float rotateAxis_X = 0.0;
//float rotateAxis_Y = 0.0;
//float rotateAxis_Z = 1.0;
//float translate_X = 0.0;
//float translate_Y = 0.0;
//float translate_Z = 0.0;
//float scale_X = 0.5;
//float scale_Y = 0.5;
//float scale_Z = 0.5;



//Human information
float human_x_position = 0.0f;
float human_z_position = 0.0f;
float human_speed = 0.001f;
float horizontal_angle = 0.0f;   // must be in degree
float vertical_angle = 45.0f;


//Ball information
float ball_x_position = 0.0f;
float ball_y_position = 0.0f;
float ball_z_position = 0.0f;
float ball_velocity = 0.411998f;         // unit: m/s
float ball_del_x = 0.0f;
float ball_del_y = 0.0f;
float ball_del_z = 0.0f;
float ball_animation = false;
float ball_animation_speed = 0.03f;

const float g = .09; // unit: m/s^2
float u=0.f;


//Ring position
float ring_x_position = 1.3f;
float ring_y_position = 0.3f;
float ring_z_position = 0.0f;

//Error
float considerble_distance = 0.01f;

//Scoring
float score = 0;





//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 90.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//float eyeX = -60.0, eyeY = 25.0, eyeZ = 50.0;//R
//float lookAtX = 1.0, lookAtY = 0.0, lookAtZ = 0.0;
//glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
//BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// lights
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.3f,  0.5f,  1.3f),
    glm::vec3(1.3f,  0.5f,  -1.3f),
    glm::vec3(-1.3f,  0.5f,  1.3f),
    glm::vec3(-1.3f,  0.5f,  -1.3f)
};
PointLight pointlight1(
    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    1       // light number
);
PointLight pointlight2(
    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    2       // light number
);
PointLight pointlight3(
    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    3       // light number
);
PointLight pointlight4(
    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    4       // light number
);

DirLight dirLight(
    -0.2f, -1.0f, -0.3f,
    0.3f, 0.3f, 0.3f,
    0.65f,0.65f,0.65f,     // diffuse
    0.5f, 0.5f, 0.5f
);

SpotLight spotLight(
    0.0f, 2.0f, 0.0f,        // co-ordinate of spotlight
    0.0f, -1.0f, 0.0f,      // Direction of spot light
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,
    0.09f,
    0.032f,
    12.5f,
    15.0f
);




// light settings
bool dirLightOn = false;
bool pointLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;





//map<string, int>;
//string path1[100];
//path2[100];
//void fn (count, path1, path2) {
//    for i conunt:
//}

void delay(float secs)
{
    float end = clock() / CLOCKS_PER_SEC + secs;
    while ((clock() / CLOCKS_PER_SEC) < end);
}


void exampleDrawingFunction(Cube &cube,string &diffuseMapPath, string &specularMapPath, Shader &lightingShaderWithTexture)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    
    
    
    
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
}


void drawCourt(Cube &cube, Shader &lightingShaderWithTexture)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f, 0.0f, -0.75f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f, 0.01f, 1.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    
    
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

}

void drawBackboard(Cube &cube, Shader &lightingShaderWithTexture)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.4f, 0.3f, -0.09f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.10f, 0.18f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
    
    
    
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f, 0.3f, -0.09f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.10f, 0.18f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    
    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


}

void drawGallery(Triangle &triangle, Cube &cube, Shader &lightingShaderWithTexture)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    
    // back gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f-0.3f, 0.0f, -0.75f-0.3f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.25f, 1.5f+0.3f));
    model = translateMatrix * scaleMatrix * rotateYMatrix;
    
    triangle.drawTriangleWithTexture(lightingShaderWithTexture, model);
    
    
    // front gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.4f+0.3, 0.0f, 0.75f));
    rotateYMatrix = glm::rotate(identityMatrix, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.25f, 1.5f+0.3f));
    model = translateMatrix * scaleMatrix * rotateYMatrix;
    
    triangle.drawTriangleWithTexture(lightingShaderWithTexture, model);

    
    // left gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.4f+0.3f, 0.0f, -0.75f-0.3f));
    rotateYMatrix = glm::rotate(identityMatrix, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f+0.3f+0.3f, 0.25f, 0.3f));
    model = translateMatrix * scaleMatrix * rotateYMatrix;
    
    triangle.drawTriangleWithTexture(lightingShaderWithTexture, model);
}


void drawHuman(Shader& shader, glm::mat4 allTogether){
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    Cube cube(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 30);
//    Triangle triangle(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 30);
   
   //0,0
    //cube1 (Head)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.025f, 0.15f, -0.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
    model = allTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader, model, 1.0f, 0.0f, 0.0f);
    
    
    //cube2 (Body)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.05f, -0.05f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.1f, 0.1f));
    model = allTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader, model, 0.0f, 1.0f, 0.0f);
    
    //cube3 (Hands)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.05f, 0.05f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.07f, 0.02f));
    model = allTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader, model, 0.0f, 0.0f, 1.0f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.05f, -0.05f-0.02f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.07f, 0.02f));
    model = allTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader, model, 0.0f, 0.0f, 1.0f);

    //cube4 (Legs)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.0f, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.05f, 0.03f));
    model = allTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader, model, 0.5f, 0.3f, 0.1f);
    
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.0f, -0.01f-0.03f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.05f, 0.03f));
    model = allTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader, model, 0.5f, 0.3f, 0.1f);
}

void drawDirection(Shader& lightingShader)
{
    float gap = 0.05;
    float size_difference = 0.001;
    float height_of_ball = 0.1f;
    
    
    Sphere sphere1 = Sphere(1, 36, 18, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    glm::mat4 model, identityMatrix;
    glm::mat4 modelForSphere1 = glm::mat4(1.0f);
    modelForSphere1 =
    glm::translate(model, glm::vec3(human_x_position, height_of_ball, human_z_position)) * glm::rotate(identityMatrix, horizontal_angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, vertical_angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(model, glm::vec3(gap, 0.0f, 0.0f)) *
        glm::scale(identityMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
    sphere1.drawSphere(lightingShader, modelForSphere1);
    
    
    Sphere sphere2 = Sphere(1, 36, 18, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    model = glm::mat4(1.0f);
    glm::mat4 modelForSphere2 = glm::mat4(1.0f);
    modelForSphere2 = 
    glm::translate(model, glm::vec3(human_x_position, height_of_ball, human_z_position)) * glm::rotate(identityMatrix, horizontal_angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, vertical_angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(model, glm::vec3(gap*2, 0.0f, 0.0f)) * 
        glm::scale(identityMatrix, glm::vec3(0.01f-size_difference, 0.01f-size_difference, 0.01f-size_difference));
    sphere2.drawSphere(lightingShader, modelForSphere2);

    
    Sphere sphere3 = Sphere(1, 36, 18, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    model = glm::mat4(1.0f);
    glm::mat4 modelForSphere3 = glm::mat4(1.0f);
    modelForSphere3 = 
    glm::translate(model, glm::vec3(human_x_position, height_of_ball, human_z_position)) * glm::rotate(identityMatrix, horizontal_angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, vertical_angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(model, glm::vec3(gap*3, 0.0f, 0.0f)) * 
        glm::scale(identityMatrix, glm::vec3(0.01f-size_difference*2, 0.01f-size_difference*2, 0.01f-size_difference*2));
    sphere3.drawSphere(lightingShader, modelForSphere3);

    
    Sphere sphere4 = Sphere(1, 36, 18, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    model = glm::mat4(1.0f);
    glm::mat4 modelForSphere4 = glm::mat4(1.0f);
    modelForSphere4 =
    glm::translate(model, glm::vec3(human_x_position, height_of_ball, human_z_position)) * glm::rotate(identityMatrix, horizontal_angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, vertical_angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(model, glm::vec3(gap*4, 0.0f, 0.0f)) * 
        glm::scale(identityMatrix, glm::vec3(0.01f-size_difference*3, 0.01f-size_difference*3, 0.01f-size_difference*3));
    sphere4.drawSphere(lightingShader, modelForSphere4);
    
    
    Sphere sphere5 = Sphere(1, 36, 18, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    model = glm::mat4(1.0f);
    glm::mat4 modelForSphere5 = glm::mat4(1.0f);
    modelForSphere5 =
    glm::translate(model, glm::vec3(human_x_position, height_of_ball, human_z_position)) * glm::rotate(identityMatrix, horizontal_angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(identityMatrix, vertical_angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::translate(model, glm::vec3(gap*5, 0.0f, 0.0f)) * 
        glm::scale(identityMatrix, glm::vec3(0.01f-size_difference*4, 0.01f-size_difference*4, 0.01f-size_difference*4));
    sphere5.drawSphere(lightingShader, modelForSphere5);
    
}

void drawBasketBall(Shader& lightingShader)
{
    Sphere ball = Sphere(1, 36, 18, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 100);
    
    glm::mat4 model, identityMatrix;
    glm::mat4 modelForBall = glm::mat4(1.0f);
    modelForBall = glm::translate(model, glm::vec3(ball_x_position, ball_y_position, ball_z_position)) * glm::scale(identityMatrix, glm::vec3(0.04f, 0.04f, 0.04f));
    ball.drawSphere(lightingShader, modelForBall);
}

void checkScore()
{
    float distance = sqrt((ring_x_position-ball_x_position)*(ring_x_position-ball_x_position) + (ring_y_position-ball_y_position)*(ring_y_position-ball_y_position) + (ring_z_position-ball_z_position)*(ring_z_position-ball_z_position));
    
    cout<<"Ball distance from ring "<< distance<<endl;
    
    if(distance<=considerble_distance && ball_del_y<0)
    {
        score += 1;
        ball_animation = false;
//        vertical_angle = 0;
//        horizontal_angle = 0;
    }
}

void changeSpotLight(Shader& lightingShaderWithTexture)
{
    spotLight.direction = glm::vec3(human_x_position/2, -1, human_z_position/2);
    spotLight.setUpSpotLight(lightingShaderWithTexture);
}





int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Basketball", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShaderWithTexture("/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/vertexShaderForPhongShadingWithTexture.vs", "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/fragmentShaderForPhongShadingWithTexture.fs");
    Shader ourShader("/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/vertexShader.vs", "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/fragmentShader.fs");

    string diffuseMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/court.jpg";
    string specularMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/whiteBackground.png";


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    //making cube for court
    diffuseMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/court.jpg";
    specularMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/whiteBackground.png";
    Cube cubeCourt = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 30.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    // making cube for backboard
    diffuseMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/backboard.jpg";
    specularMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/whiteBackground.png";
    Cube cubeBackboard = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    
    
    // making Triangle for gallery
    diffuseMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/gallery.png";
    specularMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/whiteBackground.png";
    Triangle triangleGallery = Triangle(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    
    // making wall for gallery
    diffuseMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/gallery.png";
    specularMapPath = "/Users/ankansaha/Academic Courses/4-2/Lab/Graphics/Lab 4/Work/Basketball3/Basketball3/image/whiteBackground.png";
    Cube cubeGallery = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    
    
    

    

    
    
    
    
    
    

    Sphere ball = Sphere();

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
//        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        
        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        
        // Directional light
        dirLight.setUpDirLight(lightingShaderWithTexture);

        // Spot light
        spotLight.setUpSpotLight(lightingShaderWithTexture);

//        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
//        modelMatrixForContainer = glm::translate(model, glm::vec3(-0.45f, -0.4f, -2.8f));
//        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        
        
        
//        Drawing Everything
        
        drawCourt(cubeCourt, lightingShaderWithTexture);

        drawBackboard(cubeBackboard, lightingShaderWithTexture);

        drawGallery(triangleGallery, cubeGallery, lightingShaderWithTexture);
        
        drawRing(lightingShaderWithTexture,9.6, 2.64,0.5,0);
        
        drawHuman(ourShader, glm:: translate(identityMatrix, glm::vec3(human_x_position, 0.0f, human_z_position)) * glm::rotate(identityMatrix, horizontal_angle, glm::vec3(0.0f, 1.0f, 0.0f)));
        
        
//        Drawing Ball Direction
        drawDirection(lightingShaderWithTexture);
        
        

//       Setting Ball Position
        if (ball_animation==false) {
            ball_x_position = human_x_position;
            ball_y_position = 0.1;
            ball_z_position = human_z_position;
        }
        if (ball_animation==true)
        {
//             for x axis
            ball_del_x = ball_velocity * cos(glm::radians(vertical_angle)) * cos(glm::radians(horizontal_angle)) * ball_animation_speed;
            ball_x_position += ball_del_x;
            
//            for y axis
            ball_del_y = (u-0.5f*g)*ball_animation_speed;
            u -= g*ball_animation_speed;
            ball_y_position += ball_del_y;
            
//            for z axis
            ball_del_z = -ball_velocity * cos(glm::radians(vertical_angle)) * sin(glm::radians(horizontal_angle)) * ball_animation_speed;
            ball_z_position += ball_del_z;
            
            if(ball_y_position<=0.04)
            {
                ball_animation=false;
                delay(2.0);
            }
        }
        
        
        
        drawBasketBall(lightingShaderWithTexture);
        
        checkScore();
        
        changeSpotLight(lightingShaderWithTexture);
        
        
        // Display informations
        
        
        cout<<"ball position "<<ball_x_position<<"  "<<ball_y_position<<"  "<<ball_z_position<<endl;
        cout<<"ball velocity "<<ball_velocity<<"m/s"<<endl;
        cout<<"horizontal angle "<<horizontal_angle<<"      "<< cos(glm::radians(horizontal_angle))<<endl;
        cout<<"verticle angle "<<vertical_angle<<"      "<<cos(glm::radians(vertical_angle))<<"  "<<sin(glm::radians(vertical_angle))<<endl;
        cout<<"del Y "<<ball_del_y<<endl;
        cout<<"Score:  "<<score<<endl;
        cout<<"ball animation "<<ball_animation<<endl;
        

        
        
        
        
        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
//        for (unsigned int i = 0; i < 4; i++)
//        {
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, pointLightPositions[i]);
//            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
//            cube.drawCube(ourShader, model, 0.8f, 0.8f, 0.8f);
//        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    
    // Human movement change control
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        human_x_position += human_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        human_x_position -= human_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        human_z_position -= human_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
        human_z_position += human_speed;
    }
    
    // horizontal angle change control
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        horizontal_angle += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        horizontal_angle -= 0.1;
    }
    
    // vertical angle change control
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        vertical_angle += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        vertical_angle -= 0.1;
    }
    
    // ball velocity change control
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        ball_velocity += 0.001;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        ball_velocity -= 0.001;
    }
    
    // fire ball
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        delay(0.5f);
        ball_animation = !ball_animation;
        
        ball_x_position = human_x_position;
        ball_y_position = 0.1f;
        ball_z_position = human_z_position;
        
        u = ball_velocity*sin(glm::radians(vertical_angle));
    }

    
    
    
    

    

//    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001f;
//    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001f;
//    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001f;
//    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001f;
//    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001f;
//    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001f;


    
    

    // Eye change control
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 50 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 50 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 50 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 50 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        eyeY += 50 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        eyeY -= 50 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    
    
    // look at point change control
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        lookAtX += 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        lookAtX -= 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        lookAtZ += 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        lookAtZ -= 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        lookAtY += 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        lookAtY -= 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    
    
    
    if(horizontal_angle >= 90)
    {
//        horizontal_angle = -360 - horizontal_angle;
        horizontal_angle = 90;
    }
    if(horizontal_angle <= -90)
    {
//        horizontal_angle = 360 + horizontal_angle;
        horizontal_angle = -90;
    }
    if(vertical_angle >= 70)
    {
        vertical_angle = 70;
    }
    if(vertical_angle < 0)
    {
        vertical_angle = 0;
    }
    if(ball_velocity >= 5)
    {
        ball_velocity = 5;
    }
    if(ball_velocity < 0)
    {
        ball_velocity = 0;
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (dirLightOn)
        {
            dirLight.turnOff();
            dirLightOn = !dirLightOn;
        }
        else
        {
            dirLight.turnOn();
            dirLightOn = !dirLightOn;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotLightOn)
        {
            spotLight.turnOff();
            spotLightOn = !spotLightOn;
        }
        else
        {
            spotLight.turnOn();
            spotLightOn = !spotLightOn;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
