#pragma once

#include <string>

#include <obs-module.h>
#include <dak-source-support.h>

class DAKFilter {
public:
	DAKFilter(obs_data_t *settings, obs_source_t *obs_source);
	~DAKFilter();

	void SetVisible(bool visible);
	uint32_t GetIndex();
	void Update(obs_data_t *settings);

	static void *Create(obs_data_t *settings, obs_source_t *obs_source);
	static void Destroy(void *data);
	static void Update(void *data, obs_data_t *settings);
	static const char *GetName(void *);
	static void Render(void *data, gs_effect_t *effect);
	static obs_properties_t *GetProperties(void *data);
	static void GetDefaults(obs_data_t *settings);

	void _DoRender();
	static bool DAKSportChanged(obs_properties_t *props, obs_property_t *property, obs_data_t *settings);

private:
	obs_source_t *_source;

	std::string _sport;
	uint32_t _index;
	bool _visible;
};
