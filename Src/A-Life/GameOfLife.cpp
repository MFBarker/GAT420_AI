#include "GameOfLife.h"
#include "Texture.h"

#include "Random.h"

bool GameOfLife::Initialize()
{
	bufferA.resize(size.x * size.y);
	bufferB.resize(size.x * size.y);

	return true;
}

void GameOfLife::Step()
{
	frame++;
	std::vector<uint8_t>& readBuffer = (frame/2) ? bufferA : bufferB; 
	std::vector<uint8_t>& writeBuffer = (frame/2) ? bufferB : bufferA;

    static uint8_t rule = 0;
    rule = 73;

    // update buffer
    // clear write buffer
    std::fill(writeBuffer.begin(), writeBuffer.end(), 0);

    for (int y = 0; y < size.y; y++)
    {
        for (int x = 0; x < size.x; x++)
        {
            int weight = 0;

            // read surrounding 8 cells, accumulate weight
            // XXX
            // X0X
            // XXX
            weight += Read<uint8_t>(readBuffer, size.x, size.y);

            // game of life rules
           // if cell is alive, update
            //whole section may be incorrect
            if (Read<uint8_t>(readBuffer, x, y))
            {
                if (1) Write<uint8_t>(writeBuffer, x, y,weight);
                //... (not sure what was wanted here)
            }
            else
            {
                // if cell is dead, update
                if (0) Write<uint8_t>(writeBuffer, x, y,weight);
            }
        }
    }

    // convert buffer data format to color buffer
    std::transform((writeBuffer).begin(), (writeBuffer).end(), color_buffer.begin(), [](uint8_t v)
        {
            { return  v ? 0xffffffff : 0; }
        });

    // copy color buffer to texture
    texture->Copy(color_buffer);
}

void GameOfLife::KeyDown(SDL_Keycode keycode)
{
    if (keycode == SDLK_r)
    {
        // write random 'alive' cells to buffer
        std::vector<uint8_t>& writeBuffer = (frame % 2) ? bufferB : bufferA;
        for (int i = 0; i < writeBuffer.size(); i++) writeBuffer[i] = (random(0, 10) == 0) ? 1 : 0;
    }
}