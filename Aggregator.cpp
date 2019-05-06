#include "Aggregator.h"
#include "math.h"

Aggregator::Aggregator(/* args */) : count(0), mean(0.0), M2(0.0)
{
}

Aggregator::~Aggregator()
{
}
void Aggregator::reset()
{
	mean = 0.0;
	M2 = 0.0;
	count = 0;
}
void Aggregator::addData(double datum)
{
	//implementation of welfords algorythm
	++count;
	double delta = datum - mean;
	mean += delta / count;
	double delta2 = datum - mean;
	M2 += delta * delta2;
}
unsigned long
Aggregator::getCount()
{
	return count;
}
double
Aggregator::getMean()
{
	return mean;
}
double
Aggregator::getVar()
{
	return M2 / (count - 1);
}
double
Aggregator::getStd()
{
	return sqrt(getVar());
}