/*
 *  CacheFile.h
 *  mallard
 *
 *  Created by jian zhang on 10/10/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
 
#pragma once
#include <HFile.h>
#include <BaseState.h>
#include <map>
#include <AllMath.h>
class HBase;
class HDataset;
class CacheFile : public HFile, public BaseState {
public:
    CacheFile();
	CacheFile(const char * name);
    
	virtual bool create(const std::string & fileName);
	virtual bool open(const std::string & fileName);
	virtual bool save();
	virtual bool close();
	
	void addEntry(const std::string & name);
	void addSliceVector3(const std::string & entryName, const std::string & sliceName);
	void openSlice(const std::string & entryName, const std::string & sliceName);
	void closeSlice(const std::string & entryName, const std::string & sliceName);
	
	void writeSliceVector3(const std::string & entryName, const std::string & sliceName, unsigned start, unsigned count, Vector3F * data);

private:
	std::map<std::string, HBase *> m_entries;
	std::map<std::string, HDataset *> m_slices;
};

