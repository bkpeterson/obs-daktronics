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

#include <string>

#include "dak-sport-data.hpp"
#include "daktronics-filter.hpp"
#include "dak-data-utils.hpp"

DAKFilter::DAKFilter(obs_data_t *settings, obs_source_t *source) : _source(source)
{
	_visible = true;
	Update(this, settings);
}

DAKFilter::~DAKFilter() {}

void *DAKFilter::Create(obs_data_t *settings, obs_source_t *source)
{
	auto context = new DAKFilter(settings, source);
	DAKDataUtils::AddFilter(context);
	return context;
}

void DAKFilter::Destroy(void *data)
{
	auto instance = static_cast<DAKFilter *>(data);
	DAKDataUtils::RemoveFilter(instance);
	delete instance;
}

const char *DAKFilter::GetName(void *type_data)
{
	UNUSED_PARAMETER(type_data);
	return obs_module_text("DaktronicsFilter");
}

void DAKFilter::SetVisible(bool visible)
{
	_visible = visible;
}

uint32_t DAKFilter::GetIndex()
{
	return _index;
}

void DAKFilter::Render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	auto instance = static_cast<DAKFilter *>(data);
	instance->_DoRender();
}

void DAKFilter::_DoRender()
{
	// Get the source being filtered
	if (!_source)
		return;
	obs_source_set_enabled(_source, _visible);
}

void DAKFilter::Update(void *data, obs_data_t *settings)
{
	auto &instance = *static_cast<DAKFilter *>(data);
	instance.Update(settings);
}

void DAKFilter::Update(obs_data_t *settings)
{
	_sport = (std::string)obs_data_get_string(settings, "dak_sport_type");
	_index = (uint32_t)obs_data_get_int(settings, "dak_field_list");
}

void DAKFilter::GetDefaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, "dak_sport_type", "Basketball");
	obs_data_set_default_int(settings, "dak_field_list", 1);
}

obs_properties_t *DAKFilter::GetProperties(void *data)
{
	UNUSED_PARAMETER(data);

	obs_properties_t *props = obs_properties_create();

	obs_property_t *sport_type = obs_properties_add_list(props, "dak_sport_type",
							     obs_module_text("DaktronicsSource.SportType"),
							     OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	DAKDataUtils::PopulateSportProps(sport_type);

	obs_property_set_modified_callback(sport_type, DAKFilter::DAKSportChanged);

	obs_properties_add_list(props, "dak_field_list", obs_module_text("DaktronicsSource.FieldList"),
				OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	std::string info = "<a href=\"https://github.com/bkpeterson/obs-daktronics\">Daktronics Source</a> (";
	info += PLUGIN_VERSION;
	info += ") by bkpeterson";

	obs_properties_add_text(props, "plugin_info", info.c_str(), OBS_TEXT_INFO);

	return props;
}

bool DAKFilter::DAKSportChanged(obs_properties_t *props, obs_property_t *property, obs_data_t *settings)
{
	UNUSED_PARAMETER(property);

	std::string sport_type = (std::string)obs_data_get_string(settings, "dak_sport_type");

	obs_property_t *list = obs_properties_get(props, "dak_field_list");
	obs_property_list_clear(list);

	DAKSportData *sportData = DAKDataUtils::getSportData(sport_type);
	sportData->PopulateFieldProps(list);

	return true;
}
