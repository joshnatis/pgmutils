#pragma once

#include <iostream>
#include <fstream>
#include <time.h> //noise filter, for random numbers

const int MAX_WIDTH = 1024;
const int MAX_HEIGHT = 1024;
//the number which constitutes white, determined by input image
int MAX_GRAY;


//fills image matrix with values, sets height and width in callee to height/width values
void read_image(int img[MAX_HEIGHT][MAX_WIDTH], int &height, int &width, std::ifstream &fin);
//pass in a heap allocated matrix with pixel values, it'll be written to a pgm image file
void write_image(int **out, int height, int width , std::ofstream &fout);
//pass in a stack allocated matrix with pixel values, it'll be written to a pgm image file
void write_image(int out[MAX_HEIGHT][MAX_WIDTH], int height, int width , std::ofstream &fout);


//standard image algorithm, applies func to each pixel and writes
void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout, int(*func)(int));
//apply a filter (2d matrix) to an image (convolution filters)
void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout,
	double filter[5][5], double factor, double bias);
void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout, 
	double filter[3][3], double factor, double bias);


//nonstandard algorithms (don't simply apply filter to each pixel)
void rotate(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void reflect(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void scale_down(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, int scale, std::ofstream &fout);
void scale_up(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, int scale, std::ofstream &fout);
void asciify(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, const std::string &fn);
void frame(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void sobel(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void gridlines(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void acid(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);
void soundscape(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout);


//image algorithm helpers
int noise(int pixel) { return (1 + rand() % MAX_GRAY); }
int invert(int pixel) { return MAX_GRAY - pixel; }
int nothing(int pixel) { return pixel; }
int posterize(int pixel);
char asciify(int pixel);
int sobel(int a, int b, int c, int d, int e, int f, int g, int h, int i);
int selectKth(int* data, int s, int e, int k);


/*
======================================
======================================

		IMAGE ALGORITHMS

=====================================
=====================================
*/

//fills image matrix with values, sets h and w in callee to height/width values
void read_image(int img[MAX_HEIGHT][MAX_WIDTH], int &height, int &width, std::ifstream &fin)
{
	std::string header;
	getline(fin, header); //should be p5 or p2

	// skip the comments (if any)
	while ((fin >> std::ws).peek() == '#')
		fin.ignore(4096, '\n');

	fin >> width >> height >> MAX_GRAY;

	if(width > 1024 || height > 1024)
	{
		std::cout << "Your image is too large, the maximum dimensions are " << MAX_HEIGHT << "px x " << MAX_WIDTH << "px.\n";
		exit(1);
	}

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
			fin >> img[i][j];
	}

	fin.close();
}

//standard image algorithm, applies func to each pixel and writes
void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout, int(*func)(int))
{
	fout << "P2\n";
	fout << width << " " << height << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			fout << func(img[i][j]) << " ";
		}
		fout << std::endl;
	}
}

//outputs an image of dimensions (height * 2) x width (reflection on lower half)
void reflect(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{	
	fout << "P2\n";
	fout << width << " " << height * 2 << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
			fout << img[i][j] << " " << std::endl;

		fout << std::endl;
	}

	for(int i = height - 1; i >= 0; --i)
	{
		for(int j = 0; j < width; j++)
			fout << img[i][j] << " ";
		
		fout << std::endl;
	}
}

//rotate 90 degrees counter clockwise
void rotate(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{	
	fout << "P2\n";
	fout << height << " " << width << std::endl; //flipped
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < width; ++i)
	{
		for(int j = 0; j < height; ++j)
		{
			fout << img[j][i] << " ";
		}
		fout << std::endl;
	}
}

//outputs a txt file with ascii characters resembling the original image
void asciify(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, const std::string &fn)
{
	std::ofstream fout;

	if(fn.substr(fn.size() - 3) != "txt")
	{
		std::cout << "The asciify filter requires your output file to be a txt file!\n";
		exit(1);
	}
	else
		fout.open(fn);

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			fout << asciify(img[i][j]);
		}
		fout << std::endl;
	}
}

//shrinks the image by a user-specified factor
void scale_down(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, int scale, std::ofstream &fout)
{
	fout << "P2\n";
	fout << width/scale << " " << height/scale << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; i+=scale)
	{
		for(int j = 0; j < width; j+=scale)
		{
			fout << img[i][j] << " ";
		}
		fout << std::endl;
	}
}

