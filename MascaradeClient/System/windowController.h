#ifndef WINDOW_CONTROLLER_H_INCLUDED
#define WINDOW_CONTROLLER_H_INCLUDED

class WindowController{
public:
	WindowController();
	~WindowController();

	//	call every frame just after input handler
	virtual void drawFrame() = 0;

	//	windowControl should start listen to notification when isController is true
	//	and stop listen to notification when isController is false
	//	caution to window size when isController is set to true
	virtual void setIsCurrentController(bool isController) = 0;
};

#endif
