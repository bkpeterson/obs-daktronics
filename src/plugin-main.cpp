/*
Plugin Name
Copyright (C) <Year> <Developer> <Email Address>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <obs-module.h>
#include <obs-frontend-api.h>

#include "dak-source-support.h"
#include "dak-data-utils.hpp"
#include "daktronics-filter.hpp"
#include "dak-serial.hpp"

#include <string>
#include <vector>
#include <QThread>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("daktronics-realtime-data", "en-US")

obs_source_info create_daktronics_filter_info()
{
	obs_source_info daktronics_filter_info = {};
	daktronics_filter_info.id = "daktronics_filter";
	daktronics_filter_info.type = OBS_SOURCE_TYPE_FILTER;
	daktronics_filter_info.output_flags = OBS_SOURCE_VIDEO;
	daktronics_filter_info.get_name = DAKFilter::GetName;
	daktronics_filter_info.create = DAKFilter::Create;
	daktronics_filter_info.destroy = DAKFilter::Destroy;
	daktronics_filter_info.video_render = DAKFilter::Render;
	daktronics_filter_info.video_tick = DAKFilter::video_tick;
	daktronics_filter_info.get_defaults = DAKFilter::GetDefaults;
	daktronics_filter_info.get_properties = DAKFilter::GetProperties;
	daktronics_filter_info.update = DAKFilter::Update;
	daktronics_filter_info.icon_type = OBS_ICON_TYPE_IMAGE;

	return daktronics_filter_info;
}

bool obs_module_load(void)
{
	DAKDataUtils::sync_init();
	DAKDataUtils::populateSportsData();

	obs_source_info dakFilter = create_daktronics_filter_info();
	obs_register_source(&dakFilter);

    obs_frontend_add_event_callback([](obs_frontend_event event, void* private_data) {
        if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
            setup_menu();
        }
    }, nullptr);

	obs_log(LOG_INFO, "Daktronics plugin loaded successfully (version %s)", PLUGIN_VERSION);

	return true;
}

void obs_module_unload(void)
{
	DAKDataUtils::sync_destroy();
	DAKDataUtils::clearSportsData();

	obs_log(LOG_INFO, "Daktronics plugin unloaded");
}





// ******************************************************

//============================== Do menu updates and stuff =============================//

/**
 * @brief Called on plugin load to create the "Tools -> Serial Port Selector" menu.
 */
static void setup_menu() {
    // Add the main "Serial Port Selector" item to the Tools menu.
    // When clicked, it will call refresh_ports_callback which dynamically generates the submenu.
    obs_frontend_add_tools_menu_item(
        "Serial Port Selector", // Menu item text
        refresh_ports_callback,
        nullptr, // No custom data needed for root callback
        "obs_serial_data_selector_root" // Unique ID for the root item
    );

    // Call it once to initialize the submenu with the list of ports
    refresh_ports_callback(nullptr, nullptr);
}

/**
 * @brief Menu item callback to refresh the list of ports.
 * This is the main callback for the "Serial Port Selector" menu item.
 */
static void refresh_ports_callback(void *data, obs_frontend_source_t *source) {
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(source);

    obs_frontend_menu_t menu = obs_frontend_get_tools_menu();
    if (!menu) return;

    // Remove old submenu items and then add the new ones
    obs_frontend_menu_remove_all_sub_items(menu, "obs_serial_data_selector_root");

    std::vector<std::string> ports = SerialPort::listPorts();
	std::string currentPort = DAKDataUtils::getSerialPort();

    // 1. Add 'Stop Listening' option
    // Passing an empty string "" as data signals to StartSerialListener to stop.
    obs_frontend_menu_add_item(menu, "obs_serial_data_selector_root",
        "Stop Listening", port_selected_callback, (void*)"",
        "obs_serial_data_selector_stop");

    // Check 'Stop Listening' if no port is currently active
    obs_frontend_set_tools_menu_item_checked("obs_serial_data_selector_stop", currentPort.empty());

    obs_frontend_menu_add_separator(menu, "obs_serial_data_selector_root");

    // 2. Add available ports
    for (const auto& port : ports) {
        // Use the full port path/name as both the display text and the menu item ID
        obs_frontend_menu_add_item(menu, "obs_serial_data_selector_root",
            port.c_str(), port_selected_callback, (void*)port.c_str(), 
            port.c_str()); 

        // Check if this is the currently selected port
        if (port == currentPort && !currentPort.empty()) {
            obs_frontend_set_tools_menu_item_checked(port.c_str(), true);
        } else {
            obs_frontend_set_tools_menu_item_checked(port.c_str(), false);
        }
    }

    // 3. If no ports are found, add a placeholder
    if (ports.empty()) {
        obs_frontend_menu_add_item(menu, "obs_serial_data_selector_root",
            "No Serial Ports Found", nullptr, nullptr, 
            "obs_serial_data_selector_no_ports");
    }
}

/**
 * @brief Menu item callback when a specific port is clicked.
 */
static void port_selected_callback(void *data, obs_frontend_source_t *source) {
	UNUSED_PARAMETER(source);

    const char *port_name = (const char *)data;
    StartSerialListener(port_name);
}


/**
 * @brief Manages the lifecycle of the serial listener thread based on menu selection.
 */
static void StartSerialListener(const std::string& port) {

    // Update menu item checkmarks for visual feedback
    obs_frontend_menu_t menu = obs_frontend_get_tools_menu();
    if (menu) {
        // Update the 'Stop Listening' checkmark
        obs_frontend_set_tools_menu_item_checked(
            "obs_serial_data_selector_stop", port.empty());
    }

	DAKDataUtils::startSerial(port);
}
