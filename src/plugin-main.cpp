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
#include "dak-widget.hpp"

#include <string>
#include <vector>
#include <QMainWindow>

DAKDock *dock = nullptr;

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

	obs_frontend_add_event_callback(
		[](obs_frontend_event event, void *private_data) {
			UNUSED_PARAMETER(private_data);
			if (event == OBS_FRONTEND_EVENT_FINISHED_LOADING) {
				const auto main_window = static_cast<QMainWindow *>(obs_frontend_get_main_window());
				dock = new DAKDock(main_window);
				obs_frontend_add_dock_by_id("dak_serial_dock", "Daktronics Serial Reader", dock);
				DAKLogger::instance().emit logMessage("Plugin loaded!");
			}
		},
		nullptr);

	obs_add_main_render_callback(DAKDataUtils::execute_global_tick_logic, NULL);

	obs_log(LOG_INFO, "Daktronics plugin loaded successfully (version %s)", PLUGIN_VERSION);

	return true;
}

void obs_module_unload(void)
{
	obs_remove_main_render_callback(DAKDataUtils::execute_global_tick_logic, NULL);

	DAKDataUtils::sync_destroy();
	DAKDataUtils::clearSportsData();

	dock->deleteLater();

	obs_log(LOG_INFO, "Daktronics plugin unloaded");
}
