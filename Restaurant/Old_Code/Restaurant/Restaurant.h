#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"


#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> eventsQueue;	//Queue of all events that will be loaded from file

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Restaurant();
	~Restaurant();
	void addEvent(Event* pE);	//adds a new event to the queue of events
	void executeEvents(int TimeStep);	//executes all events at current timestep
	void runSimulation();

	/// ==> 
	///  DEMO-related functions. Should be removed in phases 1&2
	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue
	Order* getDemoOrder();			//return the front order from demo queue
	/// ==> 


	//
	// TODO: Add More Member Functions As Needed
	//

};

#endif