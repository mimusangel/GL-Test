#include "texture.hpp"
#include <png.h>

Texture::Texture(unsigned int width, unsigned int height, unsigned char *data, GLenum format) :
_width(width), _height(height), _data(data)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, format, GL_UNSIGNED_BYTE, _data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	// delete[] data;
}

Texture::~Texture()
{
	delete[] _data;
	glDeleteTextures(1, &_textureID);
}

void		Texture::bind(void)
{
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

Texture		*Texture::LoadBMP(const char *path, GLenum format)
{
	unsigned char	header[54];
	unsigned int	dataPos;
	unsigned int	width;
	unsigned int	height;
	unsigned int	imageSize;
	unsigned char	*data;

	FILE *file = fopen(path, "rb");
	if (!file)
		throw std::logic_error("Image could not be opened\n");
	if (fread(header, 1, 54, file) != 54)
		throw std::logic_error("Not a correct BMP file\n");
	if (header[0] != 'B' || header[1] != 'M')
		throw std::logic_error("Not a correct BMP file\n");
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)
		imageSize = width * height * 3;
	if (dataPos == 0)
		dataPos = 54;
	fseek(file, dataPos, SEEK_SET);
	data = new unsigned char [imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);
	return (new Texture(width, height, data, format));
}

Texture		*Texture::LoadPNG(const char *path)
{
	int width;
	int height;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
	FILE *fp = fopen(path, "rb");
	if (!fp)
		throw std::logic_error("[read_png] File could not be opened for reading");
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png)
		throw std::logic_error("[read_png_file] Error during read_image");
	png_infop info = png_create_info_struct(png);
	if(!info)
		throw std::logic_error("");
	if(setjmp(png_jmpbuf(png)))
		throw std::logic_error("[read_png] Error during init_io");
	png_init_io(png, fp);
	png_read_info(png, info);
	width      = png_get_image_width(png, info);
	height     = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth  = png_get_bit_depth(png, info);
	if(bit_depth == 16)
		png_set_strip_16(png);
	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);
	if(png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);
	if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);
	png_read_update_info(png, info);
	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for(int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
	}
	png_read_image(png, row_pointers);
	fclose(fp);
	return (new Texture(width, height, (unsigned char *)row_pointers, GL_BGRA));
}
