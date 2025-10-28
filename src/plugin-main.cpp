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

#include <dak-source-support.h>
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

struct plugin_global_data {
    QThread workerThread;
    SerialPortWorker *worker = nullptr;

    std::string current_port;

    plugin_global_data();
    void cleanup();
};

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
	daktronics_filter_info.get_defaults = DAKFilter::GetDefaults;
	daktronics_filter_info.get_properties = DAKFilter::GetProperties;
	daktronics_filter_info.update = DAKFilter::Update;
	daktronics_filter_info.icon_type = OBS_ICON_TYPE_IMAGE;

	return daktronics_filter_info;
}

bool obs_module_load(void)
{
	obs_source_info dakFilter = create_daktronics_filter_info();
	obs_register_source(&dakFilter);

	DAKDataUtils::populateSportsData();

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
	DAKDataUtils::clearSportsData();
	g_plugin_data.cleanup();

	obs_log(LOG_INFO, "Daktronics plugin unloaded");
}

// ====================================================================
// Global Data Structure Implementation
// ====================================================================

// Global state to manage the worker thread and the latest data
plugin_global_data::plugin_global_data() {
    worker = new SerialPortWorker();
    // Move the worker object to the new thread to ensure signal/slot safety
    worker->moveToThread(&workerThread);
    
    // Thread startup connections
    QObject::connect(&workerThread, &QThread::started, worker, &SerialPortWorker::connectPort);
    QObject::connect(&workerThread, &QThread::finished, worker, &SerialPortWorker::deleteLater);
    
    // Connect the worker's data signal to the thread-safe handler in the main OBS thread
    QObject::connect(worker, &SerialPortWorker::dataReceived, [](const QString& data) {
        // This lambda executes in the main (OBS/Qt) thread
        // This is the implementation of the SetValue concept.
        std::string s_data = data.toStdString();
        
		/*************** CALL SETVALUE *********************** */
		/**													   */
		/***************************************************** */

    });
}

// Call this before destroying the global object
void plugin_global_data::cleanup() {
    if (workerThread.isRunning()) {
        worker->disconnectPort(); // Tell the worker to close the port first
        workerThread.quit();
        workerThread.wait();
    }
    // Worker is automatically deleted via deleteLater slot when thread finishes
}


// Global instance of the plugin's data
static plugin_global_data g_plugin_data;


/**
 * @brief Manages the lifecycle of the serial listener thread based on menu selection.
 */
static void StartSerialListener(const std::string& port) {
    // 1. Stop current listener/thread
    if (g_plugin_data.workerThread.isRunning()) {
        g_plugin_data.worker->disconnectPort();
        g_plugin_data.workerThread.quit();
        g_plugin_data.workerThread.wait();
    }
    g_plugin_data.current_port = port;
    
    if (!port.empty()) {
        // 2. Configure worker and start thread for the new port
        g_plugin_data.worker->portName = QString::fromStdString(port);
        g_plugin_data.workerThread.start();
        blog(LOG_INFO, "Serial Port Selected: %s. QSerialPort Worker starting.", port.c_str());
    } else {
        blog(LOG_INFO, "Serial Port Deselected. QSerialPort Worker stopped.");
    }

    // Update menu item checkmarks for visual feedback
    obs_frontend_menu_t menu = obs_frontend_get_tools_menu();
    if (menu) {
        // Update the 'Stop Listening' checkmark
        obs_frontend_set_tools_menu_item_checked(
            obs_module_text("MenuId_Stop"), port.empty());
    }
}

// ====================================================================
// OBS Frontend (Qt Menu) Implementation
// ====================================================================

/**
 * @brief Uses QSerialPortInfo to find available ports.
 */
static std::vector<std::string> GetAvailablePorts() {
    std::vector<std::string> ports;
    const auto availablePorts = QSerialPortInfo::availablePorts();
    for (const auto &info : availablePorts) {
        ports.push_back(info.portName().toStdString());
    }
    return ports;
}


