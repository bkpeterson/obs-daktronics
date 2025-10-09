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
#include <graphics/vec4.h>

#include "dak-sport-data.hpp"
#include "daktronics-source.hpp"
#include "dak-data-utils.hpp"
#include <pango/pangocairo.h>

DAKSource::DAKSource(obs_data_t *settings, obs_source_t *source) : _source(source)
{
	Update(this, settings);
}

DAKSource::~DAKSource()
{
	if (_texture) {
		obs_enter_graphics();
		gs_texture_destroy(_texture);
		obs_leave_graphics();
	}
}

void *DAKSource::Create(obs_data_t *settings, obs_source_t *source)
{
	auto context = new DAKSource(settings, source);
	DAKDataUtils::AddSource(context);
	return context;
}

void DAKSource::Destroy(void *data)
{
	auto instance = static_cast<DAKSource *>(data);
	DAKDataUtils::RemoveSource(instance);
	delete instance;
}

const char *DAKSource::GetName(void *type_data)
{
	UNUSED_PARAMETER(type_data);
	return obs_module_text("DaktronicsSource");
}

void DAKSource::SetTextValue(std::string newValue)
{
	_textValue = newValue.c_str();
}

uint32_t DAKSource::GetIndex()
{
	return _index;
}

void DAKSource::_DoRender()
{
	/* Layout */
	PangoContext *pango = pango_font_map_create_context(pango_cairo_font_map_get_default());
	PangoLayout *layout = pango_layout_new(pango);
	PangoFontDescription *desc = pango_font_description_from_string(_font);

	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);
	pango_layout_set_text(layout, _textValue, TEXT_MAXLEN);

	int width, height;
	pango_layout_get_pixel_size(layout, &width, &height);
	assert(width >= 0 && height >= 0);
	_width = width;
	_height = height;

	pango_layout_set_alignment(layout, _align);

	/* Render */
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, _width, _height);
	cairo_t *cr = cairo_create(surface);
	cairo_set_source_rgba(cr, _bg.x, _bg.y, _bg.z, _bg.w);
	cairo_paint(cr);
	cairo_set_source_rgba(cr, _outline.x, _outline.y, _outline.z, _outline.w);
	cairo_set_line_width(cr, _outlinew);
	pango_cairo_layout_path(cr, layout);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr, _fg.x, _fg.y, _fg.z, _fg.w);
	pango_cairo_show_layout(cr, layout);

	/* OBS texture */
	cairo_surface_flush(surface);
	unsigned char *textdata = cairo_image_surface_get_data(surface);
	obs_enter_graphics();
	if (_texture) {
		gs_texture_destroy(_texture);
	}
	_texture = gs_texture_create(_width, _height, GS_BGRA, 1, (const uint8_t **)&textdata, 0);
	obs_leave_graphics();

	/* Cleanup */
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	g_object_unref(layout);
	g_object_unref(pango);
}

uint32_t DAKSource::GetWidth(void *data)
{
	auto &instance = *static_cast<DAKSource *>(data);
	return instance._width;
}

uint32_t DAKSource::GetHeight(void *data)
{
	auto &instance = *static_cast<DAKSource *>(data);
	return instance._height;
}

void DAKSource::Render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	auto &instance = *static_cast<DAKSource *>(data);

	instance._DoRender();
	obs_source_draw(instance._texture, 0, 0, instance._width, instance._height, false);
}

void DAKSource::Update(void *data, obs_data_t *settings)
{
	auto &instance = *static_cast<DAKSource *>(data);
	instance.Update(settings);
}

void DAKSource::Update(obs_data_t *settings)
{
	SetTextValue(obs_data_get_string(settings, "dak_field_default_val"));

	_sport = (std::string)obs_data_get_string(settings, "dak_sport_type");
	_index = (uint32_t)obs_data_get_int(settings, "dak_field_list");

	obs_data_t *font_obj = obs_data_get_obj(settings, "font");
	snprintf(_font, FONT_MAXLEN, "%s %lld", obs_data_get_string(font_obj, "face"),
		 obs_data_get_int(font_obj, "size"));
	obs_data_release(font_obj);

	int align = obs_data_get_int(settings, "dak_text_align");
	switch (align) {
	case 1:
		_align = PANGO_ALIGN_LEFT;
		break;
	case 2:
		_align = PANGO_ALIGN_CENTER;
		break;
	case 3:
		_align = PANGO_ALIGN_RIGHT;
		break;
	}

	vec4_from_rgba(&_bg, obs_data_get_int(settings, "bg"));
	vec4_from_rgba(&_fg, obs_data_get_int(settings, "fg"));
	vec4_from_rgba(&_outline, obs_data_get_int(settings, "outline"));
	_outlinew = obs_data_get_double(settings, "outlinew");

	_DoRender();
}

