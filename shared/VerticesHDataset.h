/*
 *  VerticesHDataset.h
 *  opium
 *
 *  Created by jian zhang on 6/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <HDataset.h>

class VerticesHDataset : public HDataset {
public:
	VerticesHDataset();
	VerticesHDataset(const std::string & path);
	virtual ~VerticesHDataset();
	
	void setNumVertices(int num);
	int numVertices() const;
	
	virtual char create();
};