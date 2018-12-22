#include "texture.h"
#include "stb_image.h"

Texture::Texture() {
	
}

Texture::Texture(std::string filepath, bool flip) {
	path = filepath;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &colorChannels, STBI_rgb_alpha);

	hasTransparency = false;
	for (int i = 3; i < width*height*colorChannels; i += 4) {
		if (data[i] != 0xff) {
			printf("Transparency detected in: %s\n", filepath.c_str());
			hasTransparency = true;
			break;
		}
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture Texture::White() {
	Texture tex;
	const size_t width = 2, height = 2, channels = 3;

	tex.path = "white";
	tex.width = width;
	tex.height = height;
	tex.colorChannels = channels;
	tex.hasTransparency = false;

	static unsigned char data[width* height*channels];
	std::fill_n(data, width* height*channels, 0xff);

	glGenTextures(1, &tex.textureId);
	glBindTexture(GL_TEXTURE_2D, tex.textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

Texture Texture::NormalBlue() {
	Texture tex;
	const size_t width = 2, height = 2, channels = 3;

	tex.path = "white";
	tex.width = width;
	tex.height = height;
	tex.colorChannels = channels;
	tex.hasTransparency = false;

	static unsigned char data[width* height*channels] =
	{	//r   g     b     r     g     b
		0x80, 0x80, 0xFF, 0x80, 0x80, 0xFF,
		0x80, 0x80, 0xFF, 0x80, 0x80, 0xFF
	};

	glGenTextures(1, &tex.textureId);
	glBindTexture(GL_TEXTURE_2D, tex.textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}
