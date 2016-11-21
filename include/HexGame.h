#pragma once
#include <vector>

#include "SDLDisplay.h"
class HexGame
{
	public:
		HexGame();
		~HexGame();

		bool Initialize();
		void Run();
		void Cleanup();

		void Quit();

	private:
		void HandleInput();
		void Update();
		void Draw();

		bool _running;
		SDLDisplay mainDisplay;
};

