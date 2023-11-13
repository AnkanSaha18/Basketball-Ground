#ifndef triangle_h
#define triangle_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Triangle {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Triangle()
    {
        setUpTriangleVertexDataAndConfigureVertexAttribute();
    }

    Triangle(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpTriangleVertexDataAndConfigureVertexAttribute();
    }

    Triangle(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpTriangleVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Triangle()
    {
        glDeleteVertexArrays(1, &TriangleVAO);
        glDeleteVertexArrays(1, &lightTriangleVAO);
        glDeleteVertexArrays(1, &lightTexTriangleVAO);
        glDeleteBuffers(1, &TriangleVBO);
        glDeleteBuffers(1, &TriangleEBO);
    }

    void drawTriangleWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        //a fragment shader can use multiple texture at a time, 0 and 1 is used to indicate fragment shader needs to work with both of the texture together
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);  //0 means diffuse texture
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap); //0 is bound to diffuseMap
        // bind specular map    
        glActiveTexture(GL_TEXTURE1); //1 means specular texture 
        glBindTexture(GL_TEXTURE_2D, this->specularMap); //1 is bound to specularMap

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexTriangleVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawTriangleWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightTriangleVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawTriangle(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(TriangleVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int TriangleVAO;
    unsigned int lightTriangleVAO;
    unsigned int lightTexTriangleVAO;
    unsigned int TriangleVBO;
    unsigned int TriangleEBO;

    void setUpTriangleVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float Triangle_vertices[] = {
            // positions      // normals         // texture

            // XY plane points (back Triangle)
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,

            // Front Triangle
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,

            // Left Base
            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
            0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

            // Floor Base
            0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmax,

            // Inclined Plane
            0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, TXmax, TYmax,
            1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, TXmin, TYmax,
            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, TXmin, TYmin
        };
        unsigned int Triangle_indices[] = {
            0, 1, 2, 

            3, 4, 5,

            6, 7, 9,
            9, 7, 8,

            10, 11, 12,
            10, 12, 13,

            14, 15, 16,
            14, 16, 17
        };

        glGenVertexArrays(1, &TriangleVAO);
        glGenVertexArrays(1, &lightTriangleVAO);
        glGenVertexArrays(1, &lightTexTriangleVAO); //for texture
        glGenBuffers(1, &TriangleVBO);
        glGenBuffers(1, &TriangleEBO);


        glBindVertexArray(lightTexTriangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle_vertices), Triangle_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriangleEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Triangle_indices), Triangle_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightTriangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriangleEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(TriangleVAO);

        glBindBuffer(GL_ARRAY_BUFFER, TriangleVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TriangleEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};


#endif /* Triangle_h */
