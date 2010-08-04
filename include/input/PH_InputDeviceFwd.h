#ifndef PH_INPUT_DEVICE_FWD_H
#define PH_INPUT_DEVICE_FWD_H

#include <boost/intrusive_ptr.hpp>

namespace Phobos
{
	class InputDevice_c;

	typedef boost::intrusive_ptr<InputDevice_c> InputDevicePtr_t;
}

#endif