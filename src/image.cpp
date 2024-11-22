#include "../include/image.h"

void image::initialize(SDL_Renderer* renderer) {
	imageSurface = IMG_Load(filePath);
	if (imageSurface == nullptr) {
		std::cout << "A problem occurred when trying to create image. Error: " << SDL_GetError() << '\n';
		return;
	}
	imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);	
}

void image::render(SDL_Renderer* renderer) {
	if (!isVisible()) return;

	if (filePath != prevFilePath) {
		SDL_DestroyTexture(imageTexture);
		imageSurface = IMG_Load(filePath);
		imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
		SDL_FreeSurface(imageSurface);
		prevFilePath = filePath;
	}

	SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
}

void image::update(std::string str) {
	filePath = str.c_str();
}

bool image::isVisible() const { return visible; }

void imageManager::add(image img) {
	images.push_back(img);
}

void imageManager::initializeAll(SDL_Renderer* renderer) {
	for (auto& img : images) {
		img.initialize(renderer);
	}
}

void imageManager::renderAll(SDL_Renderer* renderer) {
	for (auto& img : images) {
		img.render(renderer);
	}
}