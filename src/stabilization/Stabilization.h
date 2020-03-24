#ifndef STABILIZATION_H_
#define STABILIZATION_H_

#include "Attitude.h"
#include "ESC.h"
#include "PID.h"
#include "Reception.h"
#include "PIDConstants.h"

class Stabilization {
  private:
    const float mixing = 0.5;

    int rollPosCmd, pitchPosCmd = 0;
    int rollMotorPwr, pitchMotorPwr, yawMotorPwr = 0;
    float speedCurr[3] = {0.0, 0.0, 0.0}; // Teta speed (°/s) (only use gyro)
    float posCurr[3] = {0.0, 0.0, 0.0};   // Teta position (°) (use gyro + accelero)
    uint16_t throttle = 0;
    ESC ESCs;
    PID rollPosPID_Angle, pitchPosPID_Angle, yawPosPID_Angle;
    PID rollSpeedPID_Angle, pitchSpeedPID_Angle, yawSpeedPID_Angle;
    PID rollSpeedPID_Accro, pitchSpeedPID_Accro, yawSpeedPID_Accro;
    Attitude attitude;
    Reception Rx;

  public:
    void SetMotorsPwrXConfig();
    void Init();
    void Idle();
    void Accro(float _loopTimeSec);
    void Angle(float _loopTimeSec);
    void PrintAccroModeParameters();
    void PrintAngleModeParameters();
    void ResetPID();
    void SetESCsPWM(volatile uint16_t *TCNTn, volatile uint16_t *OCRnA) {
        ESCs.SetPWM_f5(TCNTn, OCRnA);
    }
    int GetESCsMaxPower() {
        return ESCs.GetESCsMaxPower();
    }
    int GetESCsMinPower() {
        return ESCs.GetESCsMinPower();
    }
    int GetESCsMaxThrottlePercent() {
        return ESCs.GetESCsMaxThrottlePercent();
    }
    int GetESCsMaxThrottle() {
        return ESCs.GetESCsMaxThrottle();
    }
    int GetESCIdleThreshold() {
        return ESCs.GetESCIdleThreshold();
    }
    bool AreAttitudeOffsetsComputed() {
        return attitude.AreOffsetComputed();
    }
    void AttitudeComputeOffsets() {
        attitude.ComputeOffsets();
    }
    inline void ComputeRxImpulsionWidth() {
        Rx.GetWidth();
    }
    inline int GetFlyingMode() {
        return Rx.GetFlyingMode();
    }
    inline int GetThrottle() {
        return Rx.GetThrottle(GetESCsMinPower(), GetESCsMaxThrottle());
    }

    bool IsThrottleIdle() {
        return GetThrottle() < GetESCIdleThreshold();
    }
};
#endif // STABILIZATION_H_