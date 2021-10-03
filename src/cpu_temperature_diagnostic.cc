#include "cpu_temperature_diagnostics/cpu_temperature_diagnostic.hh"

#include <string>

#include "cpu_temperature_diagnostics/sensor_chip.hh"

namespace cpu_temperature_diagnostics
{
CpuTemperatureDiagnostic::CpuTemperatureDiagnostic(SensorChip& chip)
  : chip_(chip)
{
    diagnostics_updater_.setHardwareID(chip_.get_identifier());

    auto diagnostic_pub_func =
        std::bind(&CpuTemperatureDiagnostic::diagnostics_callback,
                  this,
                  std::placeholders::_1);

    diagnostics_updater_.add("CPU temperature", diagnostic_pub_func);
}

void CpuTemperatureDiagnostic::diagnostics_callback(
    diagnostic_updater::DiagnosticStatusWrapper& stat) const
{
    auto is_ok = true;
    auto readings = chip_.get_temperature_readings();
    for (const auto& read : readings)
    {
        if (!read.is_ok_)
        {
            is_ok = read.is_ok_;
        }
        std::string status_string = read.is_ok_ ? "OK" : "Error";
        stat.add(read.label_,
                 std::to_string(int(read.temperature_)) + "°C " +
                     status_string);
    }
    std::string summary =
        is_ok ? "CPU Temperature is OK" : "CPU Temperature is too hot";

    is_ok ? stat.summary(diagnostic_msgs::DiagnosticStatus::OK, summary) :
            stat.summary(diagnostic_msgs::DiagnosticStatus::ERROR, summary);
}

void CpuTemperatureDiagnostic::emit_diagnostics()
{
    diagnostics_updater_.update();
}

void CpuTemperatureDiagnostic::critical_temp_override(int critical_temp)
{
    chip_.critical_temp_override(critical_temp);
}

void CpuTemperatureDiagnostic::max_temp_override(int max_temp)
{
    chip_.max_temp_override(max_temp);
}

}  // namespace cpu_temperature_diagnostics
