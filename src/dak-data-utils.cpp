#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

#include "dak-sport-data.hpp"
#include "dak-data-utils.hpp"

std::map<std::string, DAKSportData *> DAKDataUtils::_allSportsData;
std::map<uint32_t, std::vector<DAKSource *>> DAKDataUtils::_sources;
std::map<uint32_t, std::vector<DAKFilter *>> DAKDataUtils::_filters;

void DAKDataUtils::read_csv_field(std::istringstream &ss, std::string &field)
{
	std::getline(ss, field, '\t');
}

void DAKDataUtils::read_csv_field(std::istringstream &ss, uint32_t &field)
{
	std::string temp;
	std::getline(ss, temp, '\t');
	field = std::stoi(temp);
}

uint32_t DAKDataUtils::readDataLine(std::istream &is, DAKTSVData &sportData)
{
	std::string line;
	if (std::getline(is, line)) {
		if (line.empty()) {
			//End of data category
			return 1;
		}

		std::istringstream iss(line);
		read_csv_field(iss, sportData.index);
		read_csv_field(iss, sportData.field);
		read_csv_field(iss, sportData.length);
	} else {
		//EOF
		return -1;
	}

	return 0;
}

uint32_t DAKDataUtils::readHeaderLine(std::istream &is, DAKTSVHeader &sportData)
{
	std::string line;
	if (std::getline(is, line)) {
		std::istringstream iss(line);
		read_csv_field(iss, sportData.sport);
		read_csv_field(iss, sportData.size);

		return 0;

	} else {
		//EOF
		return -1;
	}
}

void DAKDataUtils::populateSportsData()
{
	std::ifstream file("dak-data.tsv");
	if (!file.is_open()) {
		std::cerr << "Error opening file." << std::endl;

	} else {
		DAKTSVHeader sportDataLine;

		while (readHeaderLine(file, sportDataLine) == 0) {
			DAKSportData *sportData = new DAKSportData(sportDataLine.sport, sportDataLine.size);
			DAKTSVData fieldDataLine;

			while (readDataLine(file, fieldDataLine) == 0) {
				sportData->AddFieldData(fieldDataLine.index, fieldDataLine.field, fieldDataLine.length);
			}

			_allSportsData[sportData->GetSportName()] = sportData;
		}
	}
}

DAKSportData *DAKDataUtils::getSportData(std::string sport)
{
	for (std::pair<std::string, DAKSportData *> sportData : _allSportsData) {
		if (sportData.first == sport)
			return sportData.second;
	}

	return nullptr;
}

void DAKDataUtils::PopulateSportProps(obs_property_t *sportList)
{
	for (std::pair<std::string, DAKSportData *> sportData : _allSportsData) {
		obs_property_list_add_string(sportList, sportData.first.c_str(), sportData.first.c_str());
	}
}

void DAKDataUtils::AddSource(DAKSource *newSource)
{
	uint32_t index = newSource->GetIndex();
	_sources[index].push_back(newSource);
}

void DAKDataUtils::RemoveSource(DAKSource *oldSource)
{
	UNUSED_PARAMETER(oldSource);
	//uint32_t index = oldSource->GetIndex();
	//std::vector<DAKSource *> sources = _sources[index];

	//auto tmpVector = std::remove(sources.begin(), sources.end(), oldSource);
	//sources.erase(tmpVector, sources.end());
}

void DAKDataUtils::AddFilter(DAKFilter *newFilter)
{
	uint32_t index = newFilter->GetIndex();
	_filters[index].push_back(newFilter);
}

void DAKDataUtils::RemoveFilter(DAKFilter *oldFilter)
{
	UNUSED_PARAMETER(oldFilter);
	//uint32_t index = oldFilter->GetIndex();
	//std::vector<DAKFilter *> filters = _filters[index];

	//auto tmpVector = std::remove(filters.begin(), filters.end(), oldFilter);
	//filters.erase(tmpVector, filters.end());
}

void DAKDataUtils::UpdateField(uint32_t index, std::string value)
{
	std::vector<DAKSource *> sources = _sources[index];

	for (DAKSource *sourceData : sources) {
		sourceData->SetTextValue(value);
	}

	std::vector<DAKFilter *> filters = _filters[index];

	for (DAKFilter *filterData : filters) {
		filterData->SetVisible(value.length() > 0);
	}
}
