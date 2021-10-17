#include "Game2D.h"

#include "Application.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Player.h"
#include "Settings.h"




Game2D::Game2D(const char* title, int width, int height, bool fullscreen) : Game(title, width, height, fullscreen)
{
	// Initalise the 2D renderer.
	m_2dRenderer = new aie::Renderer2D();
	//m_2dRenderer->SetCameraPos(-(MAP_SIZE_X * (NODE_GAP + 1)) / 2, -(MAP_SIZE_Y * (NODE_GAP + 1)) / 2);
	// Create some textures for testing.

	// Create a player object.
	m_Pathfinder = new Pathfinder(m_2dRenderer);
	m_agent = new Agent(m_Pathfinder);
}

Game2D::~Game2D()
{
	// Delete player.

	// Delete the renderer.
	delete m_2dRenderer;
}

void Game2D::Update(float deltaTime)
{
	// Update the player.
	//m_Player->Update(deltaTime);
	

	//m_Pathfinder->Pathfind(Vector2(5, 3), Vector2(7, 9));
	m_agent->Update(deltaTime);


	//// Input example: Update the camera position using the arrow keys.
	aie::Input* input = aie::Input::GetInstance();
	float camPosX;
	float camPosY;

	m_2dRenderer->GetCameraPos(camPosX, camPosY);

	if (input->IsKeyDown(aie::INPUT_KEY_W))
		camPosY += 500.0f * deltaTime;

	if (input->IsKeyDown(aie::INPUT_KEY_S))
		camPosY -= 500.0f * deltaTime;

	if (input->IsKeyDown(aie::INPUT_KEY_A))
		camPosX -= 500.0f * deltaTime;

	if (input->IsKeyDown(aie::INPUT_KEY_D))
		camPosX += 500.0f * deltaTime;

	m_2dRenderer->SetCameraPos(camPosX, camPosY);

	// Exit the application if escape is pressed.
	if (input->IsKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		aie::Application* application = aie::Application::GetInstance();
		application->Quit();
	}
}

void Game2D::Draw()
{
	aie::Application* application = aie::Application::GetInstance();
	float time = application->GetTime();

	// Wipe the screen to clear away the previous frame.
	application->ClearScreen();

	// Prepare the renderer. This must be called before any sprites are drawn.
	m_2dRenderer->Begin();

	//Draw background colour
	Vector2 cameraPos;
	m_2dRenderer->GetCameraPos(cameraPos.x, cameraPos.y);
	Vector2 screenSize = Vector2(aie::Application::GetInstance()->GetWindowWidth(), aie::Application::GetInstance()->GetWindowHeight());
	m_2dRenderer->SetRenderColour C_BACKGROUND;
	m_2dRenderer->DrawBox(cameraPos.x + (screenSize.x/2), cameraPos.y + (screenSize.y/2), screenSize.x, screenSize.y, 0, 100);
	
	// Draw the player.
	m_Pathfinder->Render();
	m_agent->Render(m_2dRenderer);

	//// Draw a thin line.
	//m_2dRenderer->DrawLine(150.0f, 400.0f, 250.0f, 500.0f, 2.0f);

	//// Draw a sprite
	//m_2dRenderer->DrawSprite(m_texture2, 200.0f, 200.0f);

	//// Draw a moving purple circle.
	//m_2dRenderer->SetRenderColour(1.0f, 0.0f, 1.0f, 1.0f);
	//m_2dRenderer->DrawCircle(sin(time) * 100.0f + 450.0f, 200.0f, 50.0f);

	//// Draw a rotating sprite with no texture, coloured yellow.
	//m_2dRenderer->SetRenderColour(1.0f, 1.0f, 0.0f, 1.0f);
	//m_2dRenderer->DrawSprite(nullptr, 700.0f, 200.0f, 50.0f, 50.0f, time);
	//m_2dRenderer->SetRenderColour(1.0f, 1.0f, 1.0f, 1.0f);

	//// Demonstrate animation.
	//float animSpeed = 10.0f;
	//int frame = ((int)(time * animSpeed) % 6);
	//float size = 1.0f / 6.0f;
	//m_2dRenderer->SetUVRect(frame * size, 0.0f, size, 1.0f);
	//m_2dRenderer->DrawSprite(m_texture, 900.0f, 200.0f, 100.0f, 100.0f);
	//m_2dRenderer->SetUVRect(0.0f, 0.0f, 1.0f, 1.0f);
	//
	//// Draw some text.
	//float windowHeight = (float)application->GetWindowHeight();
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", application->GetFPS());
	//m_2dRenderer->DrawText2D(m_font, fps, 15.0f, windowHeight - 32.0f);
	//m_2dRenderer->DrawText2D(m_font, "Arrow keys to move.", 15.0f, windowHeight - 64.0f);
	//m_2dRenderer->DrawText2D(m_font, "WASD to move camera.", 15.0f, windowHeight - 96.0f);
	//m_2dRenderer->DrawText2D(m_font, "Press ESC to quit!", 15.0f, windowHeight - 128.0f);

	// Done drawing sprites. Must be called at the end of the Draw().
	m_2dRenderer->End();
}