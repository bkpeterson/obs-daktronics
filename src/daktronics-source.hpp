#pragma once

#include <string>

#include <obs-module.h>
#include <dak-source-support.h>
#include <pango/pangocairo.h>

#define FONT_MAXLEN 64
#define TEXT_MAXLEN 64

class DAKSource {
public:
	DAKSource(obs_data_t *settings, obs_source_t *obs_source);
	~DAKSource();

	void SetTextValue(std::string newValue);
	uint32_t GetIndex();
	void Update(obs_data_t *settings);

	static void *Create(obs_data_t *settings, obs_source_t *obs_source);
	static void Destroy(void *data);
	static void Update(void *data, obs_data_t *settings);
	static const char *GetName(void *);
	static void Render(void *data, gs_effect_t *effect);
	static uint32_t GetWidth(void *data);
	static uint32_t GetHeight(void *data);
	static obs_properties_t *GetProperties(void *data);
	static void GetDefaults(obs_data_t *settings);

	void _DoRender();
	static bool DAKSportChanged(obs_properties_t *props, obs_property_t *property, obs_data_t *settings);

	static obs_source_info *create_daktronics_source_info();

private:
	obs_source_t *_source;

	uint32_t _width;
	uint32_t _height;

	std::string _sport;
	uint32_t _index;
	const char *_textValue;

	gs_texture_t *_texture;

	PangoAlignment _align;
	struct vec4 _bg;
	struct vec4 _fg;
	struct vec4 _outline;

	double _outlinew;
	char _font[FONT_MAXLEN + 1];

	static obs_source_info daktronics_source_info;
};
