#include <rengine.hpp>
#include <Rengine/Core/entrypoint.hpp>
#include "EditorLayer.hpp"
#include <imgui.h>

#include "itkConnectedComponentImageFilter.h"
#include <itkRescaleIntensityImageFilter.h>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"


namespace Rengin{
class Editor :public Application
{
public:
    Editor() :Application()
    {
        PushLayer(new EditorLayer());
    }

    ~Editor(){}
};

Application* CreateApplication()
{
    return new Editor();
}

using namespace std;

void print(const std::string& file, itk::Image<unsigned char, 2>* image)
{
	ofstream fout(file);
	auto size = image->GetBufferedRegion().GetSize();
	for (auto i = 0; i < size[0]; i++)
	{
		for (auto j = 0; j < size[1]; j++)
		{
			itk::Image<unsigned char, 2>::IndexType index{ {i,j} };
			auto c = image->GetPixel(index);
			fout << (int)c;
		}
		fout << std::endl;
	}
}


int main2(int argc, char *argv[])
{
	// 1、获取二值化的分割结果mask
	const char* input_path = "D:/documents/vs2019/itk_tutorial/largestcompent/build/RelWithDebInfo/left_femur.nrrd";// 图像保存的路径
	using ImageType = itk::Image<unsigned char, 3>;
	using ReaderFilter = itk::ImageFileReader<ImageType>;
	auto reader = ReaderFilter::New();
	reader->SetFileName(input_path);
	reader->Update();
	ImageType::Pointer image = reader->GetOutput();

	// 2, 调用ITK的itkConnectedComponentImageFilter
	using ConnectedComponentFilter = itk::ConnectedComponentImageFilter<ImageType, ImageType>;
	auto connnectedComponentFilter = ConnectedComponentFilter::New();
	connnectedComponentFilter->SetInput(image);
	connnectedComponentFilter->Update();

	// 3,调用ITK的itkLabelShapeKeepNObjectsImageFilter
	using LabelShapeKeepNObjectsFilter = itk::LabelShapeKeepNObjectsImageFilter<ImageType>;
	auto labelShapeKeepNObjectsFilter = LabelShapeKeepNObjectsFilter::New();
	labelShapeKeepNObjectsFilter->SetInput(connnectedComponentFilter->GetOutput());
	labelShapeKeepNObjectsFilter->SetBackgroundValue(0);
	labelShapeKeepNObjectsFilter->SetNumberOfObjects(1);
	labelShapeKeepNObjectsFilter->SetAttribute(
		LabelShapeKeepNObjectsFilter::LabelObjectType::NUMBER_OF_PIXELS);
	labelShapeKeepNObjectsFilter->Update();
    // 4、将Label便签转换为1
    typedef itk::RescaleIntensityImageFilter<ImageType, ImageType> RescaleFilterType;
    RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
    rescaleFilter->SetOutputMinimum(0);
    rescaleFilter->SetOutputMaximum(1);
    rescaleFilter->SetInput(labelShapeKeepNObjectsFilter->GetOutput());
    rescaleFilter->Update();
    //ImageType::Pointer largestccimage = rescaleFilter->GetOutput(); 

	// 5,保存最大连通区域图像

	const char* out_path = "D:/documents/vs2019/itk_tutorial/largestcompent/build/RelWithDebInfo/output.nii.gz"; // 保存输出结果的路径
	using WriterFilter = itk::ImageFileWriter<ImageType>;
	auto writer = WriterFilter::New();
	writer->SetFileName(out_path);
	writer->SetInput(rescaleFilter->GetOutput());
	writer->Update();
	 
	return EXIT_SUCCESS;
}

}