#include "Order.h"

void Order::setPriority(int mode)
{
	if (mode == 0)
		priority = (type == TYPE_VIP) ? (400.0 / arrivalTime + 100.0 / distance + 0.1 * totalMoney) : -1;

	else if (mode == 1)
		priority = -finishTime;
}

void Order::setType(ORDER_TYPE newType)
{
	type = newType;
}

bool Order::promote(int extraMoney)
{
	if (type == TYPE_VIP || type == TYPE_FROZEN)
		return false;

	type = TYPE_VIP;
	totalMoney += extraMoney;
	setPriority();
	return true;
}

Order::Order(int r_Time, ORDER_TYPE r_Type, int r_ID, int r_Distance, int r_Money, REGION r_region)
{
	arrivalTime = r_Time;
	type = r_Type;
	ID = (r_ID > 0 && r_ID < 1000) ? r_ID : 0;	//1<ID<999
	distance = r_Distance;
	totalMoney = r_Money;
	region = r_region;
	toBeDeleted = false;
	setPriority();
}

Order::~Order()
{
}

int Order::getID() const
{
	return ID;
}

bool Order::operator ==(int id) const
{
	return ID == id;
}


bool Order::operator>(Order & right) const
{
	return priority > right.priority;
}

ORDER_TYPE Order::GetType() const
{
	return type;
}

REGION Order::GetRegion() const
{
	return region;
}

void Order::setTimes(int startTime, int speed)
{
	finishTime = startTime + ceil(distance * 1.0 / speed);
	waitTime = startTime - arrivalTime;
	serviceTime = ceil(distance * 1.0 / speed);
}




void Order::setDistance(int d)
{
	distance = d > 0 ? d : 0;
}

int Order::getDistance() const
{
	return distance;
}

bool Order::isDeleted() const
{
	return toBeDeleted;
}

void Order::deleteIt(bool del)
{
	toBeDeleted = del;
}

int Order::getFinishTime() const
{
	return finishTime;
}

int Order::getArrivalTime() const
{
	return arrivalTime;
}

int Order::getWaitTime() const
{
	return waitTime;
}

int Order::getServiceTime() const
{
	return serviceTime;
}

void Order::writeData(ofstream & outFile) const
{
	outFile << finishTime << "\t" << ID << "\t" << arrivalTime << "\t" << waitTime << "\t" << serviceTime << endl;
}
