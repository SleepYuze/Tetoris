#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
struct AnimationFrame
{
	sf::Texture texture;
	int delay_ms;
};
int main()
{
	setlocale(LC_ALL, "ru");
	std::string filename = "assets\\tetoris.gif";
	FILE* file = fopen(filename.c_str(), "rb");
	if (!file)
	{
		std::cerr << "Не удалось открыть файл!" << std::endl;
		return -1;
	}
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	std::vector<unsigned char> fileBuffer(fileSize);
	fread(fileBuffer.data(), 1, fileSize, file);
	fclose(file);
	int width, height, frameCount, compPerPixel;
	int* delaysMs = nullptr;
	unsigned char* gifData = stbi_load_gif_from_memory(
		fileBuffer.data(), fileSize,
		&delaysMs,
		&width, &height,
		&frameCount,
		&compPerPixel,
		0
	);
	if (!gifData)
	{
		std::cerr << "Ошибка загрузки GIF: " << stbi_failure_reason() << std::endl;
		return -1;
	}
	std::cout << "Загружено " << frameCount << " кадров. Размер: " << width << "x" << height << std::endl;
	if (width <= 0 || height <= 0 || frameCount <= 0)
	{
		std::cerr << "Некорректные размеры GIF!" << std::endl;
		stbi_image_free(gifData);
		if (delaysMs) free(delaysMs);
		return -1;
	}
	std::vector<AnimationFrame> frames;
	int frameSizeInBytes = width * height * 4;
	for (int i = 0; i < frameCount; ++i)
	{
		unsigned char* framePixelData = gifData + i * frameSizeInBytes;
		assert(framePixelData >= gifData);
		assert(framePixelData < gifData + frameCount * frameSizeInBytes);
		sf::Texture texture;
		if (!texture.resize({ static_cast<unsigned int>(width), static_cast<unsigned int>(height) }))
		{
			std::cerr << "Не удалось задать размер текстуры для кадра " << i << std::endl;
			continue;
		}
		texture.update(framePixelData, sf::Vector2u(width, height), sf::Vector2u(0, 0));
		frames.push_back({ std::move(texture), delaysMs[i] });
	}
	stbi_image_free(gifData);
	if (delaysMs)
		free(delaysMs);
	if (frames.empty()) {
		std::cerr << "Не удалось создать ни одного кадра!" << std::endl;
		return -1;
	}
	std::cout << "Успешно загружено " << frames.size() << " кадров" << std::endl;
	sf::Music music("assets/Tetoris.ogg");
	music.setLooping(true);
	music.setVolume(70);
	music.play();
	sf::RenderWindow window(
		sf::VideoMode({ static_cast<unsigned int>(width), static_cast<unsigned int>(height) }),
		"Tetoris"
	);
	sf::Sprite sprite(frames[0].texture);
	size_t currentFrame = 0;
	sf::Clock frameTimer;
	float accumulator = 0.0f;

	while (window.isOpen())
	{
		while (auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}
		float deltaTime = frameTimer.restart().asSeconds();
		float frameDuration = frames[currentFrame].delay_ms / 1000.0f;
		accumulator += deltaTime;
		while (accumulator >= frameDuration) {
			accumulator -= frameDuration;
			currentFrame = (currentFrame + 1) % frames.size();
			frameDuration = frames[currentFrame].delay_ms / 1000.0f;
			sprite.setTexture(frames[currentFrame].texture);
		}
		window.clear(sf::Color::Black);
		window.draw(sprite);
		window.display();
	}
	return 0;
}
