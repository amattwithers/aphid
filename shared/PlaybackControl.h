/*
 *  PlaybackControl.h
 *  mallard
 *
 *  Created by jian zhang on 10/9/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

class PlaybackControl {
public:
	PlaybackControl();
	
	int rangeMin() const;
	int rangeMax() const;
	int rangeLength() const;
	int currentFrame() const;
	virtual int playbackMin() const;
	virtual int playbackMax() const;
	void setCurrentFrame(int x);
	
	virtual void setFrameRange(int mn, int mx);
	virtual void disableControl();
	virtual void enableControl();
	
	bool isControlEnabled() const;

private:
	int m_rangeMin, m_rangeMax, m_currentFrame;
	bool m_enabled;
};