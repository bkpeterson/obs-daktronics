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

obs_source_info *create_daktronics_source_info()
{
	obs_source_info *daktronics_source_info = {};
	daktronics_source_info->id = "daktronics_source";
	daktronics_source_info->type = OBS_SOURCE_TYPE_INPUT;
	daktronics_source_info->output_flags = OBS_SOURCE_VIDEO;
	daktronics_source_info->get_name = DAKSource::GetName;
	daktronics_source_info->create = DAKSource::Create;
	daktronics_source_info->destroy = DAKSource::Destroy;
	daktronics_source_info->get_width = DAKSource::GetWidth;
	daktronics_source_info->get_height = DAKSource::GetHeight;
	daktronics_source_info->video_render = DAKSource::Render;
	daktronics_source_info->get_defaults = DAKSource::GetDefaults;
	daktronics_source_info->get_properties = DAKSource::GetProperties;
	daktronics_source_info->update = DAKSource::Update;
	daktronics_source_info->icon_type = OBS_ICON_TYPE_TEXT;

	return daktronics_source_info;
}

obs_source_info *create_daktronics_filter_info()
{
	obs_source_info *daktronics_filter_info = {};
	daktronics_filter_info->id = "daktronics_filter";
	daktronics_filter_info->type = OBS_SOURCE_TYPE_FILTER;
	daktronics_filter_info->output_flags = OBS_SOURCE_VIDEO;
	daktronics_filter_info->get_name = DAKFilter::GetName;
	daktronics_filter_info->create = DAKFilter::Create;
	daktronics_filter_info->destroy = DAKFilter::Destroy;
	daktronics_filter_info->video_render = DAKFilter::Render;
	daktronics_filter_info->get_defaults = DAKFilter::GetDefaults;
	daktronics_filter_info->get_properties = DAKFilter::GetProperties;
	daktronics_filter_info->update = DAKFilter::Update;
	daktronics_filter_info->icon_type = OBS_ICON_TYPE_IMAGE;

	return daktronics_filter_info;
}

bool obs_module_load(void)
{
	obs_register_source(create_daktronics_source_info());
	obs_register_source(create_daktronics_filter_info());

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
