//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef DirLight_h
#define DirLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class DirLight { //point light class
public:
    //the attributes of light
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;


    //constructor
    DirLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB) {

        direction = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);

    }
    //input is a shader, ekhan theke set up hochhe fragment shader e
    void setUpDirLight(Shader& lightingShaderWithTexture)
    {

        lightingShaderWithTexture.setVec3("dirLight.direction", direction);
        lightingShaderWithTexture.setVec3("dirLight.ambient", ambientOn * ambient);  //turn off hoile ambientOn 0, so ultimately ambient = 0 . complemete shutdown, we can add some number to make sure there is little light/color, even if everything is turned off, its called ambient light
        lightingShaderWithTexture.setVec3("dirLight.diffuse", diffuseOn * diffuse);
        lightingShaderWithTexture.setVec3("dirLight.specular", specularOn * specular);

    }
    void turnOff() //light off
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private: //for light on-off
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* DirLight_h */