void scale_up(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, int scale, std::ofstream &fout)
{
	//allocate enough heap space
	int **out = new int*[height * scale];
	for(int i = 0; i < height * scale; ++i)
		out[i] = new int[width * scale];

	//copy pixel value to a scale x scale rectangle
	int val;
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			val = img[i][j];

			out[i * scale][j * scale] = val;
			out[i * scale + 1][j * scale] = val;
			out[i * scale][j * scale + 1] = val;
			out[i * scale + 1][j * scale + 1] = val;
		}
	}

	write_image(out, height * scale, width * scale, fout);
}

//pass in a heap allocated matrix with pixel values, it'll be written to a pgm image file
void write_image(int **out, int height, int width, std::ofstream &fout)
{
	fout << "P2\n";
	fout << width << " " << height << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			fout << out[i][j] << " ";
		}
		fout << std::endl;
	}

	for(int i = 0; i < height; ++i) //deallocate
		delete[] out[i];
}

//pass in a stack allocated matrix with pixel values, it'll be written to a pgm image file
void write_image(int out[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{
	fout << "P2\n";
	fout << width << " " << height << std::endl;
	fout << MAX_GRAY << std::endl;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			fout << out[i][j] << " ";
		}
		fout << std::endl;
	}
}

void frame(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{
	fout << "P2\n";
	fout << width << " " << height << std::endl;
	fout << MAX_GRAY << std::endl;

	const int BORDER_WIDTH = 5;

	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			if(i < BORDER_WIDTH || j < BORDER_WIDTH
				|| i > (height - BORDER_WIDTH) || j > (width - BORDER_WIDTH))
			{
				fout << 0 << " ";
			}
			else
				fout << img[i][j] << " ";
		}
		fout << std::endl;
	}
}

void gridlines(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{
	int grid_size = 32;
	int grid_color = 0;

	for(int i = 0; i <= height - grid_size; i += grid_size)
	{
		for(int j = 0; j <= width - grid_size; j += grid_size)
		{
			
			for(int k = 0; k < grid_size; ++k)
			{
				img[i + k][j] = grid_color;
				img[i][j + k] = grid_color;
			}
		}
	}

	write_image(img, height, width, fout);
}

void sobel(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{
	int **out = new int*[height];
	for(int i = 0; i < height; ++i)
		out[i] = new int[width];

	int a,b,c,d,e,f,g,h,i;

	for(int row = 0; row < height; row++)
	{
		for(int col = 0; col < width; col++)
		{
			//we don't have 8 neighbors
			if(col == width || col == 0 || row == 0 || row == height)
			{
				//handle border pixels by assuming all neighbors are black
				out[row][col] = sobel(0,0,0,0,img[row][col],0,0,0,0);
			}
			else
			{
				a = img[row-1][col-1]; //pixel top left
				b = img[row-1][col]; //pixel above
				c = img[row-1][col+1]; //pixel top right
				d = img[row][col-1]; //pixel to the left
				e = img[row][col]; //current pixel
				f = img[row][col+1]; //pixel to the right
				g = img[row+1][col-1]; //pixel bottom left
				h = img[row+1][col]; //pixel under
				i = img[row+1][col+1]; //pixel bottom right

				out[row][col] = sobel(a,b,c,d,e,f,g,h,i);
			}
		}
	}

	write_image(out, height, width, fout);
}

void soundscape(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{
	const int filterWidth = 30;
	const int filterHeight = 20;

	int **out = new int*[height];
	for(int i = 0; i < height; ++i)
		out[i] = new int[width];

	//apply the filter
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			int gray = 0;
			int n = 0;
			//set the color values in the arrays
			for(int fY = 0; fY < filterHeight; fY++)
			{
				for(int fX = 0; fX < filterWidth; fX++)
				{
					int imageX = (j - filterWidth / 2 + fX + width) % width;
					int imageY = (i - filterHeight / 2 + fY + height) % height;
					out[i][n] = img[imageY][imageX];
					n++;
				}
			}

			int filterSize = filterWidth * filterHeight;
			gray = selectKth(out[i], 0, filterSize, filterSize / 2);
			out[i][j] = gray;
		}
	}

	write_image(out, height, width, fout);
}

void acid(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout)
{
	const int filterWidth = 2;
	const int filterHeight = 9;

	int **out = new int*[height];
	for(int i = 0; i < height; ++i)
		out[i] = new int[width];

	//apply the filter
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			int gray = 0;
			//set the color values in the arrays
			for(int fY = 0; fY < filterHeight; fY++)
			{
				for(int fX = 0; fX < filterWidth; fX++)
				{
					int imageX = (j - filterWidth / 2 + fX + width) % width;
					int imageY = (i - filterHeight / 2 + fY + height) % height;
					gray += img[imageY][imageX];
				}
			}

			int filterSize = filterWidth * filterHeight;
			gray = (gray + selectKth(img[i], 0, filterSize, filterSize / 2)) % MAX_GRAY;
			out[i][j] = gray;
		}
	}

	write_image(out, height, width, fout);
}

