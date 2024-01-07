
#include <iostream>
#include"opencv2\opencv.hpp"
#include <include/parameter.h>
#include <include/paddleocr.h>
#include <include/paddlestructure.h>
#include <windows.h>

using namespace PaddleOCR;

int main(int argc, char** argv)
{
	PPOCR* ocr = new PPOCR;
	std::vector<cv::Mat> img_list;
	std::vector<cv::String> cv_all_names;
	cv::glob(FLAGS_image_dir, cv_all_names);
	std::vector<cv::String> cv_all_img_names;

	// test memory leak
    delete ocr;
    ocr = nullptr;

	for (int i = 0; i < cv_all_names.size(); i++)
	{
		cv::Mat img = cv::imread(cv_all_names[i], cv::IMREAD_COLOR);
		if (img.empty())
			continue;
		img_list.push_back(img);
		cv_all_img_names.push_back(cv_all_names[i]);
	}
	/*cv::String filepath = "F:/Hobby/OCR/data/bin/test1.bmp";
	std::vector<cv::String> cv_all_img_names;
	cv_all_img_names.push_back(filepath);
	cv::Mat img = cv::imread(cv_all_img_names[0], cv::IMREAD_COLOR);
	std::vector<cv::Mat> img_list;
	img_list.push_back(img);*/

	std::vector<std::vector<OCRPredictResult>> ocr_results =ocr->ocr(img_list, FLAGS_det, FLAGS_rec, FLAGS_cls);

	if (!Utility::PathExists(FLAGS_output))
	{
		Utility::CreateDir(FLAGS_output);
	}

	SetConsoleOutputCP(CP_UTF8);	// »√øÿ÷∆Ã® ‰≥ˆUTF-8£¨±‹√‚ ‰≥ˆ¬“¬Î

	for (int i = 0; i < cv_all_img_names.size(); ++i)
	{
		std::string file_name = Utility::basename(cv_all_img_names[i]);
		std::cout << "predict img: " << file_name << std::endl;
		Utility::print_result(ocr_results[i]);
		if (FLAGS_visualize && FLAGS_det)
		{
			cv::Mat srcimg = img_list[i];
			Utility::VisualizeBboxes(srcimg, ocr_results[i], std::string(FLAGS_output) + "/" + file_name);
		}
	}
	std::cout << "Run completed.\n";
	img_list.clear();
	ocr_results.clear();

	delete ocr;
	ocr = nullptr;
}
