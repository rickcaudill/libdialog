#include "common.h"

BitmapImage* GetImage( const String& cPath )
{
	Resources cCol(get_image_id());
	ResStream *pcStream = cCol.GetResourceStream(cPath);
	BitmapImage *pcImage = new BitmapImage(pcStream);
	pcImage->SetSize(os::Point(24,24));
	return pcImage;
}

os::String ReadFileFromResource(const String& cPath)
{
	Resources cCol(get_image_id());
	ResStream *pcStream = cCol.GetResourceStream(cPath);

	os::String ret = "";
	char buffer[100];
	for( ;; )
	{
		memset(buffer,'\0',sizeof(buffer));
		int bytesRead = pcStream->Read( buffer, sizeof( buffer )-1);
		if (bytesRead == 0)
		{
			ret += buffer;
			break;
		}
		ret += buffer;
	}
	return ret;
}