void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout, double filter[5][5], double factor, double bias)
{
	int **out = new int*[height];
	for(int i = 0; i < height; ++i)
		out[i] = new int[width];

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			int gray = 0;

			//multiply every value of the filter with corresponding image pixel
			for(int fY = 0; fY < 5; fY++)
			{
				for(int fX = 0; fX < 5; fX++)
				{
					int imageX = (i - 5 / 2 + fX + width) % width;
					int imageY = (j - 5 / 2 + fY + height) % height;
					gray += img[imageY][imageX] * filter[fY][fX];
				}
			}

			//truncate values smaller than zero and larger than 255
			out[j][i] = std::min(std::max(int(factor * gray + bias), 0), MAX_GRAY);
		}
	}

	write_image(out, height, width, fout);
}

void apply_filter(int img[MAX_HEIGHT][MAX_WIDTH], int height, int width, std::ofstream &fout, double filter[3][3], double factor, double bias)
{
	int **out = new int*[height];
	for(int i = 0; i < height; ++i)
		out[i] = new int[width];

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			int gray = 0;

			//multiply every value of the filter with corresponding image pixel
			for(int fY = 0; fY < 5; fY++)
			{
				for(int fX = 0; fX < 5; fX++)
				{
					int imageX = (i - 5 / 2 + fX + width) % width;
					int imageY = (j - 5 / 2 + fY + height) % height;
					gray += img[imageY][imageX] * filter[fY][fX];
				}
			}

			//truncate values smaller than zero and larger than 255
			out[j][i] = std::min(std::max(int(factor * gray + bias), 0), MAX_GRAY);
		}
	}

	write_image(out, height, width, fout);
}


/*
======================================
======================================

	IMAGE ALGORITHM HELPERS

=====================================
=====================================
*/


int posterize(int pixel)
{
	if(pixel > MAX_GRAY/2) return MAX_GRAY;
	else return 0;
}

char asciify(int pixel)
{
	if(pixel > 200) return '.';
	else if(pixel > 100) return '*';
	else return '#';
}

int sobel(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
	int color = (g+(2*h)+i)-(a+2*b+c); //Sobel edge detection formula

	if(color > MAX_GRAY)
		return MAX_GRAY;

	if(color < 0)
		return 0;

	return color;
}

// selects the k-th largest element from the data between start and end (end exclusive)
int selectKth(int* data, int s, int e, int k)
{
	// 5 or less elements: do a small insertion sort
	if(e - s <= 5)
	{
		for(int i = s + 1; i < e; i++)
			for(int j = i; j > 0 && data[j - 1] > data[j]; j--) std::swap(data[j], data[j - 1]);
		return s + k;
	}

	int p = (s + e) / 2; // choose simply center element as pivot

	// partition around pivot into smaller and larger elements
	std::swap(data[p], data[e - 1]); // temporarily move pivot to the end
	int j = s;  // new pivot location to be calculated
	for(int i = s; i + 1 < e; i++)
		if(data[i] < data[e - 1]) std::swap(data[i], data[j++]);
	std::swap(data[j], data[e - 1]);

	// recurse into the applicable partition
	if(k == j - s) return s + k;
	else if(k < j - s) return selectKth(data, s, j, k);
	else return selectKth(data, j + 1, e, k - j + s - 1); // subtract amount of smaller elements from k
}