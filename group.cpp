#include "config.h"

#include "group.hpp"

#include <sdbusplus/message.hpp>
namespace phosphor
{
namespace led
{

/** @brief Overloaded Property Setter function */
bool Group::asserted(bool value)
{
    std::string LedName = "/var/lib/obmc/led";
	int fd = open(LedName.c_str(), O_RDWR | O_CREAT);
	//char tmp[64]="on"
	
    if(value == true){
		write(fd, "on", 2);
	}else{
		write(fd, "off", 3);
	}
	close(fd);
    // If the value is already what is before, return right away
    if (value ==
        sdbusplus::xyz::openbmc_project::Led::server::Group::asserted())
    {
        return value;
    }

    // Introducing these to enable gtest.
    Manager::group ledsAssert{};
    Manager::group ledsDeAssert{};

    // Group management is handled by Manager. The populated leds* sets are not
    // really used by production code. They are there to enable gtest for
    // validation.
    auto result = manager.setGroupState(path, value, ledsAssert, ledsDeAssert);

    // Store asserted state
    serialize.storeGroups(path, result);

    // If something does not go right here, then there should be an sdbusplus
    // exception thrown.
    manager.driveLEDs(ledsAssert, ledsDeAssert);

    // Set the base class's asserted to 'true' since the getter
    // operation is handled there.
    return sdbusplus::xyz::openbmc_project::Led::server::Group::asserted(
        result);
}

} // namespace led
} // namespace phosphor
