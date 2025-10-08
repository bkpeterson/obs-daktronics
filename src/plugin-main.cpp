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

obs_source_info daktronics_source_info = {
    .id = "daktronics_source",
    .type = OBS_SOURCE_TYPE_INPUT,
    .output_flags = OBS_SOURCE_VIDEO,
    .get_name = DAKSource::GetName,
    .create = DAKSource::Create,
    .destroy = DAKSource::Destroy,
    .get_width = DAKSource::GetWidth,
    .get_height = DAKSource::GetHeight,
    .video_render = DAKSource::Render,
    .get_defaults = DAKSource::GetDefaults,
    .get_properties = DAKSource::GetProperties,
    .update = DAKSource::Update,
    .icon_type = OBS_ICON_TYPE_TEXT,
};

obs_source_info daktronics_filter_info = {
    .id = "daktronics_filter",
    .type = OBS_SOURCE_TYPE_FILTER,
    .output_flags = OBS_SOURCE_VIDEO,
    .get_name = DAKFilter::GetName,
    .create = DAKFilter::Create,
    .destroy = DAKFilter::Destroy,
    .video_render = DAKFilter::Render,
    .get_defaults = DAKFilter::GetDefaults,
    .get_properties = DAKFilter::GetProperties,
    .update = DAKFilter::Update,
    .icon_type = OBS_ICON_TYPE_IMAGE,
};



bool obs_module_load(void)
{
	obs_register_source(&daktronics_source_info);
	obs_register_source(&daktronics_filter_info);

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
