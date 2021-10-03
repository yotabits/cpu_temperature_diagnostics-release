#include "cpu_temperature_diagnostics/temperature_feature.hh"

#include <optional>

#include <sensors/sensors.h>

namespace
{
std::optional<double> read_subfeature(const sensors_chip_name* chip_name,
                                      const sensors_subfeature* subfeature)
{
    double value_read;
    int return_value =
        sensors_get_value(chip_name, subfeature->number, &value_read);
    if (return_value < 0)
    {
        return std::nullopt;
    }
    else
    {
        return value_read;
    }
}
}  // namespace

namespace cpu_temperature_diagnostics
{
std::optional<TemperatureFeature>
TemperatureFeature::make_temp_feature(const sensors_chip_name* chip_name,
                                      const sensors_feature* feature,
                                      double default_critical_temp,
                                      double defaut_max_temp)
{
    if (feature->type == SENSORS_FEATURE_TEMP)
    {
        return { TemperatureFeature(
            chip_name, feature, default_critical_temp, defaut_max_temp) };
    }
    else
    {
        return std::nullopt;
    }
}

TemperatureFeature::TemperatureFeature(const sensors_chip_name* chip_name,
                                       const sensors_feature* feature,
                                       double default_critical_temp,
                                       double defaut_max_temp)
{
    sensors_subfeature const* subfeature;
    int subf_num = 0;
    label_ = std::string(sensors_get_label(chip_name, feature));
    while ((subfeature = sensors_get_all_subfeatures(
                chip_name, feature, &subf_num)) != nullptr)
    {
        // A not readable feature has no interrest for us
        if (subfeature->flags & SENSORS_MODE_R)
        {
            if (subfeature->type == SENSORS_SUBFEATURE_TEMP_INPUT)
            {
                input_temp_subfeature_ = subfeature;
            }
            else if (subfeature->type == SENSORS_SUBFEATURE_TEMP_CRIT)
            {
                auto crit_read = read_subfeature(chip_name, subfeature);
                crit_temp_ = crit_read.value_or(default_critical_temp);
            }
            else if (subfeature->type == SENSORS_SUBFEATURE_TEMP_MAX)
            {
                auto max_read = read_subfeature(chip_name, subfeature);
                max_temp_ = max_read.value_or(defaut_max_temp);
            }
        }
    }
}

std::string TemperatureFeature::get_label() const
{
    return label_;
}

temperature_info TemperatureFeature::read_temperature_info(
    const sensors_chip_name* chip_name) const
{
    temperature_info ret_info;
    if (auto temp_read = read_subfeature(chip_name, input_temp_subfeature_))
    {
        ret_info.temperature_ = temp_read.value();
        ret_info.is_ok_ = (temp_read.value() < max_temp_);
        ret_info.label_ = label_;
    }
    return ret_info;
}

}  // namespace cpu_temperature_diagnostics