void DAKSource::GetDefaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, "dak_sport_type", "Basketball");
	obs_data_set_default_int(settings, "dak_field_list", 1);
	obs_data_set_default_string(settings, "dak_field_default_val", "");
	obs_data_t *font_obj = obs_data_create();
	obs_data_set_default_string(font_obj, "face", "Sans Serif");
	obs_data_set_default_int(font_obj, "size", 72);
	obs_data_set_default_obj(settings, "font", font_obj);
	obs_data_release(font_obj);
	obs_data_set_default_int(settings, "dak_text_align", 2);
	obs_data_set_default_int(settings, "bg", 0x00000000);
	obs_data_set_default_int(settings, "fg", 0xFFFFFFFF);
	obs_data_set_default_int(settings, "outline", 0xFF000000);
	obs_data_set_default_double(settings, "outlinew", 10.0);
}

obs_properties_t *DAKSource::GetProperties(void *data)
{
	UNUSED_PARAMETER(data);

	obs_properties_t *props = obs_properties_create();
	obs_properties_t *prop_dak_group = obs_properties_create();

	obs_property_t *sport_type = obs_properties_add_list(prop_dak_group, "dak_sport_type",
							     obs_module_text("DaktronicsSource.SportType"),
							     OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	DAKDataUtils::PopulateSportProps(sport_type);

	obs_property_set_modified_callback(sport_type, DAKSource::DAKSportChanged);

	obs_properties_add_list(prop_dak_group, "dak_field_list", obs_module_text("DaktronicsSource.FieldList"),
				OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_properties_add_text(prop_dak_group, "dak_field_default_val",
				obs_module_text("DaktronicsSource.FieldDefaultVal"), OBS_TEXT_DEFAULT);

	obs_properties_add_group(props, "sport_group", obs_module_text("DaktronicsSource.SportGroupLabel"),
				 OBS_GROUP_NORMAL, prop_dak_group);

	obs_properties_add_font(props, "font", obs_module_text("DaktronicsSource.Font"));

	obs_property_t *align_type = obs_properties_add_list(props, "dak_text_align",
							     obs_module_text("DaktronicsSource.AlignList"),
							     OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);

	obs_property_list_add_int(align_type, "Left", 1);

	obs_property_list_add_int(align_type, "Center", 2);

	obs_property_list_add_int(align_type, "Right", 3);

	obs_properties_add_color_alpha(props, "bg", obs_module_text("DaktronicsSource.BackgroundColor"));

	obs_properties_add_color_alpha(props, "fg", obs_module_text("DaktronicsSource.ForegroundColor"));

	obs_properties_add_color_alpha(props, "outline", obs_module_text("DaktronicsSource.OutlineColor"));

	obs_properties_add_float(props, "outlinew", obs_module_text("DaktronicsSource.OutlineWidth"), 0.0, INFINITY,
				 1.0);

	std::string info = "<a href=\"https://github.com/bkpeterson/obs-daktronics\">Daktronics Source</a> (";
	info += PLUGIN_VERSION;
	info += ") by bkpeterson";

	obs_properties_add_text(props, "plugin_info", info.c_str(), OBS_TEXT_INFO);

	return props;
}

bool DAKSource::DAKSportChanged(obs_properties_t *props, obs_property_t *property, obs_data_t *settings)
{
	UNUSED_PARAMETER(property);

	std::string sport_type = obs_data_get_string(settings, "dak_sport_type");

	obs_property_t *list = obs_properties_get(props, "dak_field_list");
	obs_property_list_clear(list);

	DAKSportData *sportData = DAKDataUtils::getSportData(sport_type);
	sportData->PopulateFieldProps(list);

	return true;
}

obs_source_info* DAKSource::create_daktronics_source_info()
{
    daktronics_source_info = {};
    daktronics_source_info.id = "daktronics_source";
    daktronics_source_info.type = OBS_SOURCE_TYPE_INPUT;
    daktronics_source_info.output_flags = OBS_SOURCE_VIDEO;
    daktronics_source_info.get_name = DAKSource::GetName;
    daktronics_source_info.create = DAKSource::Create;
    daktronics_source_info.destroy = DAKSource::Destroy;
    daktronics_source_info.get_width = DAKSource::GetWidth;
    daktronics_source_info.get_height = DAKSource::GetHeight;
    daktronics_source_info.video_render = DAKSource::Render;
    daktronics_source_info.get_defaults = DAKSource::GetDefaults;
    daktronics_source_info.get_properties = DAKSource::GetProperties;
    daktronics_source_info.update = DAKSource::Update;
    daktronics_source_info.icon_type = OBS_ICON_TYPE_TEXT;
    
    return &daktronics_source_info;
}
