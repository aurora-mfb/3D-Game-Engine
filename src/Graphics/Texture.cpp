
#include "Graphics/Texture.h"
#include "glew/GL/glew.h"
#include "iostream"

Texture::Texture() : Id(0), Size(0, 0)
{

}

Texture::~Texture()
{
    if (Id != 0) {
        glDeleteTextures(1, &Id);
    }
}

std::shared_ptr<Texture> Texture::load(const char* filename)
{
	int width; //puntero con el ancho de la imagen
	int height; //punter con alto de la imagen
	int comp; //puntero con numero de componentes del buffer
	int req_comp = 4; //Componentes de cada pixel, 4 para que sea RGBA

	unsigned char* img = stbi_load(filename, &width, &height, &comp, req_comp);
	if (!img) {
		std::cerr << "No se pudo cargar la textura: " << filename << std::endl;
		return nullptr; // Retornar nullptr si no se pudo cargar la imagen
	}

	// Invertir la imagen verticalmente, ya que STB la carga desde la fila superior
	unsigned char* img_inverted = new unsigned char[width * height * req_comp];
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			for (int c = 0; c < req_comp; ++c) {
				img_inverted[((height - 1 - y) * width + x) * req_comp + c] =
					img[(y * width + x) * req_comp + c];
			}
		}
	}

	//Generación:
	GLuint textId; // Declarar una variable de tipo GLuint
	glGenTextures(1, &textId); // Pasar la dirección de textIdS

	//Activacion
	glBindTexture(GL_TEXTURE_2D, textId);

	//Parametros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Carga
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);

	//Mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	//Delete buffer
	delete[] img_inverted;
	stbi_image_free(img);

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->Id = textId;
	texture->Size = glm::ivec2(width, height);

	return texture;
}

uint32_t Texture::getId() const
{
	return Id;
}

const glm::ivec2& Texture::getSize() const
{
	return Size;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, Id);
}
