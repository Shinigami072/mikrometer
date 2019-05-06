#ifndef AGGREGATOR_H
#define AGGREGATOR_H

//datastructure used for mean and variance calculation
//using welfords online algorythm
class Aggregator
{
  private:
	unsigned long count;
	double mean;
	double M2;

  public:
	Aggregator();

	~Aggregator();

	/**
	 *	add data to aggregator - using welfords algorythm
	 *  */
	void addData(double datum);

	unsigned long getCount();
	double getMean();
	double getVar();
	double getStd();

	/**
	 * resets aggregator - sets all values to 0
	 * 
	 */
	void reset();
};

#endif //AGGREGATOR_H