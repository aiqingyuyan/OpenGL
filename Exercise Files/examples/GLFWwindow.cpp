/*
 * =====================================================================================
 *
 *       Filename:  xGLFWwindow.cpp
 *
 *    Description:  making a window with GLFW
 *
 *        Version:  1.0
 *        Created:  06/11/2014 20:37:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pablo Colapinto (), gmail -> wolftype
 *   Organization:  
 *
 * =====================================================================================
 */

//Include OpenGL for graphics and Glfw for windowing
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include "GLFW/glfw3.h"     // graphics library extension wrangler
#include <iostream>


int main(){

    if( !glfwInit() ) exit(EXIT_FAILURE);

    int w = 1024; int h = 768;

    GLFWwindow * window;             //Create Window (use glfwGetPrimaryMonitor() for fullscreen)
    window = glfwCreateWindow(w,h,"glfw",NULL,NULL);

    if (!window) {                   //Check Validity
      glfwTerminate();
      exit(EXIT_FAILURE);
    }        
    glfwMakeContextCurrent(window);  //Make Window Current Context


    printf("hello window\n");
  
    /*-----------------------------------------------------------------------------
     *  MAIN LOOP - Draw Loop (Render once per frame)
     *-----------------------------------------------------------------------------*/
    while ( !glfwWindowShouldClose(window) ){ // glfwWindowShouldClose takes a window and returns whether we want to close it
        glViewport(0,0,w,h);                  //Set Viewport in pixels (0,0) top left hand corner
        glClearColor(0.3f,0.6f,0.4f,1.0f);                    //CLEAR WINDOW CONTENTS (RGBA)
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Execute the cleaing of the clear color we set before
                                                              // The depth bit is also enabled to clear the depth buffer

        //put drawing code in here

        glfwSwapBuffers(window);                //<-- SWAP BUFFERS
        glfwPollEvents();                       //<-- LISTEN FOR WINDOW EVENTS
    }
  
    // Once we exit the winodw
    //Destroy window and terminate glfw session
    glfwDestroyWindow(window);
    glfwTerminate();
    printf("goodbye window\n");

  return 0;
}
