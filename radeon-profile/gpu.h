// copyright marazmista @ 29.03.2014

// in here, we have class which represent graphic adapter in program //
// It chooses driver and communicate with proper class for selected driver //
// and retuns result to gui //

#ifndef GPU_H
#define GPU_H

#include "globalStuff.h"
#include "dxorg.h"
#include <QtConcurrent/QtConcurrent>

class gpu : public QObject
{

    Q_OBJECT
public:
    explicit gpu(QObject *parent = 0 ) : QObject(parent), currentGpuIndex(0), driverHandler(nullptr) {
        connect(&futureGpuUsage, SIGNAL(finished()),this,SLOT(handleGpuUsageResult()));
    }

    ~gpu() {
        if (driverHandler != nullptr)
            delete driverHandler;
    }

    // main map that has all info available by ValueID
    GPUDataContainer gpuData;
    QList<GPUSysInfo> gpuList;

    char currentGpuIndex;
    QString currentPowerProfile, currentPowerLevel;

    QList<QTreeWidgetItem *> getCardConnectors() const;
    QStringList getGLXInfo(QString gpuName) const;
    QList<QTreeWidgetItem *> getModuleInfo() const;
    QString getCurrentPowerLevel();
    QString getCurrentPowerProfile();
    void getPowerLevel();

    void getClocks();
    void getTemperature();
    void getFanSpeed();
    void getGpuUsage();
    void getPowerCapSelected();
    void getPowerCapAverage();

    void changeGpu(int index);
    void setPowerProfile(const QString &newPowerProfile);
    void setForcePowerLevel(const QString &newForcePowerLevel);
    void setPwmManualControl(bool manual);
    void setPwmValue(unsigned int value);
    void setOcTableValue(const QString &type, const QString &tableKey, int powerState, const FreqVoltPair powerStateValues);
    void sendOcTableCommand(const QString cmd);
    void setOcRanges(const QString &type, const QString &tableKey, int powerState, int rangeValue);
    void setPowerCap(const unsigned value);
    void setManualFrequencyControlStates(const QString &file, const QString &states);
    void resetFrequencyControlStates();
    void refreshPowerPlayTables();

    void detectCards();
    bool initialize(const dXorg::InitializationConfig &config);
    void setOverclockValue(const QString &file, int value);
    void resetOverclock();
    const DriverFeatures& getDriverFeatures() const;
    const GPUConstParams& getGpuConstParams() const;
    const DeviceFilePaths& getDriverFiles() const;
    void finalize();
    bool isInitialized();
    int getCurrentPowerPlayTableId(const QString &file);
    void readOcTableAndRanges();
    void setOcTable(const QString &tableType, const FVTable &table);

private slots:
    void handleGpuUsageResult() {
        GPUUsage tmp = futureGpuUsage.result();

        if (gpuData.contains(ValueID::GPU_USAGE_PERCENT))
            gpuData[ValueID::GPU_USAGE_PERCENT].setValue(tmp.gpuUsage);

        if (gpuData.contains(ValueID::GPU_VRAM_USAGE_MB))
            gpuData[ValueID::GPU_VRAM_USAGE_MB].setValue(tmp.gpuVramUsage);

        if (gpuData.contains(ValueID::GPU_VRAM_USAGE_PERCENT)) {
            gpuData[ValueID::GPU_VRAM_USAGE_PERCENT].setValue(tmp.gpuVramUsagePercent);
        }
    }

private:
    dXorg *driverHandler;
    void defineAvailableDataContainer();
    QFutureWatcher<GPUUsage> futureGpuUsage;

};

#endif // GPU_H
