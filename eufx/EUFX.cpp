//todo --
//(1) make filters compositable. 
// 	this probably means heap allocating an out** and returning it from each function
//	this way i dont have to write and read a file to apply multiple filters, i can just
//	pass in the out array to multiple places
//  drawbacks -- no more stack allocation (? maybe figure out how to deal with out[MAX_WIDTH][MAX_HEIGHT])
//
// (2) remove cin, only use command line args (scale_up, scale_down)
// (3) fix scale_down where too many scales breaks the image
// (4) properly credit https://lodev.org/cgtutor/filtering.html

#include <iostream>
#include <vector>
#include <fstream>

#include "FILTERS.hpp"


//check if element is contained in an iterable container (used to check validity of filters)
#define contains(v, x) std::find(v.begin(), v.end(), x) != v.end()


//housekeeping helpers
void list_filters(const std::vector<std::string> &filters);
bool handle_errors(int argc, char **argv, const std::vector<std::string> &FILTERS);


/*
======================================
======================================

				MAIN

=====================================
=====================================
*/

int main(int argc, char **argv)
{
	const std::vector<std::string> FILTERS = 
	{"reflect", "rotate", "asciify", "scale_down", "scale_up", "noise",
	"posterize", "nothing", "invert", "frame", "box_blur", 
	"gaussian_blur", "motion_blur", "sobol_edge_detection", 
	"horizontal_edge_detection", "vertical_edge_detection",
	"45_edge_detection", "all_edge_detection", "edge_detection_2",
	"sharpen1", "sharpen2", "deepfry", "gridlines", "emboss", "acid",
	"soundscape"};

	bool no_specified_output_file = handle_errors(argc, argv, FILTERS);

	std::string input_img = argv[3];
	std::ifstream fin;
	fin.open(input_img);
	if (fin.fail()) 
	{
		std::cout << "Unable to read input file.\n";
		exit(1);
	}

	std::string filter = argv[2];
	std::string output_img;

	if(no_specified_output_file)
	{
		//remove any path from the input file name
		for(int i = 0; i < input_img.size(); ++i)
		{
			if(input_img[i] == '/')
			{
				input_img = input_img.substr(i+1);
				i = 0;
			}
		}

		//write to a default output file (input_<filter>.pgm/txt)
		output_img = input_img.substr(0, input_img.length() - 4) + "_" + filter;

		if(filter == "asciify") output_img += ".txt";
		else output_img += ".pgm";

	}
	else output_img = argv[4];
	
	std::ofstream fout;
	fout.open(output_img);

	int img[MAX_HEIGHT][MAX_WIDTH];
	int height, width; //set by read_image
	read_image(img, height, width, fin);

	if(filter == "reflect") reflect(img, height, width, fout);
	else if(filter == "rotate") rotate(img, height, width, fout);
	else if(filter == "frame") frame(img, height, width, fout);
	else if(filter == "sobol_edge_detection") sobel(img, height, width, fout);
	else if(filter == "gridlines") gridlines(img, height, width, fout);
	else if(filter == "acid") acid(img, height, width, fout);

	else if(filter == "noise") apply_filter(img, height, width, fout, noise);
	else if(filter == "posterize") apply_filter(img, height, width, fout, posterize);
	else if(filter == "nothing") apply_filter(img, height, width, fout, nothing);
	else if(filter == "invert") apply_filter(img, height, width, fout, invert);

	else if(filter == "box_blur")
	{
		double filter[5][5] =
		{
			0, 0, 1, 0, 0,
			0, 1, 1, 1, 0,
			1, 1, 1, 1, 1,
			0, 1, 1, 1, 0,
			0, 0, 1, 0, 0,
		};

		//to maintain brightness of image, filter must sum to 1
		//but we'll just divide factor by total to avoid filling filter with 1/13
		double factor = 1.0 / 13.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "gaussian_blur")
	{
		double filter[5][5] =
		{
		  1,  4,  6,  4,  1,
		  4, 16, 24, 16,  4,
		  6, 24, 36, 24,  6,
		  4, 16, 24, 16,  4,
		  1,  4,  6,  4,  1,
		};

		double factor = 1.0 / 256.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "motion_blur")
	{
		double filter[5][5] =
		{
			1, 0, 0, 0, 0,
			0, 1, 0, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, 1, 0,
			0, 0, 0, 0, 1,
		};

		double factor = 1.0 / 5.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "horizontal_edge_detection")
	{
		double filter[5][5] =
		{
			0,  0, -1,  0,  0,
			0,  0, -1,  0,  0,
			0,  0,  2,  0,  0,
			0,  0,  0,  0,  0,
			0,  0,  0,  0,  0,
		};

		double factor = 1.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "vertical_edge_detection")
	{
		double filter[5][5] =
		{
			0,  0, -1,  0,  0,
			0,  0, -1,  0,  0,
			0,  0,  4,  0,  0,
			0,  0, -1,  0,  0,
			0,  0, -1,  0,  0,
		};

		double factor = 1.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "45_edge_detection")
	{
		double filter[5][5] =
		{
			-1,  0,  0,  0,  0,
			0, -2,  0,  0,  0,
			0,  0,  6,  0,  0,
			0,  0,  0, -2,  0,
			0,  0,  0,  0, -1,
		};

		double factor = 1.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "all_edge_detection")
	{
		double filter[3][3] =
		{
			-1, -1, -1,
			-1,  8, -1,
			-1, -1, -1
		};

		double factor = 1.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "sharpen1")
	{
		double filter[3][3] =
		{
			-1, -1, -1,
			-1,  9, -1,
			-1, -1, -1
		};

		double factor = 1.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "sharpen2")
	{
		double filter[5][5] =
		{
		  -1, -1, -1, -1, -1,
		  -1,  2,  2,  2, -1,
		  -1,  2,  8,  2, -1,
		  -1,  2,  2,  2, -1,
		  -1, -1, -1, -1, -1,
		};

		double factor = 1.0 / 8.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "deepfry")
	{
		double filter[5][5] =
		{
		  -1, -1, -1, -1, -1,
		  -1,  2,  2,  2, -1,
		  -1,  2,  8,  2, -1,
		  -1,  2,  2,  2, -1,
		  -1, -1, -1, -1, -1,
		};

		double factor = 1.0 / 8.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
		int degrees_fried = 4; //larger == more fried

		for(int i = 0; i < degrees_fried; ++i)
		{
			fin.clear();
			fin.seekg(0);
			fin.open(output_img);
			read_image(img, height, width, fin);
			fout.open(output_img);
			apply_filter(img, height, width, fout, filter, factor, bias);
		}
	}

	else if(filter == "edge_detection_2")
	{
		double filter[3][3] =
		{
			1,  1,  1,
			1, -7,  1,
			1,  1,  1
		};

		double factor = 1.0;
		double bias = 0.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "emboss")
	{
		double filter[5][5] =
		{
			-1, -1, -1, -1,  0,
			-1, -1, -1,  0,  1,
			-1, -1,  0,  1,  1,
			-1,  0,  1,  1,  1,
			0,  1,  1,  1,  1
		};

		double factor = 1.0;
		double bias = 128.0;

		apply_filter(img, height, width, fout, filter, factor, bias);
	}

	else if(filter == "soundscape")
	{
		//filter is pretty intensive, scale the image down first
		int width_threshold = 128;
		int projected_width = width;
		int resizes = 0;
		while(projected_width > width_threshold)
		{
			projected_width /= 2;
			resizes++;
		}

		std::string temp_fn = ".soundscape_small.pgm";
		std::ofstream temp(temp_fn);
		scale_down(img, height, width, resizes, temp);
		temp.close();

		//read in scaled-down image
		std::ifstream temp2(temp_fn);
		read_image(img, height, width, temp2);
		temp2.close();

		remove(temp_fn.c_str()); //delete temporary scaled_down image file
		soundscape(img, height, width, fout);
	}

	else if(filter == "asciify")
	{
		//delete output file, we'll rewrite it in the asciify() function
		//in case the file extension was wrong, since we made no checks
		remove(argv[4]);
		asciify(img, height, width, output_img);
	}

	else if(filter == "scale_down")
	{
		int factor;
		std::cout << "Scale down by what factor? (e.g. 2 to half): ";
		std::cin >> factor;

		if (std::cin.fail() || factor <= 0)
		{
			std::cout << "Invalid input, factor should be an integer greater than 0.\n";
			remove(argv[4]); //delete now-empty file
			exit(1);
		}

		if(width / factor < 1 || height / factor < 1)
		{
			std::cout << "Maybe that's a bit too small, chump. Try a smaller number.\n";
			remove(argv[4]); //delete file
			exit(1);
		}

		scale_down(img, height, width, factor, fout);
	}

	else if(filter == "scale_up")
	{
		int factor;
		std::cout << "Scale up by what factor? (e.g. 2 to double): ";
		std::cin >> factor;

		if (std::cin.fail() || factor <= 0)
		{
			std::cout << "Invalid input, factor should be an integer greater than 0.\n";
			remove(argv[4]); //delete file
			exit(1);
		}

		if(factor > 10)
		{
			std::cout << "Maybe that's a bit too large, chump. Try a smaller number.\n";
			remove(argv[4]); //delete file
			exit(1);
		}

		scale_up(img, height, width, factor, fout);
	}

	else std::cout << "Invalid filter! Use the --help flag for a list of available filters.\n";

	fout.close();
	return 0;
}


