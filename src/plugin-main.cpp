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
#include <dak-source-support.h>
#include "dak-data-utils.hpp"
#include "daktronics-source.hpp"
#include "daktronics-filter.hpp"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("daktronics-realtime-data", "en-US")


bool obs_module_load(void)
{
	obs_register_source(DAKSource::create_daktronics_source_info());
	obs_register_source(DAKFilter::create_daktronics_filter_info());

	DAKDataUtils::populateSportsData();

	/**********************
	 * Start serial port
	 */

	obs_log(LOG_INFO, "Daktronics plugin loaded successfully (version %s)", PLUGIN_VERSION);

	return true;
}

void obs_module_unload(void)
{
	obs_log(LOG_INFO, "Daktronics plugin unloaded");
}
