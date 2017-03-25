/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileRom.cpp
 * Author: dinf0014_admin
 * 
 * Created on 8 mars 2017, 11:53
 */

#include "file_rom.h"

#include <fstream>

using namespace Frankenstein;

FileRom::FileRom(std::string filename) : romData()
{
    std::fstream file(filename, std::ios_base::binary | std::ios_base::in);
    if (!file.eof() && !file.fail())
    {
        file.seekg(0, std::ios_base::end);
        std::streampos fileSize = file.tellg();
        romData.resize(fileSize);

        file.seekg(0, std::ios_base::beg);
        file.read(&romData[0], fileSize);
    }
    else {
        throw "File does not exist";
    }
}

FileRom::~FileRom()
{
}
    
const iNesHeader* FileRom::GetHeader() const{
    iNesHeader* header = new iNesHeader;
    for (unsigned int i = 0; i < sizeof(iNesHeader); ++i){
        header->raw[i] = this->GetRaw()[i];
    }
    return header;
}

const char* FileRom::GetRaw() const{
    return &romData[0];
}

unsigned int FileRom::GetLength() const {
    return romData.size();
}

unsigned int FileRom::GetTrainerOffset() const{
	return CheckBit<3>(this->GetHeader()->controlByte1) * Rom::TrainerSize;
}
