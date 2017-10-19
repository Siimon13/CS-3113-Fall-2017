#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "ShaderProgram.h"
#include <vector>
#include <windows.h>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define FIXED_TIMESTEP 0.0166666f

SDL_Window* displayWindow;

// Loads Texture
GLuint LoadTexture(const char *filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}
	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
	return retTexture;
}

//Vector Class
class Vector3 {
public:
	Vector3(){}

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
	float x;
	float y;
	float z;
};

//Sheet Class
class SheetSprite {
public:
	SheetSprite() {};
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float
		size) :textureID(textureID), u(u), v(v), width(width), height(height), size(size) {}

	void Draw(ShaderProgram *program);
	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
};

void SheetSprite::Draw(ShaderProgram *program) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLfloat texCoords[] = {
		u, v + height,
		u + width, v,
		u, v,
		u + width, v,
		u, v + height,
		u + width, v + height
	};
	float aspect = width / height;
	float vertices[] = {
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, 0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, -0.5f * size ,
		0.5f * size * aspect, -0.5f * size };

	// draw our arrays
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6 );
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

}

//Draw Texts
void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		int spriteIndex = (int)text[i];
		float texture_x = (float)(spriteIndex % 16) / 16.0f;
		float texture_y = (float)(spriteIndex / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}

	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw this data (use the .data() method of std::vector to get pointer to data)
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6*text.size());
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}


//Entity Class
class Entity {
public:
	Entity(){}

	void Draw(ShaderProgram *program) {
		sprite.Draw(program);
	}

	Vector3 position;
	float velocity;
	Vector3 size;
	Matrix viewMatrix;
	float rotation;
	SheetSprite sprite;
	float health;
	float somethingElse;
};

//States
class GameState {
public:
	GameState(){}

	Entity player;
	std::vector<Entity> enemies;
	std::vector<Entity> bullets;
	Entity bullet;
	Entity enemy;
	int score;
};

enum GameMode{ STATE_MAIN_MENU, STATE_GAME};
GameMode mode = STATE_MAIN_MENU;
GameState state;

//Player Globals
Matrix playerviewMatrix;
//Matrix enemies[12];
SheetSprite bulletSprite;
SheetSprite enemySprite;

GLuint spriteSheetTexture;
GLuint textTexture;

//Render Function
void Render(ShaderProgram* program) {

	Matrix titleModelViewMatrix;
	Matrix commandModelViewMatrix;

	//Render Player
	program->SetModelviewMatrix(playerviewMatrix);
	state.player.Draw(program);
	
	//Render Enemy
	program->SetModelviewMatrix(state.enemy.viewMatrix);
	state.enemy.Draw(program);

	//Render Bullet
	program->SetModelviewMatrix(state.bullet.viewMatrix);
	state.bullet.Draw(program);

	for (size_t i = 0; i < state.bullets.size(); i++) {
		OutputDebugString("DrawBullet!!");
		program->SetModelviewMatrix(state.bullets[i].viewMatrix);
		state.bullets[i].Draw(program);
	}
	switch (mode) {

	case STATE_MAIN_MENU:
		titleModelViewMatrix.Translate(-1.25, 1, 0);
		program->SetModelviewMatrix(titleModelViewMatrix);
		DrawText(program, textTexture, "Space Invaders", .25f, -0.10f);

		commandModelViewMatrix.Translate(-2, -0.5, 0);
		program->SetModelviewMatrix(commandModelViewMatrix);
		DrawText(program, textTexture, "Press Space to Start Game", .25f, -0.10f);

		break;

	case STATE_GAME:
		break;
	}

}


//Static MainMenu
void UpdateMainMenu(float elapsed) {}

