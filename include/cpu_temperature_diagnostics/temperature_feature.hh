#pragma once
#include <optional>

#include <sensors/sensors.h>

namespace cpu_temperature_diagnostics
{
struct temperature_info
{
    std::string label_;
    double temperature_ = 0;
    bool is_ok_ = false;
};

class TemperatureFeature
{
  public:
    static std::optional<TemperatureFeature>
    make_temp_feature(const sensors_chip_name* chip_name,
                      const sensors_feature* feature,
                      double default_critical_temp = 100,
                      double defaut_max_temp = 85);
    temperature_info
    read_temperature_info(const sensors_chip_name* chip_name) const;
    std::string get_label() const;

    void set_critical_temp(double critical_temp)
    {
        crit_temp_ = critical_temp;
    }

    void set_max_temp(double max_temp)
    {
        max_temp_ = max_temp;
    }

  private:
    TemperatureFeature(const sensors_chip_name* chip_name,
                       const sensors_feature* feature,
                       double default_critical_temp,
                       double defaut_max_temp);
    std::string label_;
    double max_temp_;
    double crit_temp_;
    const sensors_subfeature* input_temp_subfeature_;
};
}  // namespace cpu_temperature_diagnostics