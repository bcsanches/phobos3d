/*
Phobos 3d
September 2011
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include <boost/test/unit_test.hpp>

#include <Phobos/Engine/Math/Accelerometer.h>

BOOST_AUTO_TEST_CASE(Accelerometer_basic)
{
	Phobos::Engine::Math::Accelerometer accel(5);	

	BOOST_CHECK(accel.GetValue() == 0);

	accel.SetButtonState(1.0f);
	BOOST_CHECK(accel.GetValue() == 0);

	accel.Update(5);
	BOOST_CHECK(accel.GetValue() == 1);

	accel.SetButtonState(-1.0f);
	accel.Update(5);
	BOOST_CHECK(accel.GetValue() == 0);

	accel.Update(5);
	BOOST_CHECK(accel.GetValue() == -1);

	accel.SetButtonState(0);
	accel.Update(2.5);
	BOOST_CHECK(accel.GetValue() == -0.5);

	accel.Update(2.5);
	BOOST_CHECK(accel.GetValue() == 0);
}
