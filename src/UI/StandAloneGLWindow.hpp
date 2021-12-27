#ifndef STANDALONEGLWINDOW_HPP
#define STANDALONEGLWINDOW_HPP

#include "Application.hpp"

// ***************************************************************************
//! \brief A GLWindow not needing GLApplication.
//! \note not tested
// ***************************************************************************
class StandAloneGLWindow : public GLWindow
{
public:

    StandAloneGLWindow(uint32_t const width, uint32_t const height, const char *title)
        : GLWindow(width, height, title)
    {
        GLApplication::initGlew();
    }

    //--------------------------------------------------------------------------
    bool run()
    {
        if (!setup())
        {
            return false;
        }

        while (!shouldHalt())
        {
            if (!update())
            {
                return false;
            }
        }

        return true;
    }
};

#endif
