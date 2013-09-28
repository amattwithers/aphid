/*
 *  HFeather.cpp
 *  mallard
 *
 *  Created by jian zhang on 9/28/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "HFeather.h"
#include <MlFeather.h>
HFeather::HFeather(const std::string & path) : HBase(path) {}

char HFeather::save(MlFeather * feather)
{
	feather->verbose();
	
	int id = feather->featherId();
	if(!hasNamedAttr(".featherId"))
		addIntAttr(".featherId");
		
	writeIntAttr(".featherId", &id);
	
	int nseg = feather->numSegment();
	
	if(!hasNamedAttr(".nseg"))
		addIntAttr(".nseg");
		
	writeIntAttr(".nseg", &nseg);
	
	if(!hasNamedData(".quill"))
		addFloatData(".quill", nseg);
		
	writeFloatData(".quill", nseg, feather->quilly());
	std::cout<<" q "<<feather->quilly()[0]<<"\n";
	
	float b[4];
	readFloatData(".quill", nseg, b);
	std::cout<<" qi "<<b[0]<<"\n";
	
	int nvv = feather->numVaneVertices();
	if(!hasNamedAttr(".nvv"))
		addIntAttr(".nvv");
		
	writeIntAttr(".nvv", &nvv);
	
	if(!hasNamedData(".vv"))
		addFloatData(".vv", nvv * 2);
		
	writeFloatData(".vv", nvv * 2, (float *)feather->vane());
	
	return 1;
}

char HFeather::load(MlFeather * feather)
{
	if(!hasNamedAttr(".featherId"))
		return 0;
	
	int id = 0;
	readIntAttr(".featherId", &id);
	
	feather->setFeatherId(id);
	
	int nseg = 4;
	if(!hasNamedAttr(".nseg"))
		return 0;
		
	readIntAttr(".nseg", &nseg);
	
	feather->createNumSegment(nseg);
	
	if(!hasNamedData(".quill"))
		return 0;
		
	readFloatData(".quill", nseg, feather->quilly());
	
	int nvv = feather->numVaneVertices();
	if(!hasNamedAttr(".nvv"))
		return 0;
		
	readIntAttr(".nvv", &nvv);
	
	if(!hasNamedData(".vv"))
		return 0;
		
	readFloatData(".vv", nvv * 2, (float *)feather->vane());
	
	feather->computeLength();
	feather->verbose();
	
	return 1;
}