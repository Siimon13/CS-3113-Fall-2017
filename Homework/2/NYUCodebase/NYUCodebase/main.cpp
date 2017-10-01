#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <math.h>
#include <iostream>
#include <windows.h>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 750, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	Matrix projectionMatrix;
	projectionMatrix.SetOrthoProjection(-5.55f, 5.55f, -4.0f, 4.0f, -1.0f, 1.0f);
	Matrix leftPaddleViewMatrix;
	Matrix rightPaddleViewMatrix;
	Matrix ballViewMatrix;

	float prevTicks = 0.0f;

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {
				//Controls

				//Left Paddle
				if (event.key.keysym.scancode == SDL_SCANCODE_W) {

					OutputDebugString("W pressed");
					leftPaddleViewMatrix.Translate(0.0f, 0.05f, 0.0f);
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					leftPaddleViewMatrix.Translate(0.0f, -0.05f, 0.0f);
				}

				//Right Paddle
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					rightPaddleViewMatrix.Translate(0.0f, 0.05f, 0.0f);
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					rightPaddleViewMatrix.Translate(0.0f, -0.05f, 0.0f);

				}
			}

		}
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program.programID);
		program.SetProjectionMatrix(projectionMatrix);

		//Draw left Paddle
		program.SetModelviewMatrix(leftPaddleViewMatrix);
		float leftPaddleVertices[] = { -5.0, 0.5, -5.0, -0.5, -4.8, -0.5, -4.8, -0.5, -4.8, 0.5, -5.0, 0.5 };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, leftPaddleVertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Draw right Paddle
		program.SetModelviewMatrix(rightPaddleViewMatrix);
		float rightPaddleVertices[] = { 5.0, 0.5, 5.0, -0.5, 4.8, -0.5, 4.8, -0.5, 4.8, 0.5, 5.0, 0.5 };
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, rightPaddleVertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Draw ball
		program.SetModelviewMatrix(ballViewMatrix);
		float ballVertices[] = { -.125f, .125f, -.125f, -.125f, .125, .125f, -.125f, -.125f, .125f, -.125f, .125f, .125f};
		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
		glEnableVertexAttribArray(program.positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(program.positionAttribute);

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - prevTicks;
		prevTicks = ticks;

		//Ball Movement 

		SDL_GL_SwapWindow(displayWindow);

		ballViewMatrix.Translate(0.1f*elapsed, -0.2f*elapsed, 0.0f);

	}

	SDL_Quit();
	return 0;
}
