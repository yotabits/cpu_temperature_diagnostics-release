#pragma once
#include <string>
#include <vector>

#include <sensors/sensors.h>

#include "cpu_temperature_diagnostics/temperature_feature.hh"
namespace cpu_temperature_diagnostics
{
class SensorChip
{
  public:
    /**
     * @brief Construct a new Sensor Chip object
     *
     * @param chip  chip that the SensorChip object is abstracting
     * @param default_critical_temp Critical temp used if not provided by the
     * chip
     * @param default_max_temp Max temp used if not provided by the chip
     */
    SensorChip(const sensors_chip_name* chip,
               double default_critical_temp = 100,
               double default_max_temp = 85);

    std::vector<temperature_info> get_temperature_readings() const;
    std::string get_identifier() const;
    void critical_temp_override(double critical_temp);
    void max_temp_override(double max_temp);

  private:
    std::vector<TemperatureFeature> temperature_features_;
    const sensors_chip_name* chip_name_;
};

namespace sensors_chip_factory
{
std::vector<SensorChip> get_chips_with_prefix(const std::string& name);
}  // namespace sensors_chip_factory

}  // namespace cpu_temperature_diagnostics
