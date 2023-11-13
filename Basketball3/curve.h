//
//  curve.h
//  Basketball3
//
//  Created by Ankan Saha on 14/11/23.
//

#ifndef curve_h
#define curve_h

#include "camera.h"
#include "basic_camera.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 0.5;
float scale_Y = 0.5;
float scale_Z = 0.5;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 90.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = -60.0, eyeY = 25.0, eyeZ = 50.0;//R
float lookAtX = 1.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);



glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

// viewport
GLint viewport[4];

/////////////////////////////////////////////////////////////
float wcsClkDn[3], wcsClkUp[3];
vector <float> cntrlPoints ={//ankon
{
-1.2350, 1.5050, 5.1000,
-1.3800, 1.4700, 5.1000,
-1.4900, 1.3800, 5.1000,
-1.5000, 1.2900, 5.1000,
-1.4450, 1.1850, 5.1000,
-1.3100, 1.1600, 5.1000,
-1.1550, 1.1800, 5.1000,
-1.0650, 1.2100, 5.1000,
-0.9700, 1.2600, 5.1000,
-0.9650, 1.3550, 5.1000,
-1.0300, 1.4650, 5.1000,
-1.1550, 1.5100, 5.1000,
-1.3100, 1.5050, 5.1000,
-1.4300, 1.4400, 5.1000,
-0.9400, 1.3100, 5.1000,
-0.9450, 1.2150, 5.1000,
-0.9500, 1.1550, 5.1000,
-0.8800, 1.0350, 5.1000,
-0.8200, 0.8850, 5.1000,
-0.7800, 0.7600, 5.1000,
-0.7700, 0.6700, 5.1000,
-0.7300, 0.5550, 5.1000,
-0.7300, 0.4700, 5.1000,
-0.7050, 0.3800, 5.1000,
-0.6850, 0.2800, 5.1000,
-0.7000, 0.2050, 5.1000,
-0.7250, 0.1450, 5.1000,
-0.7900, 0.1000, 5.1000,
}
};
vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;
class point
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];
int mouseButtonFlag = 0;
FILE* fp;
const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;
bool showControlPoints = true;
bool loadBezierCurvePoints = true;
bool showHollowBezier = false;
bool lineMode = false;
unsigned int bezierVAO;


long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}


//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L,float tx,float ty)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]+tx);//ankon
        vertices.push_back(coordinates[i + 1]+ty);//ankon
        vertices.push_back(coordinates[i + 2]);//ankon

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

void drawRing(Shader& lightingShader,float s,float tx,float ty,float tz)//ankon
{

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);


    // activate shader
    lightingShader.use();
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    lightingShader.setMat4("projection", projection);

    // camera/view transformation
        //view = camera.GetViewMatrix();
    glm::mat4 view = basic_camera.createViewMatrix();
    lightingShader.setMat4("view", view);



    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);

    for (int i = 0; i < (int)cntrlPoints.size(); i+=3)
    {
        cntrlPoints[i] /=   s;
        cntrlPoints[i + 1] /= s;
        cntrlPoints[i + 2] /= s;
    }

    for (int i = 0; i < (int)cntrlPoints.size(); i += 3)
    {
        //cntrlPoints[i] -= tx;
       //// cntrlPoints[i + 1] -= ty;
       // cntrlPoints[i + 2] -= tz;
    }


    if (loadBezierCurvePoints)
    {
        bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1,tx,ty);
        loadBezierCurvePoints = false;
        showHollowBezier = true;
    }
    if (showHollowBezier)
    {
        lightingShader.use();
        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.66f, 0.0f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.66f, 0.0f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.66f, 0.0f));
        lightingShader.setFloat("material.shininess",48.0f);

        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }
}

// This function converts the screen coordinates to world coordinates
// ---------------------------------------------------------------------------------------------
void scsToWcs(float sx, float sy, float wcsv[3])
{

    glm::vec4 winCoordinateVector; //vector to hold screen x,y,z coordinates
    glm::vec4 worldCoordinateVector; //vector to hold world x,y,z coordinates

    glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

    // setup screen coordinates values
    winCoordinateVector.x = sx;
    winCoordinateVector.y = (float)viewport[3] - (float)sy; // reversed since y-coordinates go from bottom to top
    winCoordinateVector.z = 0.0f;
    winCoordinateVector.w = 1.0f;

    // prepare viewport matrix
    float v_l = viewport[0], v_b = viewport[1], v_r = viewport[2], v_t = viewport[3];
    glm::mat4 viewportMat = glm::mat4(1.0f);
    viewportMat[0].x = (v_r - v_l) / 2.0f;  viewportMat[1].x = 0.0f; viewportMat[2].x = 0.0f;  viewportMat[3].x = (v_r + v_l) / 2.0f;
    viewportMat[0].y = 0.0f; viewportMat[1].y = (v_t - v_b) / 2.0f;  viewportMat[2].y = 0.0f;  viewportMat[3].y = (v_t + v_b) / 2.0f;
    viewportMat[0].z = 0.0f; viewportMat[1].z = 0.0f; viewportMat[2].z = 0.5f;  viewportMat[3].z = 0.5f;
    viewportMat[0].w = 0.0f; viewportMat[1].w = 0.0f; viewportMat[2].w = 0.0f;  viewportMat[3].w = 1.0f;

    //get the world coordinates from the screen coordinates
    worldCoordinateVector = glm::inverse(viewportMat * projection * view * model) * winCoordinateVector;
    wcsv[0] = worldCoordinateVector.x / worldCoordinateVector.w;
    wcsv[1] = worldCoordinateVector.y / worldCoordinateVector.w;
    wcsv[2] = worldCoordinateVector.z / worldCoordinateVector.w;
}

unsigned int drawControlPoints()
{
    unsigned int controlPointVAO;
    unsigned int controlPointVBO;

    glGenVertexArrays(1, &controlPointVAO);
    glGenBuffers(1, &controlPointVBO);

    glBindVertexArray(controlPointVAO);

    glBindBuffer(GL_ARRAY_BUFFER, controlPointVBO);
    glBufferData(GL_ARRAY_BUFFER, (unsigned int)cntrlPoints.size() * sizeof(float), cntrlPoints.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return controlPointVAO;
}

void saveControlPoints()
{
    if (cntrlPoints.size())
    {
        printf("save3d");
        fp = fopen("points you can copy.txt", "w");
        if (fp == NULL)
        {
            cout << "File Opening Failed" << endl;
        }
        else
        {
            fprintf(fp, "{\n");
            size_t count = cntrlPoints.size();
            for (int i = 0; i < count / 3; i++)
            {
                int j = i * 3;
                fprintf(fp, "%0.4f, %0.4f, %0.4f,\n", cntrlPoints[j], cntrlPoints[j + 1], cntrlPoints[j + 2]);
            }
            fprintf(fp, "}");
            fclose(fp);
        }
    }
}





    




#endif /* curve_h */
