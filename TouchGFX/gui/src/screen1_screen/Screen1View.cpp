#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/Color.hpp>

#ifdef SIMULATOR
#include <touchgfx/Utils.hpp>
#else
#include "dbger.h"
#include "atk_ncr.h"
#endif

static uint32_t randomNum(uint32_t min, uint32_t max) {
  return rand() % (max - min + 1) + min;
}

Screen1View::Screen1View()
{
#ifdef SIMULATOR	
  touchgfx_printf("sizeof(touchgfx::Line): %d bytes\n", sizeof(touchgfx::Line));
#else
	LOG_DBG((char*)"sizeof(touchgfx::Line): %d bytes\n", sizeof(touchgfx::Line));
#endif
	touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
  linePainter.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::recognize()
{
#ifdef SIMULATOR
	char str[] = "ABCDE";
	touchgfx_printf("recognize %s\n", str);
	Unicode::strncpy(txtResultBuffer, str, TXTRESULT_SIZE);
	txtResult.invalidate();
#else
	for(uint16_t i = 0; i < point_num; i++) {
		if(i % 4 == 0) {
			LOG_DBG((char*)"\n");
		}
		LOG_DBG((char*)" P(%d,%d)", draw_coor[i].x, draw_coor[i].y);
	}
	LOG_DBG((char*)"\n");
		
	alientek_ncr(draw_coor, point_num, CHAR_NUM, RECOGNIZE_ALL, result);
	LOG_DBG((char*)"recognize: %s\n", result);
	Unicode::strncpy(txtResultBuffer, result, TXTRESULT_SIZE);
	txtResult.invalidate();
#endif
}

void Screen1View::addLine()
{
  touchgfx::Line* line = new touchgfx::Line();
	if(line == NULL) {
		LOG_DBG((char*)"ERR: heap NOT enough for draw more line!\n");
		return;
	}
	
	if(point_num == 0) {
		draw_coor[0].x = randomNum(0, 300);
		draw_coor[0].y = randomNum(0, 300);
		point_num = 1;
	}
	draw_coor[point_num].x = randomNum(0, 300);
	draw_coor[point_num++].y = randomNum(0, 300);
	
  lines.push_back(line);
  line->setPosition(150, 150, 300, 300);
  line->setPainter(linePainter);
  line->setLineWidth(5);
  line->setLineEndingStyle(touchgfx::Line::ROUND_CAP_ENDING);
  add(*line);
  line->setStart(draw_coor[point_num-2].x, draw_coor[point_num-2].y);
  line->setEnd(draw_coor[point_num-1].x, draw_coor[point_num-1].y);
  line->invalidate();
	
#ifdef SIMULATOR	
  touchgfx_printf("add line %d\n", lines.size());
#else
	LOG_DBG((char*)"add line %d\n", lines.size());
#endif
}

void Screen1View::clear()
{
#ifdef SIMULATOR	
  touchgfx_printf("delete %d lines\n", lines.size());
#else
	LOG_DBG((char*)"delete %d lines\n", lines.size());
#endif
  for(std::list<touchgfx::Line*>::iterator it = lines.begin(); it != lines.end(); it++) {
    (*it)->setVisible(false);
    (*it)->invalidate();
    remove(**it);
    delete *it;
  }
  lines.clear();
	point_num = 0;
	memset(result, 0, CHAR_NUM+1);
	Unicode::strncpy(txtResultBuffer, result, TXTRESULT_SIZE);
	txtResult.invalidate();
}

void Screen1View::draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	touchgfx::Line* line = new touchgfx::Line();
	if(line == NULL) {
		LOG_DBG((char*)"ERR: heap NOT enough for draw more line!\n");
		return;
	}
	
	//LOG_DBG((char*)"L (%d,%d) --> (%d,%d)\n", x1, y1, x2, y2);
  lines.push_back(line);
  line->setPosition(150, 150, 300, 300);
  line->setPainter(linePainter);
  line->setLineWidth(5);
  line->setLineEndingStyle(touchgfx::Line::ROUND_CAP_ENDING);
  add(*line);
  line->setStart(x1-150, y1-150);
  line->setEnd(x2-150, y2-150);
  line->invalidate();
}