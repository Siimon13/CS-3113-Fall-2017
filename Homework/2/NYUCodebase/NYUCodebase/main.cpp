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
	float xDelta = -0.5f;
	float yDelta = 0.5f;
	
	//Game Loc trackers

	//Ball trackers
	float ballxpos = 0.0f;
	float ballypos = 0.0f;
	float ballacc = 1.0f;

	//PaddleTrackers
	float rightPaddleTop = 0.5f;
	float rightPaddleBottom = -0.5f;

	float leftPaddleTop = 0.5f;
	float leftPaddleBottom = -0.5f;

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
					leftPaddleViewMatrix.Translate(0.0f, 0.05f, 0.0f);
					leftPaddleTop += 0.05f;
					leftPaddleBottom += 0.05f;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					leftPaddleViewMatrix.Translate(0.0f, -0.05f, 0.0f);
					leftPaddleTop -= 0.05f;
					leftPaddleBottom -= 0.05f;
				}

				//Right Paddle
				if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
					rightPaddleViewMatrix.Translate(0.0f, 0.05f, 0.0f);
					rightPaddleTop += 0.05f;
					rightPaddleBottom += 0.05f;
				}
				else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					rightPaddleViewMatrix.Translate(0.0f, -0.05f, 0.0f);
					rightPaddleTop -= 0.05f;
					rightPaddleBottom -= 0.05f;

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

		ballypos += yDelta*elapsed*ballacc;
		ballxpos += xDelta*elapsed*ballacc;

		if (ballypos + 0.25f >= 4.0f || ballypos - 0.25f <= -4.0f)
			yDelta *= -1;

		// Collision with Paddles
		if ((ballxpos + 0.25f >= 4.95f || ballxpos - 0.25f <= -4.95f )&& (rightPaddleBottom <= ballypos <= rightPaddleTop || leftPaddleBottom <= ballypos <= leftPaddleTop))
			xDelta *= -1;


		if (ballxpos + 0.25f >= 5.2f) {
			OutputDebugString("Left Paddle Wins!!");
			ballViewMatrix.Translate(-1*ballxpos, -1*ballypos, 0);
			ballxpos = 0;
			ballypos = 0;
			xDelta *= -1;
			ballacc = 1.0f;
		}
		else if (ballxpos - 0.25f <= -5.2f){
			OutputDebugString("Right Paddle Wins!!");
			ballViewMatrix.Translate(-1 * ballxpos, -1 * ballypos, 0);
			ballxpos = 0;
			ballypos = 0;
			xDelta *= -1;
			ballacc = 1.0f;
		}	
		else {
			ballViewMatrix.Translate(xDelta*elapsed*ballacc, yDelta*elapsed*ballacc, 0.0f);
		}
		ballacc *= 1.0001f;
	}

	SDL_Quit();
	return 0;
}
