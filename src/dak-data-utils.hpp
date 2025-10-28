#pragma once

#include <sstream>
#include <string>
#include <map>

#include "dak-sport-data.hpp"
//#include "daktronics-source.hpp"
#include "daktronics-filter.hpp"

struct DAKTSVHeader {
	std::string sport;
	uint32_t size;
};

struct DAKTSVData {
	uint32_t index;
	std::string field;
	uint32_t length;
};

class DAKDataUtils {
public:
	static std::map<std::string, DAKSportData *> _allSportsData;

	static void populateSportsData();
	static DAKSportData *getSportData(std::string sport);

	static void AddFilter(DAKFilter *newFilter);
	static void RemoveFilter(DAKFilter *oldFilter);

	static void UpdateField(uint32_t index, std::string value);

	static void PopulateSportProps(obs_property_t *sportList);

private:
	static std::map<uint32_t, std::vector<DAKFilter *>> _filters;

	static void read_csv_field(std::stringstream &ss, std::string &field);
	static void read_csv_field(std::stringstream &ss, uint32_t &field);
	static uint32_t readDataLine(std::stringstream &is, DAKTSVData &data);
	static uint32_t readHeaderLine(std::stringstream &is, DAKTSVHeader &data);
};
