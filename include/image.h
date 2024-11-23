#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "mainHeader.h"

// Placeholder class to make images easier to use

class image {
private:
	SDL_Surface* imageSurface;
	SDL_Texture* imageTexture;
	SDL_Rect imageRect;
public:
	
	bool visible;
	const char* filePath;
	const char* prevFilePath;

	image(const char* filePath, int width, int height, int locX, int locY):
		imageSurface(NULL),
		imageRect{locX, locY, width, height},
		visible(true),
		filePath(filePath),
		prevFilePath(filePath)
	{}

	void initialize(SDL_Renderer* renderer);

	void render(SDL_Renderer* renderer);

	void update(std::string filePath);

	bool isVisible() const;
};

class imageManager {
public:
	std::vector<image> images;

	/*
	Adds an image.
	@param image im
	*/
	void add(image img);

	void initializeAll(SDL_Renderer* renderer);

	void renderAll(SDL_Renderer* renderer);
};


#endif /* IMAGE_H */
