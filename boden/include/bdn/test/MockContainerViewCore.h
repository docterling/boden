#ifndef BDN_TEST_MockContainerViewCore_H_
#define BDN_TEST_MockContainerViewCore_H_


#include <bdn/test/MockViewCore.h>

#include <bdn/ContainerView.h>

#include <bdn/ProgrammingError.h>

namespace bdn
{
namespace test
{


/** Implementation of a "fake" container view core that does not actually show anything
    visible, but behaves otherwise like a normal container view core.
    
    See MockUiProvider.
    */
class MockContainerViewCore : public MockViewCore
{
public:
	MockContainerViewCore(ContainerView* pView)
		: MockViewCore(pView)
	{
	}
    
	
    Size calcPreferredSize( const Size& availableSpace = Size::none() ) const override
    {
	    // this core function should never have been called.
	    // The outer window is responsible for everything layout-related.
	    programmingError("ContainerView::calcPreferredSize must be overloaded in derived class.");
        return Size();
    }

	void layout() override
	{
		P<ContainerView> pView = cast<ContainerView>( getOuterViewIfStillAttached() );
		
		P<ViewLayout> pLayout = pView->calcLayout( pView->size() );
		pLayout->applyTo( pView );
	}


};

}
}

#endif