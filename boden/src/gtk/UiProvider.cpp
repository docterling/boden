#include <bdn/init.h>
#include <bdn/gtk/UiProvider.h>

#include <bdn/gtk/WindowCore.h>
#include <bdn/gtk/ContainerViewCore.h>
#include <bdn/gtk/ButtonCore.h>
#include <bdn/gtk/TextViewCore.h>
#include <bdn/gtk/ScrollViewCore.h>

#include <bdn/ViewCoreTypeNotSupportedError.h>

namespace bdn
{
    
P<IUiProvider> getDefaultUiProvider()
{
    return &bdn::gtk::UiProvider::get();
}
    
}


namespace bdn
{
namespace gtk
{


BDN_SAFE_STATIC_IMPL( UiProvider, UiProvider::get );


UiProvider::UiProvider()
{
    _pLayoutCoordinator = newObj<LayoutCoordinator>();
    
    GtkSettings* pSettings = gtk_settings_get_default();
    
    int fontSize = 0;
    
    gchar* pFontName = nullptr;
    g_object_get( pSettings, "gtk-font-name", &pFontName, NULL );
    
    if(pFontName!=nullptr)
    {            
        String fontName(pFontName);
    
        // The font name has the format "NAME SIZE"
        // reverse iterate through the font and find the last number in the name,
        
        int digitFactor = 1;
        String::Iterator it = fontName.end();
        while(it!=fontName.begin())
        {
            it--;
            
            char32_t chr = *it;
            
            if(isdigit(chr))
            {   
                fontSize += ((int)(chr-'0')) * digitFactor;
                digitFactor*=10;
            }
            else
            {
                if(fontSize>0)
                {
                    // we found a valid number. stop here.
                    break;                    
                }
                
                fontSize = 0;                
                digitFactor = 1;
            }
        }
    }
    
    if(fontSize<=0)
    {
        // unable to extract a font size. Log an error and use 11 as a default.
        logError("Unable to determine default GTK font size. Using default.");
        
        fontSize = 11;
    }
    
    // the font size is in points, which is "72 DPI". The virtual device coordinates
    // (=DIPs) are assumed to be 96 dpi. So we have to multiple accordingly.
    _semDips = fontSize / 72.0 * 96.0;
}


String UiProvider::getName() const
{
    return "gtk";
}
    
P<IViewCore> UiProvider::createViewCore(const String& coreTypeName, View* pView)
{
    if(coreTypeName == ContainerView::getContainerViewCoreTypeName() )
        return newObj<ContainerViewCore>( cast<ContainerView>(pView) );
    
    else if(coreTypeName == Button::getButtonCoreTypeName() )
        return newObj<ButtonCore>( cast<Button>(pView) );
    
    else if(coreTypeName == Window::getWindowCoreTypeName() )
        return newObj<WindowCore>( cast<Window>(pView) );
        
    else if(coreTypeName == TextView::getTextViewCoreTypeName() )
        return newObj<TextViewCore>( cast<TextView>(pView) );
        
    else if(coreTypeName == ScrollView::getScrollViewCoreTypeName() )
        return newObj<ScrollViewCore>( cast<ScrollView>(pView) );
    
    else
        throw ViewCoreTypeNotSupportedError(coreTypeName);
}

P<ITextUi> UiProvider::getTextUi()
{
    {
        MutexLock lock( _textUiInitMutex );
        if(_pTextUi==nullptr)
            _pTextUi = newObj< ViewTextUi >();
    }

    return _pTextUi;
}


}
}