/**
 * @brief Menu item callback when a specific port is clicked.
 */
static void port_selected_callback(void *data, obs_frontend_source_t *source) {
    const char *port_name = (const char *)data;
    StartSerialListener(port_name);
}

/**
 * @brief Menu item callback to refresh the list of ports.
 * This is the main callback for the "Serial Port Selector" menu item.
 */
static void refresh_ports_callback(void *data, obs_frontend_source_t *source) {
    obs_frontend_menu_t menu = obs_frontend_get_tools_menu();
    if (!menu) return;

    // Remove old submenu items and then add the new ones
    obs_frontend_menu_remove_all_sub_items(menu, obs_module_text("MenuId_Root"));

    std::vector<std::string> ports = GetAvailablePorts();

    // 1. Add 'Stop Listening' option
    // Passing an empty string "" as data signals to StartSerialListener to stop.
    obs_frontend_menu_add_item(menu, obs_module_text("MenuId_Root"),
        obs_module_text("StopListening"), port_selected_callback, (void*)"",
        obs_module_text("MenuId_Stop"));

    // Check 'Stop Listening' if no port is currently active
    obs_frontend_set_tools_menu_item_checked(obs_module_text("MenuId_Stop"), g_plugin_data.current_port.empty());

    obs_frontend_menu_add_separator(menu, obs_module_text("MenuId_Root"));

    // 2. Add available ports
    for (const auto& port : ports) {
        // Use the full port path/name as both the display text and the menu item ID
        obs_frontend_menu_add_item(menu, obs_module_text("MenuId_Root"),
            port.c_str(), port_selected_callback, (void*)port.c_str(), 
            port.c_str()); 

        // Check if this is the currently selected port
        if (port == g_plugin_data.current_port && !g_plugin_data.current_port.empty()) {
            obs_frontend_set_tools_menu_item_checked(port.c_str(), true);
        } else {
            obs_frontend_set_tools_menu_item_checked(port.c_str(), false);
        }
    }

    // 3. If no ports are found, add a placeholder
    if (ports.empty()) {
        obs_frontend_menu_add_item(menu, obs_module_text("MenuId_Root"),
            obs_module_text("NoPortsFound"), nullptr, nullptr, 
            obs_module_text("MenuId_NoPorts"));
    }
}

/**
 * @brief Called on plugin load to create the "Tools -> Serial Port Selector" menu.
 */
static void setup_menu() {
    // Add the main "Serial Port Selector" item to the Tools menu.
    // When clicked, it will call refresh_ports_callback which dynamically generates the submenu.
    obs_frontend_add_tools_menu_item(
        obs_module_text("SerialPortSelector"), // Menu item text
        refresh_ports_callback,
        nullptr, // No custom data needed for root callback
        obs_module_text("MenuId_Root") // Unique ID for the root item
    );

    // Call it once to initialize the submenu with the list of ports
    refresh_ports_callback(nullptr, nullptr);
}



const char *obs_module_text(const char *key) {
    // Localization placeholder
    if (strcmp(key, "SerialDataSource") == 0) return "Serial Data Source (Menu Controlled)";
    if (strcmp(key, "SerialPortSelector") == 0) return "Serial Port Selector";
    if (strcmp(key, "StopListening") == 0) return "Stop Listening";
    if (strcmp(key, "NoPortsFound") == 0) return "No Serial Ports Found";
    if (strcmp(key, "SourceInfo") == 0) return "This source gets its data from a serial port selected via the 'Tools -> Serial Port Selector' menu. Uses QSerialPort in a separate thread.";
    if (strcmp(key, "MenuId_Root") == 0) return "obs_serial_data_selector_root";
    if (strcmp(key, "MenuId_Stop") == 0) return "obs_serial_data_selector_stop";
    if (strcmp(key, "MenuId_NoPorts") == 0) return "obs_serial_data_selector_no_ports";
    return key;
}