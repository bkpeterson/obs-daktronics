#pragma once

#include <string>
#include <vector>

#include "obs-module.h"

class DAKSportData {
public:
	DAKSportData(std::string sport, uint32_t dataSize);
	~DAKSportData();

    void AddFieldData(uint32_t index, std::string field, uint32_t length);

	std::string GetSportName();
    uint32_t GetDataSize();

    DAKFieldData *GetField(uint32_t fieldIdx);
    uint32_t GetFieldLen(uint32_t fieldIdx);

    void PopulateFieldProps(obs_property_t* fieldList);

private:
    std::string _sport;
    uint32_t _dataSize;
    std::vector<DAKFieldData *> _fieldData;
};

class DAKFieldData {
public:
    DAKFieldData(std::string sport, uint32_t index, uint32_t length, std::string field);
    ~DAKFieldData();

    std::string _sport;
    uint32_t _index;
    uint32_t _length;
    std::string _field;
    std::string _curValue;
};
