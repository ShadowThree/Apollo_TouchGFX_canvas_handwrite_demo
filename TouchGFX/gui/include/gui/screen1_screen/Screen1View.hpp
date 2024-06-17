#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <list>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
		virtual void recognize();
		virtual void addLine();
		virtual void clear();
		virtual void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
protected:
	static const uint32_t CANVAS_BUFFER_SIZE = 500;
	uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

	touchgfx::PainterRGB565 linePainter;
	std::list<touchgfx::Line*> lines;
};

#endif // SCREEN1VIEW_HPP
