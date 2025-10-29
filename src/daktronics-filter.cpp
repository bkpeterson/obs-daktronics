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
	_filterType = DAKFilter::DAK_TEXT;
	_internalValue = "";
	Update(this, settings);
}

DAKFilter::~DAKFilter()
{
	// Nothing to do
}

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
}

const char *DAKFilter::GetName(void *type_data)
{
	UNUSED_PARAMETER(type_data);
	return "Daktronics Scoreboard Filter";
}

void DAKFilter::SetValue(std::string newValue)
{
	_internalValue = newValue;

	if (!_source)
		return;

	obs_source_t *targetSource = obs_filter_get_parent(_source);
	obs_data_t *sourceData = obs_source_get_settings(targetSource);

	switch (_filterType) {
	case DAKFilter::DAK_VISIBLE:
		obs_source_set_enabled(_source, _internalValue.length() > 0);
		break;

	case DAKFilter::DAK_TEXT:
		obs_data_set_string(sourceData, _paramName.c_str(), _internalValue.c_str());
		obs_source_update(targetSource, sourceData);
		break;

	case DAKFilter::DAK_COLOR:
		if (_paramType == OBS_PROPERTY_COLOR) {
			obs_data_set_int(sourceData, _paramName.c_str(), _color);
			obs_source_update(targetSource, sourceData);
		} else if (_paramType == OBS_PROPERTY_COLOR_ALPHA) {
			obs_data_set_int(sourceData, _paramName.c_str(), _colorAlpha);
			obs_source_update(targetSource, sourceData);
		}

		break;
	}

	obs_data_release(sourceData);
}

uint32_t DAKFilter::GetIndex()
{
	return _index;
}

void DAKFilter::Render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	auto instance = static_cast<DAKFilter *>(data);
	instance->_render();
}

void DAKFilter::_render()
{
	obs_source_skip_video_filter(_source);
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
	_filterType = (uint32_t)obs_data_get_int(settings, "dak_filter_list");
	_paramName = (std::string)obs_data_get_string(settings, "dak_param_list");

	obs_source_t *targetSource = obs_filter_get_parent(_source);
	obs_properties_t *sourceProps = obs_source_properties(targetSource);
	obs_property_t *targetProp = obs_properties_get(sourceProps, _paramName.c_str());

	_paramType = (uint32_t)obs_property_get_type(targetProp);
	_color = (int)obs_data_get_int(settings, "dak_color");
	_colorAlpha = (int)obs_data_get_int(settings, "dak_color_alpha");

	obs_properties_destroy(sourceProps);
}

void DAKFilter::GetDefaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, "dak_sport_type", "Basketball");
	obs_data_set_default_int(settings, "dak_field_list", 1);
	obs_data_set_default_int(settings, "dak_filter_list", DAKFilter::DAK_TEXT);
}

