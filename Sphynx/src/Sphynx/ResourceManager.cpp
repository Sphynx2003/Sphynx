#include "pch.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_truetype.h"



bool Init = RuntimeInit<int>(stbi_set_flip_vertically_on_load, 1);

Sphynx::Core::Graphics::Shader* Sphynx::ResourceManager::LoadShader(const char* path, Sphynx::Core::Graphics::ShaderType type)
{
	//Read File.
	std::fstream in;
	in.open(path, in.in);
	std::string glsl((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	in.close();
	auto code = glsl.c_str();
	auto shader = Sphynx::Core::Graphics::Shader::Create(code, type);
	in.close();
	glsl.clear();
	return shader;
}

Sphynx::Core::Graphics::Texture* Sphynx::ResourceManager::LoadTexture(const char* path, Core::Graphics::TextureType type, bool Compress)
{
	//Check Loaded textures.
	if (OpenTex[path]) {
		return OpenTex[path];
	}
	void* data;
	int x, y, bits;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path, &x, &y, &bits, 0);

	Sphynx::Core::Graphics::TextureFormat format;
	switch (bits)
	{
	case 1:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRed;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::Red;
		}
		break;
	case 2:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRG;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RG;
		}
		break;
	case 3:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRGB;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RGB;
		}
		break;
	case 4:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRGBA;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RGBA;
		}
		break;
	default:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRGBA;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RGBA;
		}
		break;
	}
	auto Tex = Sphynx::Core::Graphics::Texture::Create(data, x, y, 0, type, format, Sphynx::Core::Graphics::TextureDataFormat::UByte);
	//Add to the map.
	OpenTex[path] = Tex;
	return Tex;
}

Sphynx::Core::Font* Sphynx::ResourceManager::LoadFont(const char* path, int FontHeight)
{

	if (LoadedFonts[path]) {
		return LoadedFonts[path];
	}

	std::fstream file = std::fstream(path, std::fstream::in | std::fstream::binary |std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	unsigned char* buffer = static_cast<unsigned char*>(malloc(sizeof(unsigned char) * size));

	file.read((char*)buffer, size);

	file.close();
	std::string name = std::string(path);
	size_t end;
	if ((end = name.find_last_of('.')) != name.npos) {

		name = name.substr(0, end);
	}
	size_t start;
	if ((start = name.find_last_of('/')) != name.npos || (start = name.find_last_of('\\')) != name.npos) {
		name = name.substr(start + 1);
	}
	Core::Font* f = new Core::Font(name, buffer, FontHeight);

	LoadedFonts[path] = f;

	return f;
}

void Sphynx::ResourceManager::Release(Core::Font* font)
{
}

void Sphynx::ResourceManager::Release(Core::Graphics::Texture* texture)
{
}

void Sphynx::ResourceManager::Release(Core::Graphics::Shader* shader)
{
}

void Sphynx::ResourceManager::ReleaseResources()
{
}

void Sphynx::ResourceManager::ReleaseFonts()
{
	for (auto& f : LoadedFonts) {
		delete f.second;
	}
	LoadedFonts.clear();
}

void Sphynx::ResourceManager::ReleaseTextures()
{
}