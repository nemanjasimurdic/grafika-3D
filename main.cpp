// Autor: Nemanja Simurdic, RA 190/2021
// Opis: Specifikacija 22: Arktik

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);

int main(void)
{
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 800;
    unsigned int wHeight = 600;
    const char wTitle[] = "Arktik";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    unsigned int textureShader = createShader("texture.vert", "texture.frag");

    // okean
    float oceanVertices[] =
    {
        -50.0f, -5.0f,  -50.0f,    0.0f, 0.5f, 1.0f, 1.0f,  
         50.0f, -5.0f,  -50.0f,    0.0f, 0.5f, 1.0f, 1.0f,  
         50.0f, -5.0f,   50.0f,    0.0f, 0.5f, 1.0f, 1.0f,  
        -50.0f, -5.0f,   50.0f,    0.0f, 0.5f, 1.0f, 1.0f   
    };
    unsigned int oceanIndices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int stride = (3 + 4) * sizeof(float); 
    
    unsigned int oceanVAO, oceanVBO, oceanEBO;
    glGenVertexArrays(1, &oceanVAO);
    glGenBuffers(1, &oceanVBO);
    glGenBuffers(1, &oceanEBO);

    glBindVertexArray(oceanVAO);
    glBindBuffer(GL_ARRAY_BUFFER, oceanVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(oceanVertices), oceanVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oceanEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(oceanIndices), oceanIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // sante
    float icebergVertices[] = {
        // donja baza - kocka
       -2, -1, -2,   0.8f, 0.9f, 1.0f, 1.0f,  
        2, -1, -2,   0.8f, 0.9f, 1.0f, 1.0f,  
        2, -1,  2,   0.8f, 0.9f, 1.0f, 1.0f,  
       -2, -1,  2,   0.8f, 0.9f, 1.0f, 1.0f,  
        // gornja baza
       -2,  1, -2,   0.8f, 0.9f, 1.0f, 1.0f,  
        2,  1, -2,   0.8f, 0.9f, 1.0f, 1.0f,  
        2,  1,  2,   0.8f, 0.9f, 1.0f, 1.0f,  
       -2,  1,  2,   0.8f, 0.9f, 1.0f, 1.0f,  
        // vrh piramide
        0,  3,  0,   0.95f, 0.97f, 1.0f, 1.0f  
    };

    unsigned int icebergIndices[] = {
        // Kocka
        0,1,2, 2,3,0,       
        4,5,6, 6,7,4,       
        0,1,5, 5,4,0,       
        1,2,6, 6,5,1,
        2,3,7, 7,6,2,
        3,0,4, 4,7,3,
        // Piramida od 4 trougla
        8,4,5,
        8,5,6,
        8,6,7,
        8,7,4
    };

    unsigned int icebergVAO, icebergVBO, icebergEBO;
    glGenVertexArrays(1, &icebergVAO);
    glGenBuffers(1, &icebergVBO);
    glGenBuffers(1, &icebergEBO);

    glBindVertexArray(icebergVAO);
    glBindBuffer(GL_ARRAY_BUFFER, icebergVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(icebergVertices), icebergVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icebergEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(icebergIndices), icebergIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // potpis
    float signatureVertices[] = {
    1.0f,    0.15f,    1.0f, 1.0f,  
    1.0f,    0.0f,     1.0f, 0.0f,  
    0.7f,    0.15f,    0.0f, 1.0f,  
    0.7f,    0.0f,     0.0f, 0.0f   
    };

    unsigned int signatureVAO, signatureVBO;
    glGenVertexArrays(1, &signatureVAO);
    glGenBuffers(1, &signatureVBO);

    glBindVertexArray(signatureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, signatureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(signatureVertices), signatureVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // tekstura
    unsigned int signatureTexture;
    glGenTextures(1, &signatureTexture);
    glBindTexture(GL_TEXTURE_2D, signatureTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int sigWidth, sigHeight, sigChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* sigData = stbi_load("resources/signature.png", &sigWidth, &sigHeight, &sigChannels, STBI_rgb_alpha);
    if (sigData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sigWidth, sigHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, sigData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Greska pri ucitavanju potpisa!" << std::endl;
    }
    stbi_image_free(sigData);


    glm::mat4 model = glm::mat4(1.0f);
    unsigned int modelLoc = glGetUniformLocation(unifiedShader, "uM");
    unsigned int viewLoc = glGetUniformLocation(unifiedShader, "uV");
    unsigned int projectionLoc = glGetUniformLocation(unifiedShader, "uP");

    glm::mat4 icebergModels[4];
    // povecavam sante
    glm::vec3 globalScale(1.2f, 1.2f, 1.2f);

    // santa 1
    icebergModels[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, -5.0f, -10.0f));
    icebergModels[0] = glm::rotate(icebergModels[0], glm::radians(20.0f), glm::vec3(0, 1, 0));
    icebergModels[0] = glm::scale(icebergModels[0], glm::vec3(1.0f, 0.8f, 1.2f));
    icebergModels[0] = glm::scale(icebergModels[0], globalScale);

    // santa 2
    icebergModels[1] = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -5.0f, -15.0f));
    icebergModels[1] = glm::rotate(icebergModels[1], glm::radians(75.0f), glm::vec3(0, 1, 0));
    icebergModels[1] = glm::scale(icebergModels[1], glm::vec3(1.8f, 1.2f, 1.0f));
    icebergModels[1] = glm::scale(icebergModels[1], globalScale);

    // santa 3
    icebergModels[2] = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -5.0f, 10.0f));
    icebergModels[2] = glm::rotate(icebergModels[2], glm::radians(-30.0f), glm::vec3(0, 1, 0));
    icebergModels[2] = glm::scale(icebergModels[2], glm::vec3(0.9f, 0.7f, 1.1f));
    icebergModels[2] = glm::scale(icebergModels[2], globalScale);

    // santa 4
    icebergModels[3] = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, -5.0f, 5.0f));
    icebergModels[3] = glm::rotate(icebergModels[3], glm::radians(120.0f), glm::vec3(0, 1, 0));
    icebergModels[3] = glm::scale(icebergModels[3], glm::vec3(2.0f, 0.9f, 1.5f));
    icebergModels[3] = glm::scale(icebergModels[3], globalScale);

    
    float aspect = (float)wWidth / (float)wHeight;
    glm::mat4 projectionO = glm::ortho(-30.0f, 30.0f, -20.0f, 20.0f, 1.0f, 100.0f);
    glm::mat4 projectionP = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 100.0f);

    glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 30.0f);
    glm::vec3 cameraFront = glm::normalize(glm::vec3(0.0f, -0.3f, -1.0f));
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraSpeed = 10.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool showIcebergs = true;
    bool spacePressedLastFrame = false; // da se desi jedna akcija po pritisku space-a


    glUseProgram(unifiedShader);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(unifiedShader);

        glm::mat4 view = glm::lookAt(
            cameraPos,                    
            cameraPos + cameraFront,      
            cameraUp);                    
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float adjustedSpeed = cameraSpeed * deltaTime;

        float yawSpeed = glm::radians(60.0f);

        // renderovanje okeana
        glBindVertexArray(oceanVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // renderovanje santi
        if (showIcebergs)
        {
            glBindVertexArray(icebergVAO);
            for (int i = 0; i < 4; ++i)
            {
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(icebergModels[i]));
                glDrawElements(GL_TRIANGLES, 36 + 12, GL_UNSIGNED_INT, 0); // 6 strana kocke + 4 trougla
            }
            glBindVertexArray(0);
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            glEnable(GL_DEPTH_TEST);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            glEnable(GL_CULL_FACE);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            glDisable(GL_CULL_FACE);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            cameraPos += adjustedSpeed * cameraFront;

        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            cameraPos -= adjustedSpeed * cameraFront;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * adjustedSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * adjustedSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionP));
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionO));
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            float angle = -yawSpeed * deltaTime; 
            cameraFront = glm::rotate(glm::mat4(1.0f), angle, cameraUp) * glm::vec4(cameraFront, 1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            float angle = yawSpeed * deltaTime; 
            cameraFront = glm::rotate(glm::mat4(1.0f), angle, cameraUp) * glm::vec4(cameraFront, 1.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if (!spacePressedLastFrame)
            {
                showIcebergs = !showIcebergs;
                spacePressedLastFrame = true; 
            }
        }
        else
        {
            spacePressedLastFrame = false;
        }

        // ogranicenje na 60 frejmova
        const double TARGET_FRAME_TIME = 1.0 / 60.0;
        double frameEndTime = glfwGetTime();
        double frameDuration = frameEndTime - currentFrame;

        if (frameDuration < TARGET_FRAME_TIME) {
            std::this_thread::sleep_for(
                std::chrono::duration<double>(TARGET_FRAME_TIME - frameDuration)
            );
        }

        glUseProgram(textureShader);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 orthoProj = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f);
        glm::mat4 identity = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(textureShader, "uM"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(textureShader, "uV"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(textureShader, "uP"), 1, GL_FALSE, glm::value_ptr(orthoProj));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, signatureTexture);
        glUniform1i(glGetUniformLocation(textureShader, "uTex"), 0);

        glBindVertexArray(signatureVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &oceanVAO);
    glDeleteBuffers(1, &oceanVBO);
    glDeleteBuffers(1, &oceanEBO);

    glDeleteVertexArrays(1, &icebergVAO);
    glDeleteBuffers(1, &icebergVBO);
    glDeleteBuffers(1, &icebergEBO);

    glDeleteTextures(1, &signatureTexture);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}

unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
