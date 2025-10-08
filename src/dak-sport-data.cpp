#include <string>
#include <vector>

#include "dak-sport-data.hpp"

DAKSportData::DAKSportData(std::string sport, uint32_t dataSize) 
    : _sport(sport),
      _dataSize(dataSize) {

}
	
DAKSportData::~DAKSportData() {
    std::vector<DAKFieldData *>().swap(_fieldData);
}

void DAKSportData::AddFieldData(uint32_t index, std::string field, uint32_t length) {
    _fieldData.emplace_back(_sport, index, field, length);
}

std::string DAKSportData::GetSportName() {
    return _sport;
}
    
uint32_t DAKSportData::GetDataSize() {
    return _dataSize;
}

DAKFieldData* DAKSportData::GetField(uint32_t fieldIdx) {
    for(DAKFieldData* fieldData : _fieldData) {
        if(fieldData->_index == fieldIdx)
            return fieldData;
    }

    return nullptr;
}

uint32_t DAKSportData::GetFieldLen(uint32_t fieldIdx) {
    for(DAKFieldData* fieldData : _fieldData) {
        if(fieldData->_index == fieldIdx)
            return fieldData->_length;
    }

    return 0;
}

void DAKSportData::PopulateFieldProps(obs_property_t* fieldList) {
    for(DAKFieldData* fieldData : _fieldData) {
        obs_property_list_add_int(
                fieldList,
                fieldData->_field.c_str(),
                fieldData->_index);
    }
}


DAKFieldData::DAKFieldData(std::string sport, uint32_t index, uint32_t length, std::string field) 
    : _sport(sport),
      _index(index),
      _field(field),
      _length(length) {

}
    
DAKFieldData::~DAKFieldData() {

}
