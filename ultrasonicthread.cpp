/*
	module : 28015 REV B
*/

#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <thread>
#include <mutex>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
//class Sonic;
using namespace std;
mutex mtx;
//using sonic_uptr = std::unique_ptr<Sonic>;
//static sonic_uptr sonic_one{nullptr};

/*
class SonicManager{
private:

public:

}
*/

class Sonic{
private:
	int pin_num;
	struct timeval time1,time2;
public:
	Sonic(int);
	inline double gettime();

	void operator()(){
		//int n = 100;
		//int i = 0;

		//while(i<n){
			mtx.lock();
			pinMode(pin_num,OUTPUT);
			digitalWrite(pin_num,1);
			pinMode(pin_num,INPUT);
			while(digitalRead(pin_num)!=1);
			gettimeofday(&time1,nullptr);
			while(digitalRead(pin_num)!=0);
			gettimeofday(&time2,nullptr);
			mtx.unlock();
			//i++;
			printf("%lf\n",gettime());
		//}
	}
};


inline double Sonic::gettime(){
	lock_guard<mutex> lock(mtx);
	return (time2.tv_sec-time1.tv_sec)+(time2.tv_usec-time1.tv_usec)*1.0E-6;
}

Sonic::Sonic(int n){
	pin_num = n;
}


int main(){
	if(wiringPiSetup()!=0){
		printf("error wiringPi setup\n");
		return 1;
	}else{
		printf("success wiringPi setup\n");
	}
	Sonic sonic_one(4);
	//sonic_one();
	//printf("%lf\n",sonic_one.gettime());
	thread t1(sonic_one);
	t1.join();
	return 0;
}
