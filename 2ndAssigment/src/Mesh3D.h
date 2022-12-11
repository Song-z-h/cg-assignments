#ifndef _MESH_H_
#define _MESH_H_
#include "Lib.h"
#include "Strutture.h"

extern vector<Material> materials;
class Mesh3D
{
private:
    vector<vec3> offsets;
    vector<vec3> scales;
    bool consumed = false;
    GLint drawType;
    vector<Mesh> bodyParts;

public:
    mat4 getBodyPartsModel(int i)
    {
        if (i < 0 || i >= bodyParts.size())
            return mat4(1.0f);
        return bodyParts[i].ModelM;
    }

    Mesh3D(GLint drawType)
    {
        this->drawType = drawType;
    }

    void draw(GLuint &MatModel, GLuint &lsceltaVS, GLuint &lsceltaFS, LightShaderUniform &light_unif, GLuint &loc_texture, GLuint &textureLeaves, GLuint &textureTree)
    {
        // bodySync();
        for (int i = 0; i < bodyParts.size(); i++)
        {
            glBindVertexArray(bodyParts[i].VAO);
            if (i == 0)
            {
                glBindTexture(GL_TEXTURE_2D, textureTree);
            }
            else
            {
                glUniform1i(loc_texture, 0);
                glBindTexture(GL_TEXTURE_2D, textureLeaves);
            }
            /*draw trees on the map*/
            glUniform1i(lsceltaVS, bodyParts[i].sceltaVS);
            glUniform1i(lsceltaFS, bodyParts[i].sceltaFS);
            // Passo allo shader il puntatore ai materiali
            glUniform3fv(light_unif.material_ambient, 1, glm::value_ptr(materials[bodyParts[i].material].ambient));
            glUniform3fv(light_unif.material_diffuse, 1, glm::value_ptr(materials[bodyParts[i].material].diffuse));
            glUniform3fv(light_unif.material_specular, 1, glm::value_ptr(materials[bodyParts[i].material].specular));
            glUniform1f(light_unif.material_shininess, materials[bodyParts[i].material].shininess);

            glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(bodyParts[i].ModelM));
            glDrawElements(drawType, (bodyParts[i].indici.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    vector<vec3> getAllVertices()
    {
        vector<vec3> vertices;
        for (int i = 0; i < bodyParts.size(); i++)
        {
            for (int j = 0; j < bodyParts[i].vertici.size(); j++)
            {
                vertices.push_back(bodyParts[i].vertici[j]);
            }
        }
        return vertices;
    }
    void addBodypart(Mesh &body, float offsetX = 0, float offsetY = 0, float offsetZ = 0.0,
                     float scaleX = 1, float scaleY = 1, float scaleZ = 1)
    {
        bodyParts.push_back(body);
        offsets.push_back(vec3(offsetX, offsetY, offsetZ));
        scales.push_back(vec3(scaleX, scaleY, scaleZ));
    }
    void resetModelMat()
    {
        for (int i = 0; i < bodyParts.size(); i++)
        {
            bodyParts[i].ModelM = mat4(1.0f);
        }
    }

    void translateMainBody(float x = 0, float y = 0, float z = 0)
    {
        resetModelMat();
        for (int i = 0; i < bodyParts.size(); i++)
        {
            bodyParts[i].ModelM = translate(bodyParts[i].ModelM, vec3(x, y, z));
        }
    }

    void translateBodyPart()
    {
        for (int i = 1; i < bodyParts.size(); i++)
        {
            bodyParts[i].ModelM = translate(bodyParts[i].ModelM, offsets[i]);
        }
    }

    void rotateAll(float theta, float x, float y, float z)
    {
        for (int i = 0; i < bodyParts.size(); i++)
        {
            bodyParts[i].ModelM = rotate(bodyParts[i].ModelM, radians(theta), vec3(x, y, z));
        }
    }
    void scaleAll(float x, float y, float z = 1.0f)
    {
        for (int i = 0; i < bodyParts.size(); i++)
        {
            scaleOne(i, scales[i].x, scales[i].y, scales[i].z);
            bodyParts[i].ModelM = scale(bodyParts[i].ModelM, vec3(x, y, z));
            if (!consumed)
            {
                offsets[i].x *= x;
                offsets[i].y *= y;
                offsets[i].z *= z;
            }
        }
        consumed = true;
    }

    void scaleOne(int i, float x, float y, float z = 1.0f)
    {
        if (i < 0 || i >= bodyParts.size())
            return;
        bodyParts[i].ModelM = scale(bodyParts[i].ModelM, vec3(x, y, z));
        if (!consumed)
        {
            offsets[i].x *= x;
            offsets[i].y *= y;
            offsets[i].z *= z;
        }
    }

    void animation(int i, float x, float y, float z = 1.0f)
    {
        if (i < 0 || i >= bodyParts.size())
            return;
        bodyParts[i].ModelM = scale(bodyParts[i].ModelM, vec3(x, y, z));
    }
};

#endif