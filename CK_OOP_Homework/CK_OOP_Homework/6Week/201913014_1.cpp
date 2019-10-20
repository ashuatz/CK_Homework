#include <iostream>
#include <cstdlib>
#include <random>
using namespace std;


class Car
{
private:
	int gasolineGauge;
protected:
	Car() :gasolineGauge(rand() % 100) {}
	int GetGasGauge() { return gasolineGauge; }
};

class HybridCar : public Car
{
private:
	int electricGauge;
protected:
	HybridCar() :electricGauge(rand() % 100) {}
	int GetElectricGauge() { return electricGauge; }
};

class HybridWaterCar : public HybridCar
{
private:
	int waterGauge;
public:
	HybridWaterCar() :waterGauge(rand() % 100) {}
	void ShowCurrentGauge()
	{
		cout << "ÀÜ¿©°¡¼Ö¸° : " << GetGasGauge() << endl;
		cout << "ÀÜ¿©Àü±â·® : " << GetElectricGauge() << endl;
		cout << "ÀÜ¿©¿öÅÍ·® : " << waterGauge << endl;
	}
};

int main()
{
	srand(random_device()());
	HybridWaterCar car;
	car.ShowCurrentGauge();
}