/*
======================================
======================================

	PROGRAM HOUSEKEEPING HELPERS

=====================================
=====================================
*/


void list_filters(const std::vector<std::string> &filters)
{
	for(int i = 0, s = filters.size(); i < s; ++i)
		std::cout << "-" << filters[i] << "\n";
}

bool handle_errors(int argc, char **argv, const std::vector<std::string> &FILTERS)
{
	std::string usage = " --filter <filter_type> input.pgm [output.pgm]";

	if(argc == 1)
	{
		std::cout << "Usage: " << argv[0] << usage << "\n";
		std::cout << "Try the --help flag\n";
		exit(1);
	}

	std::string flag = argv[1]; //valid: --filter or --help

	if(flag == "-h" || flag == "--help" || flag == "help")
	{
		std::cout << "Usage: " << argv[0] << usage << "\n";
		std::cout << "\nAvailable filters:\n";
		list_filters(FILTERS);
		std::cout << "\n";
		exit(1);
	}

	if(argc < 4 || flag != "--filter")
	{
		std::cout << "Usage: " << argv[0] << usage << "\n";
		exit(1);
	}

	std::string input_img = argv[3];

	if(input_img.length() < 4 || input_img.substr(input_img.size() - 4) != ".pgm")
	{
		std::cout << "Input file must be of type pgm for this filter!\n";
		exit(1);
	}

	std::string filter = argv[2];
	bool no_specified_output_file; //user did not enter an output file name
	std::string output_img;

	if(argc == 4) no_specified_output_file = true; //missing 5th argument
	else if(argc == 5)
	{
		no_specified_output_file = false; //5th argument is output file
		output_img = argv[4];
	}
	else
	{
		std::cout << "Usage: " << argv[0] << usage << "\n";
		exit(1);
	}

	if(!(contains(FILTERS, filter)))
	{
		std::cout << "Invalid filter! Use the --help flag for a list of available filters.\n";
		exit(1);
	}

	if(input_img == output_img)
	{
		std::cout << "Input and output images cannot be the same file!\n";
		exit(1);
	}

	//check if output file is the proper type of file
	if(!no_specified_output_file && 
		(output_img.length() < 4 || 
		(filter != "asciify" && output_img.substr(output_img.size() - 4) != ".pgm")))
	{
		std::cout << "Output file must be of type pgm for this filter!\n";
		exit(1);
	}

	return no_specified_output_file;
}