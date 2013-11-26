#ifndef DAY_NIGHT_HANDLER_H
#define DAY_NIGHT_HANDLER_H

#include <d3dx10.h>
#include "../Utilities/Macros.h"

namespace gfx
{

	class DayNightHandler
	{
		public:
			DayNightHandler(float time);
			~DayNightHandler();
			void setTime(float time);
			D3DXVECTOR3 getLightDirection();
			D3DXVECTOR3 getLightPosition();
			D3DXCOLOR getAmbientColor();
		private:
			D3DXVECTOR3 mLightPower;
			float mTime;
			bool mNight;
	};

}

#endif