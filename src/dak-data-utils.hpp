#pragma once

#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <util/platform.h>
#include <inttypes.h>
#include <iostream>

#include "dak-sport-data.hpp"
#include "daktronics-filter.hpp"
#include "dak-serial.hpp"

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
	static void populateSportsData();
	static void clearSportsData();
	static DAKSportData *getSportData(std::string sport);

	static void AddFilter(DAKFilter *newFilter);
	static void RemoveFilter(DAKFilter *oldFilter);

	static void UpdateField(uint32_t index, std::string value);

	static void PopulateSportProps(obs_property_t *sportList);

	static void sync_init();
	static void sync_destroy();
	static void execute_global_tick_logic(void *data, uint32_t width, uint32_t height);
	static std::string getSerialPort();

	static void startSerial(std::string port);

private:
	static std::string _DAKData[4];
	static std::map<std::string, DAKSportData *> _allSportsData;
	static std::map<uint32_t, std::vector<DAKFilter *>> _filters;
	static std::unique_ptr<SerialPort> serial;

	static void read_csv_field(std::stringstream &ss, std::string &field);
	static void read_csv_field(std::stringstream &ss, uint32_t &field);
	static uint32_t readDataLine(std::stringstream &is, DAKTSVData &data);
	static uint32_t readHeaderLine(std::stringstream &is, DAKTSVHeader &data);
	static void onLineReceived(const std::string &line);
	static void onError(const std::string &error);
};
