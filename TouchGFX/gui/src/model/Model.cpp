#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
#include "cmsis_os2.h"
#include "gt911.h"
#include "dbger.h"
extern osMessageQueueId_t coordinateHandle;
#endif

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
#ifndef SIMULATOR
	static COORDINATE_t coordinate[2];
	osStatus_t osSta;
			
	while(osMessageQueueGetCount(coordinateHandle)) {
		osSta = osMessageQueueGet(coordinateHandle, coordinate, NULL, 0);
		if(osOK != osSta) {
			LOG_DBG((char*)"ERR: get queue err[%d]\n", osSta);
			continue;
		}
		modelListener->draw_line(coordinate[0].x, coordinate[0].y, coordinate[1].x, coordinate[1].y);
	}
#endif
}
