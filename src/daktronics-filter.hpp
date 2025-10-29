#pragma once

#include <string>

#include <obs-module.h>
#include <dak-source-support.h>

class DAKFilter {
public:
	DAKFilter(obs_data_t *settings, obs_source_t *obs_source);
	~DAKFilter();

	static const uint32_t DAK_VISIBLE = 1;
	static const uint32_t DAK_TEXT = 2;
	static const uint32_t DAK_COLOR = 3;

	void SetValue(std::string value);

	uint32_t GetFilterType();
	uint32_t GetIndex();
	void Update(obs_data_t *settings);

	static void *Create(obs_data_t *settings, obs_source_t *obs_source);
	static void Destroy(void *data);
	static void Update(void *data, obs_data_t *settings);
	static const char *GetName(void *);
	static void Render(void *data, gs_effect_t *effect);
	static void video_tick(void *data, float seconds);
	static obs_properties_t *GetProperties(void *data);
	static void GetDefaults(obs_data_t *settings);

	void populateParams(obs_property_t *list, obs_property_type paramType);
	void doColorProps(obs_properties_t *props, std::string paramName);

	static bool DAKSportChanged(obs_properties_t *props, obs_property_t *property, obs_data_t *settings);
	static bool DAKFilterChanged(void *data, obs_properties_t *props, obs_property_t *property,
				     obs_data_t *settings);
	static bool DAKParamChanged(void *data, obs_properties_t *props, obs_property_t *property,
				    obs_data_t *settings);

private:
	obs_source_t *_source;

	std::string _sport;
	uint32_t _index;
	std::string _internalValue;
	uint32_t _filterType;
	std::string _paramName;
	uint32_t _paramType;
	int _color;
	int _colorAlpha;

	void _render();
};
