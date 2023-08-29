#ifndef __COMMON_H__
#define __COMMON_H__

#include <util/string.h>
#include <gui/image.h>
#include <util/resources.h>
#include <util/resource.h>
#include <storage/file.h>

using namespace os;

BitmapImage* GetImage( const String& cPath );
os::String ReadFileFromResource(const String& cPath);

#endif


