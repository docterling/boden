#ifndef BDN_Font_H_
#define BDN_Font_H_

#include <Windows.h>

namespace bdn
{


class Font : public Base
{
public:
	explicit Font(const LOGFONT& fontInfo);
	~Font();


	/** Returns the font's metrics.*/
	TEXTMETRIC getMetrics() const;


	/** Returns the font size in pixels on the screen.*/
	double getSizePixels() const;


	HFONT getHandle()
	{
		return _handle;
	}

protected:
	HFONT _handle;
};
	



}


#endif
