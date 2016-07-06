class SI7020_A20{
public:
    double temperatureC();
    double temperatureF();
private:
    int address = 0x40;
    int temperatureRegister = 227;
    int getRawTemperatureReading();
};