obs_properties_t *DAKFilter::GetProperties(void *data)
{
	obs_properties_t *props = obs_properties_create();

	obs_property_t *sport_type =
		obs_properties_add_list(props, "dak_sport_type", "Sport", OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	DAKDataUtils::PopulateSportProps(sport_type);

	obs_property_set_modified_callback(sport_type, DAKFilter::DAKSportChanged);

	obs_properties_add_list(props, "dak_field_list", "Scoreboard Data Field", OBS_COMBO_TYPE_LIST,
				OBS_COMBO_FORMAT_INT);

	obs_property_t *filter_type = obs_properties_add_list(props, "dak_filter_list", "Filter Type",
							      OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
	obs_property_list_add_int(filter_type, "Show/Hide", DAKFilter::DAK_VISIBLE);
	obs_property_list_add_int(filter_type, "Update Text", DAKFilter::DAK_TEXT);
	obs_property_list_add_int(filter_type, "Change Color", DAKFilter::DAK_COLOR);

	obs_property_set_modified_callback2(filter_type, DAKFilter::DAKFilterChanged, data);

	obs_property_t *param_type = obs_properties_add_list(props, "dak_param_list", "Property to Modify",
							     OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	obs_property_set_modified_callback2(param_type, DAKFilter::DAKParamChanged, data);

	obs_properties_add_color(props, "dak_color", "Color when data field is blank");
	obs_properties_add_color_alpha(props, "dak_color_alpha", "Color when data field is blank");

	std::string info2 =
		"<a href=\"https://github.com/bkpeterson/obs-daktronics\">Daktronics Source</a> (1.0) by bkpeterson";
	obs_properties_add_text(props, "plugin_info2", info2.c_str(), OBS_TEXT_INFO);

	return props;
}

void DAKFilter::populateParams(obs_property_t *list, obs_property_type paramType)
{
	obs_source_t *targetSource = obs_filter_get_parent(_source);
	obs_properties_t *sourceProps = obs_source_properties(targetSource);

	for (obs_property_t *prop = obs_properties_first(sourceProps); prop != NULL; obs_property_next(&prop)) {
		const char *prop_name = obs_property_name(prop);
		if (obs_property_get_type(prop) == paramType)
			obs_property_list_add_string(list, prop_name, prop_name);
	}

	obs_properties_destroy(sourceProps);
}

void DAKFilter::doColorProps(obs_properties_t *props, std::string paramName)
{
	obs_source_t *targetSource = obs_filter_get_parent(_source);
	obs_properties_t *sourceProps = obs_source_properties(targetSource);

	obs_property_t *targetProp = obs_properties_get(sourceProps, paramName.c_str());
	obs_property_t *color = obs_properties_get(props, "dak_color");
	obs_property_t *color_alpha = obs_properties_get(props, "dak_color_alpha");

	switch (obs_property_get_type(targetProp)) {
	case OBS_PROPERTY_COLOR:
		obs_property_set_visible(color, true);
		obs_property_set_visible(color_alpha, false);
		break;

	case OBS_PROPERTY_COLOR_ALPHA:
		obs_property_set_visible(color, false);
		obs_property_set_visible(color_alpha, true);
		break;

	default:
		obs_property_set_visible(color, false);
		obs_property_set_visible(color_alpha, false);
	}

	obs_properties_destroy(sourceProps);
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

bool DAKFilter::DAKFilterChanged(void *data, obs_properties_t *props, obs_property_t *property, obs_data_t *settings)
{
	UNUSED_PARAMETER(property);
	DAKFilter *instance = (DAKFilter *)data;

	uint32_t filter_type = (uint32_t)obs_data_get_int(settings, "dak_filter_list");
	obs_property_t *list = obs_properties_get(props, "dak_param_list");
	obs_property_t *color = obs_properties_get(props, "dak_color");
	obs_property_t *color_alpha = obs_properties_get(props, "dak_color_alpha");

	switch (filter_type) {
	case DAKFilter::DAK_VISIBLE:
		obs_property_set_visible(list, false);
		obs_property_set_visible(color, false);
		obs_property_set_visible(color_alpha, false);
		break;

	case DAKFilter::DAK_TEXT:
		obs_property_list_clear(list);
		instance->populateParams(list, OBS_PROPERTY_TEXT);
		obs_property_set_visible(list, true);
		obs_property_set_visible(color, false);
		obs_property_set_visible(color_alpha, false);
		break;

	case DAKFilter::DAK_COLOR:
		obs_property_list_clear(list);
		instance->populateParams(list, OBS_PROPERTY_COLOR);
		instance->populateParams(list, OBS_PROPERTY_COLOR_ALPHA);
		obs_property_set_visible(list, true);
		DAKFilter::DAKParamChanged(data, props, list, settings);
		break;
	}

	return true;
}

bool DAKFilter::DAKParamChanged(void *data, obs_properties_t *props, obs_property_t *property, obs_data_t *settings)
{
	UNUSED_PARAMETER(property);
	DAKFilter *instance = (DAKFilter *)data;

	std::string paramName = obs_data_get_string(settings, "dak_param_list");

	instance->doColorProps(props, paramName);

	return true;
}