//Updatess game states
void UpdateGameLevel(float elapsed) {

	for (size_t i = 0; i < state.bullets.size(); i++) {
		OutputDebugString("UpdateBullet!!");
		state.bullets[i].position.y += 0.8f * elapsed;
		state.bullets[i].viewMatrix.SetPosition(state.bullet.position.x, state.bullet.position.y, 0);
	}

	state.bullet.position.y += 0.8f * elapsed;
	state.bullet.viewMatrix.SetPosition(state.bullet.position.x, state.bullet.position.y, 0);

	if (state.enemy.position.x + state.enemy.velocity * elapsed > 3.3 || 
		state.enemy.position.x + state.enemy.velocity * elapsed < -3.3) {
		state.enemy.velocity *= -1;
	}

	state.enemy.position.x += state.enemy.velocity * elapsed;
	state.enemy.viewMatrix.SetPosition(state.enemy.position.x, state.enemy.position.y, 0);

}

void Update(float elapsed) {
	switch (mode) {
	case STATE_MAIN_MENU:
		UpdateMainMenu(elapsed);
		break;
	case STATE_GAME:
		UpdateGameLevel(elapsed);
		break;
	}
}


void ProcessMainMenuInput(SDL_Event event) {
	if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
		mode = STATE_GAME;
		playerviewMatrix.SetPosition(0, -1.6f, 0);

		Entity enemy;
		enemy.sprite = enemySprite;
		enemy.velocity = 0.8f;
		enemy.position = Vector3(0, 0, 0);

		state.enemy = enemy;
	}
}

void ProcessGameLevelInput(SDL_Event event, float elapsed, bool summonBullet) {

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (summonBullet) {
		OutputDebugString("Summon Bullet???");

		Entity bullet;
		bullet.sprite = bulletSprite;
		bullet.position = Vector3(state.player.position.x, -1.5f, 0);
		//bullet.viewMatrix.Identity();

		//state.bullets.push_back(bullet);
		state.bullet = bullet;
	}

	if (keys[SDL_SCANCODE_RIGHT]) {
		OutputDebugString("Move Left???");

		state.player.position.x += 15.5f * elapsed;
		playerviewMatrix.Translate(15.5f * elapsed, 0, 0);
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		OutputDebugString("Move Right???");

		state.player.position.x -= 15.5f * elapsed;
		playerviewMatrix.Translate(-15.5f * elapsed, 0, 0);
	}


}

void ProcessInput(SDL_Event event, float elapsed, bool summonBullet) {
	switch (mode) {
	case STATE_MAIN_MENU:
		OutputDebugString("Getting out of main menu");
		ProcessMainMenuInput(event);
		break;

	case STATE_GAME:
		ProcessGameLevelInput(event, elapsed, summonBullet);
		break;
	}
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Simon's SpaceInvaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 960, 540);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	textTexture = LoadTexture(RESOURCE_FOLDER"text.png");

	Matrix projectionMatrix;
	projectionMatrix.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	//======================Creating Entities
	
	//initialize player
	spriteSheetTexture = LoadTexture(RESOURCE_FOLDER"sheet.png");
	SheetSprite shipSprite(spriteSheetTexture, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.5f);

	Entity player;
	player.sprite = shipSprite;
	player.position = Vector3(0, 0, 0);

	state.player = player;
	
	//Bullet
	bulletSprite = SheetSprite(spriteSheetTexture, 834.0f / 1024.0f, 299.0f / 1024.0f, 14.0f / 1024.0f, 31.0f / 1024.0f, 0.5f);
	bool summonBullet = false;

	//Enemy
	enemySprite = SheetSprite(spriteSheetTexture, 224.0f / 1024.0f, 832.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.25f);

	//Elapsed Times
	float accumulator = 0.0f;
	float prevTicks = 0.0f;

	SDL_Event event;
	bool done = false;
	while (!done) {
		
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - prevTicks;
		prevTicks = ticks;
		elapsed += accumulator;
		if (elapsed < FIXED_TIMESTEP) {
			accumulator = elapsed;
			continue;
		}
		summonBullet = false;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN) {

				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					summonBullet = true;
				}
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program.programID);
		program.SetProjectionMatrix(projectionMatrix);

		while (elapsed >= FIXED_TIMESTEP) {
			Update(FIXED_TIMESTEP);
			elapsed -= FIXED_TIMESTEP;
		}

		accumulator = elapsed;
		ProcessInput(event, elapsed, summonBullet);
		//Update(elapsed);
		Render(&program);


		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}

